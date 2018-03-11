#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <sqlite3.h> 
#include <time.h>

/* portul folosit */
#define PORT 6565

#define SQLITE_CONFIG_MULTITHREAD   1

#define nr_intrebari_de_pus 3 //se poate schimba numerul de intrebari puse per sesiune

/* codul de eroare returnat de anumite apeluri */
extern int errno;

static void *treat(void *); /* functia executata de fiecare thread ce realizeaza comunicarea cu clientii */


typedef struct camera{ //sructura camerei de joc - aici se vor pune 
    int cl1; //descriptoul pt primul client din joc
    int cl2; //descriptoul pt al doilea client din joc
	int cl3; //descriptoul pt al treilea client din joc
}camera;

typedef struct Thread{
	pthread_t idThread; //id-ul thread-ului
	int cl;	
	int idTh;
	int id;
}Thread;

Thread *threadsPool; //un array de structuri Thread

int sd; //descriptorul de socket de ascultare




pthread_mutex_t mlock=PTHREAD_MUTEX_INITIALIZER; // variabila mutex ce va fi partajata de threaduri



int disponibili;
int m=0;
int oku=1,qui=0,qui1=0,qui2=0,max_id_quiz;
char intrebare[2000]=" ",raspunsuri[2000]=" ",new_par[20]=" ",user_pt_joaca[20]=" ";
int corect,max_id,max_id2,exista=0,rez=0,rez1=0,rez2=0,max_id_ses;
int ar0,pp=0,id_nou,utilizator_logat=0,este=0;
char infos[200];




//===============functii de callback pt baza de date QUIZ ===========
static int callback(void *NotUsed, int argc, char **argv, char **azColName){ //pune in variabila "intrebare" rezultatul query-ului corespunzator (intrebarea de la un id)
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	  strcpy(intrebare,argv[i]);
   }
   printf("\n");
   return 0;
}

static int callback_raspunsuri(void *NotUsed, int argc, char **argv, char **azColName){//pune in variabila "raspunsuri" rezultatul query-ului corespunzator (raspunsurile la intrebarea de la un id)
   bzero(raspunsuri,800);
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	  strcat(raspunsuri,argv[i]);
	  strcat(raspunsuri,"\n");
   }
   printf("\n");
   return 0;
}

static int callback_corect(void *NotUsed, int argc, char **argv, char **azColName){//pune in variabila "corect" rezultatul query-ului corespunzator (continutul campului CORECT de la un id)
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	  corect=atoi(argv[i]);
   }
   printf("\n");
   return 0;
}

static int callback_max(void *NotUsed, int argc, char **argv, char **azColName){//pune in variabila "max_id" rezultatul query-ului corespunzator (numarul de rows al tabelei QUIZ)
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	  max_id=atoi(argv[i]);
   }
   printf("\n");
   return 0;
}		 
	
static int callback_quiz(void *NotUsed, int argc, char **argv, char **azColName){ //afiseaza continutul tablei
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

static int callback_max2(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	  max_id_quiz=atoi(argv[i]);
   }
   printf("\n");
   return 0;
}

static int callback_afisare_quiz(void *NotUsed, int argc, char **argv, char **azColName){ //pune in "infos" toate informatiile cerute (campurile unei intrari din tabela)
   int i;
   bzero(infos,200);
   
   for(i=0; i<argc; i++)
   {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	  if(i==0) //pentru prima coloana
	  {
		  strcpy(infos,azColName[i]); //numele coloanei
		  strcat(infos," : ");
		  strcat(infos,argv[i]); //continutul acesteia in functie de id/nume etc.
		  
	  }
	  else 		//pentru celelalte coloane pana la ultima
	  {
		 
		  strcat(infos, "\n"); //idem mai sus
		  strcat(infos,azColName[i]);
		  strcat(infos," : ");
		  strcat(infos,argv[i]);
	  }
	}
   printf("\n");
   strcat(infos, "\n");
   return 0;
}



void actiuni_admin(int optiune,char intrebare[800],char rasp1[800],char rasp2[800],char rasp3[800],char rasp4[800],int corect)
{
		sqlite3 *db;
		char *zErrMsg = 0;
		int  rc, max_id_quiz2;
		char *sql;

		/* Open database */
		
	    rc = sqlite3_open("quiz.db", &db);
	    if( rc )
	    {
		  fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		  return(0);
	    }
	
	
	    if(optiune==1) //facem o noua intrare in tabela (o noua intrebare cu raspunsurile aferente)
		{				//pe pozitia exact urmatoare id-ului maxim din tabela
		
		
			sql= "select max(id) from QUIZ"; //selecteaza id-ul maxim curent
											 
		
			/* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_max2, 0, &zErrMsg);
			if( rc != SQLITE_OK )
			{
			  fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
	   
			max_id_quiz2=max_id_quiz+1; //adauga 1 la acesta => max_id_quiz2 va fi locul unde se insereaza intrebarea si restul informatiilor
				
			sql= sqlite3_mprintf("INSERT INTO QUIZ (ID,INTREBARE,RASP1,RASP2,RASP3,RASP4,CORECT) VALUES (%i, '%q', '%q', '%q','%q','%q',%i ); ",max_id_quiz2, intrebare,rasp1,rasp2,rasp3,rasp4,corect) ;

			 /* Execute SQL statement */
			 rc = sqlite3_exec(db, sql, callback_quiz, 0, &zErrMsg);
			 if( rc != SQLITE_OK ){
			 fprintf(stderr, "SQL error: %s\n", zErrMsg);
			 sqlite3_free(zErrMsg);
			 }
			 sqlite3_close(db);
		}
		if(optiune==4) //selectam id-ul maxim ca sa stergem ultima intrebare din baza de date quiz.bd
		{
			
			sql= "select max(id) from QUIZ";  //selecteaza id-ul maxim curent
		
			/* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_max2, 0, &zErrMsg);
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
	  
				
			sql= sqlite3_mprintf("DELETE  from QUIZ where id=%i",max_id_quiz) ;

		   /* Execute SQL statement */
		   rc = sqlite3_exec(db, sql, callback_quiz, 0, &zErrMsg); //stergem acea intrebare (de la id-ul maxim)
		   if( rc != SQLITE_OK ){
		   fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
		   }
		   sqlite3_close(db);
		}
	
	
}



	
void functie(int optiune,int id){
	
	sqlite3 *db;
    char *zErrMsg = 0;
    int  rc;
    char *sql;

		/* Open database */
	   rc = sqlite3_open("quiz.db", &db);
	   if( rc )
		{
		  fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		  return(0);
		}
		
	   sqlite3_busy_timeout(db, 500); 
	   
	   if(optiune==0)		//optiunea de creare a tabelei QUIZ
	   {			
			sql = "CREATE TABLE QUIZ("  \
				 "ID INT        NOT NULL," \
				 "INTREBARE     CHAR(800)   NOT NULL," \
				 "RASP1         CHAR(800)     NOT NULL," \
				 "RASP2         CHAR(800)," \
				 "RASP3         CHAR(800)," \
				 "RASP4         CHAR(800)," \
				 "CORECT         INT );";
				 
		   rc = sqlite3_exec(db, sql, callback_quiz, 0, &zErrMsg);
		   if( rc != SQLITE_OK ){
		   fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
		   }
		   
		   sqlite3_close(db);
		 
	   }
	   if(optiune==88)		//popularea initiala a tabelei QUIZ
	   {
		   
				sql = "INSERT INTO QUIZ (ID,INTREBARE,RASP1,RASP2,RASP3,RASP4,CORECT) "  \
			 "VALUES (1, 'Cat face 2+2?', '1. 2', '2. Delfin', '3. 4,7','4. 4',4 ); " \
			 "INSERT INTO QUIZ (ID,INTREBARE,RASP1,RASP2,RASP3,RASP4,CORECT) "  \
			 "VALUES (2, 'In ce anotimp suntem?', '1. Nu stiu', '2. Vara', '3. Toamna','4. Iarna', 4); "     \
			 "INSERT INTO QUIZ (ID,INTREBARE,RASP1,RASP2,RASP3,RASP4,CORECT)" \
			 "VALUES (3, 'Care e casa cea mai tare?', '1. Griffindor', '2. Hufflepuff', '3. Ravenclaw','4. Slitherin',4 ); " \
			 "INSERT INTO QUIZ (ID,INTREBARE,RASP1,RASP2,RASP3,RASP4,CORECT)" \
			 "VALUES (4, 'Care e prima litera in alfabet?', '1. G ', '2. 10 ', '3. Z','4. A',4 ); "\
			 "INSERT INTO QUIZ (ID,INTREBARE,RASP1,RASP2,RASP3,RASP4,CORECT)" \
			 "VALUES (5, 'Ce inseamna WAN?', '1.World Area Net', '2. Wide Antenna Network', '3. Wide Area Network','4. World Area Network', 4); "     \
			 "INSERT INTO QUIZ (ID,INTREBARE,RASP1,RASP2,RASP3,RASP4,CORECT)" \
			 "VALUES (6, 'Ce inseamna WAN?', '1.World Area Net', '2. Wide Antenna Network', '3. Wide Area Network','4. World Area Network', 4); "     \
			 "INSERT INTO QUIZ (ID,INTREBARE,RASP1,RASP2,RASP3,RASP4,CORECT)" \
			 "VALUES (7, 'Ce e o retea de calculatoare?', '1. Cateva calculatoare intr-o sala', '2. Un numar de calculatoare conectate intre ele', '3. Un tip de calculator','4. Modul de legare al unor calculatoare',2 ); " \
			 "INSERT INTO QUIZ (ID,INTREBARE,RASP1,RASP2,RASP3,RASP4,CORECT)" \
			 "VALUES (8, 'If a datagram router goes down then....?', '1.  all packets will suffer', '2. only those packets which are queued in the router at that time will suffer', '3. only those packets which are not queued in the router at that time will suffer','4. no packets will suffer',2 ); " \
			 "INSERT INTO QUIZ (ID,INTREBARE,RASP1,RASP2,RASP3,RASP4,CORECT)" \
			 "VALUES (9, ' In datagram subnet new route is chosen?', '1. for every packet sent', '2. for all the packet sent', '3. only for the first packet','4. for the packet which is not transmitted',1); " \
			 "INSERT INTO QUIZ (ID,INTREBARE,RASP1,RASP2,RASP3,RASP4,CORECT)" \
			 "VALUES (10, 'For a connection oriented service, we need a .....?', '1. virtual circuit subnet  ', '2. short circuit subnet', '3. datagram subnet ','4. wireless subnet',3 ); " \
			 "INSERT INTO QUIZ (ID,INTREBARE,RASP1,RASP2,RASP3,RASP4,CORECT)" \
			 "VALUES (11, 'Which type of switching uses the entire capacity of a dedicated link?', '1. space division ', '2. time division', '3. virtual ','4. None of these',2 ); " \
			 "INSERT INTO QUIZ (ID,INTREBARE,RASP1,RASP2,RASP3,RASP4,CORECT)" \
			 "VALUES (12, 'In ......., each packet of a message need not follow the same path from sender to receiver.', '1. circuit switching', '2. message switching', '3. virtual approach to packet switching','4. datagram approach to packet switching',4 ); " \
			 "INSERT INTO QUIZ (ID,INTREBARE,RASP1,RASP2,RASP3,RASP4,CORECT)" \
			 "VALUES (13, 'A permanent virtual circuit involves ........', '1. Connection establishment ', '2. Data transfer', '3. Connection release','4. Connection check',2 ); " \
			 "INSERT INTO QUIZ (ID,INTREBARE,RASP1,RASP2,RASP3,RASP4,CORECT)" \
			 "VALUES (14, ' In Hierarchical routing for N router subnet, the optimal number of levels is ......', '1. logN  ', '2. log(N -1) ', '3. lnN  ','4. ln(N-1)',3 ); ";
			
			rc = sqlite3_exec(db, sql, callback_quiz, 0, &zErrMsg);
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			 sqlite3_free(zErrMsg);
		   }
		   sqlite3_close(db);
		 
		 
	   }
	   if(optiune==55)		//afisarea continutului tabelei
	   {  
		   sql="SELECT * FROM QUIZ";
		   
		   rc = sqlite3_exec(db, sql, callback_quiz, 0, &zErrMsg);
		   if( rc != SQLITE_OK ){
		   fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
		   }
		   sqlite3_close(db);
		 
	    }
	   if(optiune==1)		//selecteaza intrebarea si mai apoi raspunsurile de la id-ul id pt a le afisa
	   {
			sql = sqlite3_mprintf("SELECT INTREBARE FROM QUIZ WHERE ID=%d;",id);
		
			/* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		
			sql = sqlite3_mprintf("SELECT RASP1,RASP2,RASP3,RASP4 FROM QUIZ WHERE ID=%d;",id);
			
			/* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_raspunsuri, 0, &zErrMsg);
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
			
			sqlite3_close(db);
		} 
	   if(optiune==2) 		//selectam care din cele 4 variante de raspuns e corecta (din coloana corect)
	   {
			sql = sqlite3_mprintf("SELECT corect FROM QUIZ WHERE ID=%d;",id);
			
			/* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_corect, 0, &zErrMsg); //...si punem in variabila corect aceasta varianta
			if( rc != SQLITE_OK )
			{
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
			
			sqlite3_close(db);
	   }
	   if(optiune==3)		//selectam id-ul maxim (cate intrebari sunt) din QUIZ
	   {
			
		sql = "SELECT max(id) FROM QUIZ;"; 
			
		/* Execute SQL statement */
		rc = sqlite3_exec(db, sql, callback_max, 0, &zErrMsg);
		if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		  sqlite3_free(zErrMsg);
		}
		sqlite3_close(db);
   
	   }
	   if(optiune==5)		//selectam toata linia de la id-ul "id" si afisam
	   {
			
		sql = sqlite3_mprintf("select * from QUIZ where id='%i';",id);
			
		/* Execute SQL statement */
		rc = sqlite3_exec(db, sql, callback_afisare_quiz, 0, &zErrMsg);
		if( rc != SQLITE_OK ){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		  sqlite3_free(zErrMsg);
		}
		
		sqlite3_close(db);
   
	   }
	
}




//===============functii de callback pt baza de date USERI ===========
//====================================================================

static int callback_inreg(void *NotUsed, int argc, char **argv, char **azColName){ //DOAR PRINTEAZA REZULTATELE
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

static int callback_inreg2(void *NotUsed, int argc, char **argv, char **azColName){ //PUNE IN MAX_ID2 CATE REZULTATE SE OBTIN
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	  max_id2=atoi(argv[i]);
   }
   printf("\n");
   return 0;
}

static int callback_inreg3(void *NotUsed, int argc, char **argv, char **azColName){ //PUNE IN NEW_PAR PAROLA GASITA
   int i;
   bzero(new_par,20);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	  strcpy(new_par,argv[i]);
	  exista=1;
   }
   printf("\n");
   return 0;
}

static int callback_inreg5(void *NotUsed, int argc, char **argv, char **azColName){ //PUNE IN disponibili CATI UTILIZATORI CU LOGAT=1 SUNT
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	  disponibili=atoi(argv[i]);
   }
   printf("\n");
   return 0;
}

static int callback_inreg6(void *NotUsed, int argc, char **argv, char **azColName){// pune in a0 acel utilizator -VEZI 	OPTIUNEA 10 DE LA FUNCTIA INREGISTRARE
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	  ar0=atoi(argv[i]);
   }
   printf("\n");
   return 0;
}

static int callback_inreg8(void *NotUsed, int argc, char **argv, char **azColName){ // pune in id_nou id-ul userului cu numele 'user'
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	  id_nou=atoi(argv[i]);
   }
   printf("\n");
   return 0;
}

static int callback_inreg9(void *NotUsed, int argc, char **argv, char **azColName){ //pune in variabila "utilizator_logat" rezultatul query-ului corespunzator (valoare din campul LOGAT)
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	  utilizator_logat=atoi(argv[i]);
   }
   printf("\n");
   return 0;
}

static int callback_inreg10(void *NotUsed, int argc, char **argv, char **azColName){ //pune in variabila "este" rezultatul query-ului corespunzator (daca exista un user cu numele cautat)
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	  este=1;
   }
   printf("\n");
   return 0;
}

static int callback_inreg11(void *NotUsed, int argc, char **argv, char **azColName){ //pune in variabila "user_pt_joaca" rezultatul query-ului corespunzator (numele utilizatorului de la id-ul cautat)
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	  strcpy(user_pt_joaca,argv[i]);
   }
   printf("\n");
   return 0;
}




void inregistrare(int optiune,char user[20],char parola[20],int rez,int idt)
{
	
	   sqlite3 *db;
	   char *zErrMsg = 0;
	   int  rc,id;
	   char *sql;

	   
	   /* Open database */
	   rc=sqlite3_open("useri.db",&db);
	   if(rc)
	   {
		  fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		  return(0);
	   }
	   
	   sqlite3_busy_timeout(db, 500); 
	   
		if(optiune==0) //optiunea de creare a bazei de date user.db
		{
			sql = "CREATE TABLE USERI("  \
				 "ID              INT PRIMARY KEY     NOT NULL," \
				 "USER            CHAR(50)    NOT NULL," \
				 "PAROLA          CHAR(50)     NOT NULL,"\
				 "REZ             INT     NOT NULL,"\
				 "LOGAT           INT     NOT NULL," \
				 "JOACA           INT);" ;
				 
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg, 0, &zErrMsg);
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
		   }
		   
		   
			sql= "INSERT INTO USERI (ID,USER,PAROLA,REZ,LOGAT,JOACA) VALUES (1, 'irina', '123', 0,0,0 ); "; //inseram o prima persoana
			/* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg, 0, &zErrMsg);
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			
		   }
			
		}
		if(optiune==1) // inserez user and parola in tabela iar la restul campurilor adauga 0
		{
			sql= "select max(id) from USERI"; // cautam id-ul maxim al userilor si....
		
			/* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg2, 0, &zErrMsg);
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
	   
			id=max_id2+1; //...adaugam la pozitia urmatoare in tabela un nou utilizator
				
			sql= sqlite3_mprintf("INSERT INTO USERI (ID,USER,PAROLA,REZ,LOGAT,JOACA) VALUES (%i, '%q', '%q', 0,0,0 ); ",id, user,parola) ;

		   /* Execute SQL statement */
		   rc = sqlite3_exec(db, sql, callback_inreg, 0, &zErrMsg);
		   if( rc != SQLITE_OK ){
		   fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
		   }
		   
		}
		else if(optiune==2) //selecteaza parola de la un anumit id 
		{
			
			sql= sqlite3_mprintf("select parola from useri where user='%q'; ",user) ;
			
			 rc = sqlite3_exec(db, sql, callback_inreg3, 0, &zErrMsg);
			 if( rc != SQLITE_OK ){
			 fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			 }
			
		}
		else if(optiune==3) //selectam datele userilor(pt programator)
		{
			sql="select * from USERI";
			 /* Execute SQL statement */
			 rc = sqlite3_exec(db, sql, callback_inreg, 0, &zErrMsg); //dupa care le afisam
			 if( rc != SQLITE_OK ){
			 fprintf(stderr, "SQL error: %s\n", zErrMsg);
			 sqlite3_free(zErrMsg);
			}
		}
		else if(optiune==4) //facem update campului JOACA pe baza unui user cand acesta incepe sa se joace
		{
			sql = sqlite3_mprintf("update USERI set JOACA=1 where user='%q';",user);
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg, 0, &zErrMsg);  //dupa care  afisam
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==7) //seteaza LOGAT=1 atunci cand un utilizator se logheaza
		{
			sql = sqlite3_mprintf("update USERI set LOGAT=1 where user='%q';",user);
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg, 0, &zErrMsg); //dupa care  afisam
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==8) //selecteaza toate id-urile utilizatorilor logati
		{
			sql ="SELECT id from USERI where LOGAT=1;";
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg, 0, &zErrMsg); //dupa care  afisam
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==9) //selecteaza numarul utilizatorilor logati
		{
			sql ="SELECT COUNT(LOGAT) from USERI where LOGAT=1;";
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg5, 0, &zErrMsg); //pune in variabila disponibili acest numar
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		
		}
		else if (optiune==10) // selecteaza id-ul primului utilizator (dpdv al id-ului) care e logat dar inca nu se joaca
		{	
			sql =sqlite3_mprintf("SELECT id from USERI where LOGAT=1 and JOACA=0 LIMIT %i,1;",pp);
			pp++; //cu ajutorul variabilei pp se ia urmatorul client dupa pp (ca valoare de id)->lucreaza ca un prag dupa care se ia NUMAI urmatorul id
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg6, 0, &zErrMsg); //pune in a0 id-ul acestui utilizator
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==11) //face update utilizatorului cu numele 'user' daca s-a logat
		{
			sql =sqlite3_mprintf("update USERI set LOGAT=0 where user='%q';",user);
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg, 0, &zErrMsg); //doar afisam
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==13) //selectam id-ul iserului cu numele 'user'
		{
			sql =sqlite3_mprintf("SELECT id from USERI where user='%q';",user);
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg8, 0, &zErrMsg); // pune in id_nou acest id
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==14)  // face update la baza de date cu rezultatul jocului pentru utilizatorul cu numele 'user'
		{
			sql = sqlite3_mprintf("update USERI set REZ=%i where user='%q';",rez,user); 
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg, 0, &zErrMsg); //doar afiseaza 
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==15) //verifica valoarea din coloana LOGAT pentru utilizatorul cu numele 'user'
		{
			sql = sqlite3_mprintf("select logat from useri where user='%q';",user);
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg9, 0, &zErrMsg); // si o pune in variabila 'utilizator_logat'
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==16) //cauta in tabela USERI daca se gaseste in coloana USER vreun nume 'user'
		{
			sql = sqlite3_mprintf("select user from useri where user='%q';",user);
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg10, 0, &zErrMsg); //daca da, seteaza variabila este=1 daca se gaseste un astfel de nume in tabela
			if( rc != SQLITE_OK ){										// daca nu, este= 0 (ramane cum am definit-o global)
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==17) //facem update coloanei REZ (o facem 0) pentru utilizatorul cu numele 'user' 
		{
			sql = sqlite3_mprintf("update USERI set REZ=0 where user='%q';",user);
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg, 0, &zErrMsg); //doar afiseaza
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==18) //selecteaza numele utilizatorului de la id-ul 'idt'
		{
			sql = sqlite3_mprintf("select user from useri where id=%i;",idt);
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg11, 0, &zErrMsg); //...il punem in variabila user_pt_joaca
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==19) //facem update coloanei JOACA (o facem 0) pentru utilizatorul cu numele 'user' 
		{
			sql = sqlite3_mprintf("update USERI set JOACA=0 where user='%q';",user);
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg, 0, &zErrMsg); //doar afisam
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==20)  //selectam id-ul maxim al userilor
		{
			sql = "select max(id) from USERI";
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg2, 0, &zErrMsg);		//si pune in max_id2 acest rezultat
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==21) //facem update coloanei LOGAT (o facem 0) pentru utilizatorul cu id-ul 'idt' 
		{
			sql =sqlite3_mprintf("update USERI set LOGAT=0 where id='%i';",idt);
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg, 0, &zErrMsg); //doar afiseaza
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==22) //facem update coloanei JOACA (o facem 0) pentru utilizatorul cu id-ul 'idt' 
		{
			sql =sqlite3_mprintf("update USERI set JOACA=0 where id='%i';",idt);
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg, 0, &zErrMsg); //doar afiseaza
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==23) //facem update coloanei REZ (o facem 0) pentru utilizatorul cu id-ul 'idt' 
		{
			sql =sqlite3_mprintf("update USERI set REZ=0 where id='%i';",idt);
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg, 0, &zErrMsg); //doar afiseaza
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==24)  //facem update coloanei PAROLA pentru un anumit user (se da numele lui) PENTRU OPERATII ADMIN
		{
			sql = sqlite3_mprintf("update USERI set PAROLA='%q' where user='%q';",parola,user);
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_inreg, 0, &zErrMsg);
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}	
		}
		else if (optiune==25) //selectam toate informatiile din tabela USERI de la id-ul 'idt'
		{
			sql = sqlite3_mprintf("select * from USERI where id='%i';",idt);
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_afisare_quiz, 0, &zErrMsg); //...si le afisam
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
		else if (optiune==26) //selectam toate informatiile din tabela USERI de la utilizatorul 'user'
		{
			sql = sqlite3_mprintf("select * from USERI where user='%q';",user);
			 /* Execute SQL statement */
			rc = sqlite3_exec(db, sql, callback_afisare_quiz, 0, &zErrMsg); //...si afisam
			if( rc != SQLITE_OK ){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			  sqlite3_free(zErrMsg);
			}
		}
	

   sqlite3_close(db);
	
}


static void *quiz();
static void *grupare();
int nr_cl=0; //nr. clienti


int main (int argc, char *argv[])
{
  struct sockaddr_in server;	// structura folosita de server  	
  struct sockaddr_in from;
  void threadCreate(int);
  
	pthread_t fir_executie; //pentru jocul meu
	
	int j=1,i=0;

   /* crearea unui socket */
   if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("[server]Eroare la socket().\n");
      return errno;
    }
	
  /* utilizarea optiunii SO_REUSEADDR */
  int on=1;
  setsockopt(sd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
  
  /* pregatirea structurilor de date */
	bzero (&server, sizeof (server));
	bzero (&from, sizeof (from));
  /* umplem structura folosita de server */
  /* stabilirea familiei de socket-uri */
    server.sin_family = AF_INET;	
  /* acceptam orice adresa */
    server.sin_addr.s_addr = htonl (INADDR_ANY);
  /* utilizam un port utilizator */
    server.sin_port = htons (PORT);
  
  /* atasam socketul */
  if (bind (sd, (struct sockaddr *) &server, sizeof (struct sockaddr)) == -1)
    {
      perror ("[server]Eroare la bind().\n");
      return errno;
    }

  /* punem serverul sa asculte daca vin clienti sa se conecteze */
   if (listen (sd, 2) == -1)
    {
      perror ("[server]Eroare la listen().\n");
      return errno;
    }

	while(1) //bucla infinita cu ajutrul careia acceptam un numar nelimitat de clienti 
   {
	int client;
			
	struct sockaddr_in from; 
	  
	printf ("[thread]- %d - pornit...\n", (int) i);
	fflush(stdout);
	int length = sizeof (from);

	int rasp_corect,alegere_admin,flag_meniu=0;
	char intr1[800]=" ",rasp_1[800]=" ",rasp_2[800]=" ",rasp_3[800]=" ",rasp_4[800]=" ";
	char usr_del[20]=" ", par_par[20]=" ", usr_par[20]=" ";
	int flag_meniu_adm=0;
	
	
	
	pthread_mutex_lock(&mlock);
	if ((client = accept (sd, (struct sockaddr *) &from, &length)) < 0)
		{
		 perror ("[thread]Eroare la accept().\n");	  			
		}
	pthread_mutex_unlock(&mlock);
		

	
	threadsPool=(struct Thread*)realloc(threadsPool, j*sizeof(struct Thread)); //alocam blocuri de memorie ori de cate ori se conecteaza un cient
	j++;
	
	threadsPool[nr_cl].cl=client;    //punem in threadsPool[nr_cl].cl descriptorul returnat de accept
	threadsPool[nr_cl].idTh=i;     //threadsPool[nr_cl].idTh-id-ul threadului la care suntem
	
	//printf("threadsPool[%i].cl= %i \n",nr_cl,client);
	//printf("threadsPool[%i].idTh= %i \n",nr_cl,i);
	
	i++;
	flag_meniu=0;
	while(flag_meniu==0) //pentru a deservi clientului in a-si alege tipul : admin sau jucator normal
	{
		if(login(threadsPool[nr_cl].cl, threadsPool[nr_cl].idTh)==1) //daca este jucator normal
		{	
			nr_cl++; //incrementam numarul de clienti
			flag_meniu=1;
			srand(time(NULL)); //pentru modul random de a pune intrebarile
			pthread_create(&fir_executie, NULL, &grupare, NULL); //cream un thread pt gruparea clientilor cate 3 pt a juca
			
		}
	else	//daca e admin
	{		
		while(flag_meniu_adm==0){
		if (read (threadsPool[nr_cl].cl, &alegere_admin,sizeof(alegere_admin))<= 0) //vedem ce vrea utilizatorul-admin sa faca...
		{
			perror ("Eroare la read() de la client.\n");
			return errno; 
		}
		if(alegere_admin==1) // sa introduca o intrebare
		{
			
			if (read (threadsPool[nr_cl].cl, &intr1,sizeof(intr1))<= 0)
			{
				
				 perror ("Eroare la read() de la client.\n");
				 return errno;
				 
			}
			if (read (threadsPool[nr_cl].cl, &rasp_1,sizeof(rasp_1))<= 0)
			{
				 
				 perror ("Eroare la read() de la client.\n");
				 return errno;
				 
			}
			if (read (threadsPool[nr_cl].cl, &rasp_2,sizeof(rasp_2))<= 0)
			{
				
				 perror ("Eroare la read() de la client.\n");
				 return errno;
				 
			}
			if (read (threadsPool[nr_cl].cl, &rasp_3,sizeof(rasp_3))<= 0)
			{
				
				 perror ("Eroare la read() de la client.\n");
				 return errno;
				 
			}
			if (read (threadsPool[nr_cl].cl, &rasp_4,sizeof(rasp_4))<= 0)
			{
				 perror ("Eroare la read() de la client.\n");
				 return errno;
				 
			}
			if (read (threadsPool[nr_cl].cl, &rasp_corect,sizeof(rasp_corect))<= 0)
			{
				
				 perror ("Eroare la read() de la client.\n");
				 return errno;
				 
			}
			actiuni_admin(1,intr1,rasp_1,rasp_2,rasp_3,rasp_4,rasp_corect);  //o introduce in baza de date quiz.db
			
		} 
		if(alegere_admin==4) actiuni_admin(4," "," "," "," "," ",0); // sa stearga ultima intrebare adaugata
		if(alegere_admin==2) //sa stearga un client dupa nume
		{ 
			if (read (threadsPool[nr_cl].cl, &usr_del,sizeof(usr_del))<= 0)
				{
					 perror ("Eroare la read() de la client.\n");
					 return errno;
				}
			inregistrare(5,usr_del," ",0,0);
		}
		if(alegere_admin==3)  // sa schimbe parola unui utilizator
		{ 
			if (read (threadsPool[nr_cl].cl, &usr_par,sizeof(usr_par))<= 0) //citeste numee userului
				{
					
					 perror ("Eroare la read() de la client.\n");
					 return errno;
					 
				}
				
			if (read (threadsPool[nr_cl].cl, &par_par,sizeof(par_par))<= 0) //primeste parola care se vrea a fi noua parola
				{
					
					 perror ("Eroare la read() de la client.\n");
					 return errno;
					 
				}
			inregistrare(24,usr_par,par_par,0,0); //se face update la parola
		}
		if(alegere_admin==5)  // se afiseaza informatiile utilizatorului cu id-ul... 
		{
			bzero(infos,200);
			int id_usr;
			if (read (threadsPool[nr_cl].cl, &id_usr,sizeof(id_usr))<= 0) //se primeste id-ul userului
			{
				 perror ("Eroare la read() de la client.\n");
				 return errno;
			}
			inregistrare(25," ", " ", 0,id_usr); //selecteaza de la id-ul id_usr informatiile despre acel tilizator
				
			if (write (threadsPool[nr_cl].cl, &infos, sizeof(infos))<= 0) 
			{
				 perror ("[Thread]Eroare la write() catre client.\n");
				 return errno;
			}
					
		}
		if(alegere_admin==7)  // se afiseaza intrebarea cu id-ul....   
		{
			bzero(infos,200);
			int id_intr;
			
			if (read (threadsPool[nr_cl].cl, &id_intr,sizeof(id_intr))<= 0) //se primeste id-ul intrebarii
			{
				 perror ("Eroare la read() de la client.\n");
				 return errno;
			}
			
			functie(5,id_intr); 	// se pune in variabila infos informatiile din tabela quiz de la id-ul "id_intr"
			
			if (write (threadsPool[nr_cl].cl, &infos, sizeof(infos))<= 0)  //  trimitem infos (aceste informatii) la client
			{
			 perror ("[Thread]Eroare la write() catre client.\n");
			 return errno;
			}
				
		}
		if(alegere_admin==6)  //se afiseaza informatiile utilizatorului cu numele....
		{
			bzero(infos,200);
			char num_usr[100];
			if (read (threadsPool[nr_cl].cl, &num_usr,sizeof(num_usr))<= 0) //se citeste numele de la client
			{
				 perror ("Eroare la read() de la client.\n");
				 return errno;
			}
			inregistrare(26,num_usr, " ", 0,0); // se pune in variabila infos informatiile userului cu numele "num_usr" din tabela useri
			
			if (write (threadsPool[nr_cl].cl, &infos, sizeof(infos))<= 0)  //  trimitem infos (aceste informatii) la client
			{
			 perror ("[Thread]Eroare la write() catre client.\n");
			}
				
		}
		if(alegere_admin==8)  //back
		{
			printf("Inapoi la meniu principal...\n");
			flag_meniu_adm=1;
		}
		if(alegere_admin==9)	//deconectare
		{
			flag_meniu=1;
			flag_meniu_adm=1;
		}
	  }
		flag_meniu_adm=0;
	 }
   }
  }
};				 

int login(int cl,int idThread) 
{
	
	int op_start, parola_ok=0 , ut_existent=0 , logi=0 , flag_loop_start , sau ,flag_sau=0;
	char user[20]=" ", parola[20]=" ";
	bzero(user,20);
	bzero(parola,20);
	 
 while(flag_sau==0) //daca variabila "sau" nu este fie 0 fie 1 atunci citeste din nou de la client
 {
	//inregistrare(0," ", " ",0,1); am creat tabela, nu apela!!!
	if(read (cl, &sau,sizeof(sau))<= 0) //primesc de la client daca e admin sau jucator
	{
		 printf("[Thread %d]\n",idThread);
		 perror ("Eroare la read() de la client pt sau.\n");
		 return errno;
		 
	}
	if(sau==0) //daca e jucator
	{
		if(idThread==0) //de fiecare data cand deschidem serverul setam 0 coloanele REZ, JOACA si LOGAT pt toti clientii
		{
			int i;
			
			//functie(55,0); //afiseaza in server toate intrebarile din tabela 	QUIZ
			
			inregistrare(20," "," ",0,0); //selecteaza numarul maxim de utilizatori si in pune in max_id2
			
			for( i =1; i<=max_id2; i++) //pentru toti utilizatorii...
			{
				inregistrare(21," ", " ",0,i);  //updatam LOGAT=0
				inregistrare(22," ", " ",0,i);	//updatam JOACA=0
				inregistrare(23," ", " ",0,i);	//updatam REZ=0
			}
			inregistrare(3," ", " ",0,1);//afiseaza in server toti utilizatorii si datele lor
	}
		
		
		
		if (read (cl, &op_start,sizeof(op_start))<= 0) //primeste de la client daca sa efectueze rutina pt inregistrare(0) sau logare(1)
		{
			 perror ("Eroare la read() de la client pt op_start.\n");
			 return errno;
		}

		while(op_start==0) //cat timp inca mai vrem sa inregistram
		{
		 
			if (read (cl, &flag_loop_start,sizeof(flag_loop_start))<= 0) //ne intoarcem la meniul de start dupa inregistrare
			{
				 perror ("Eroare la read() de la client pt flag_loop_start.\n");
				 return errno;
			}

			if(flag_loop_start==1) //daca vrem sa ne logam
			{
				if (read (cl, &op_start,sizeof(op_start))<= 0) //primim optinea de start de la client 
				{
				 perror ("Eroare la read() de la client.\n");
				 return errno;
				}
			}
			
			if(op_start==0) //daca vrem sa ne inregistram
			{
			
				while(ut_existent==0) //cat timp nu exista deja utilizatorul pe care vrem sa-l introducem
				{
					if (read (cl, &user,sizeof(user))<= 0) //primim de la client numele de introdus
					{
					  perror ("Eroare la read() de la client.\n");
					  return errno;
					}
					
					if (read (cl, &parola,sizeof(parola))<= 0) //primim de la client parola aferenta de introdus
					{
					  perror ("Eroare la read() de la client.\n");
					  return errno;
					}
					
					exista=0;
					inregistrare(2,user,parola,0,1); //exista devine 1 daca e parcursa tabela si gasim user
					
					if (write (cl, &exista, sizeof(exista))<= 0) //daca exista deja, anuntam si cientul pt a cere din nou
					{
					 perror ("[Thread]Eroare la write() catre client.\n");
					 return errno;
					}
					
					if(exista==0) //daca nu exista 
					{
						inregistrare(1,user,parola,0,0); //inserez acel user si parola
						ut_existent=1; // acum, exista
					}
				} 
			
			}
		 
		
		}
		
		
		if(op_start==1) //daca am ales sa ma loghez pe un cont deja existent
		{
		
		
		//functie(0,5); //creez tabela--NU DECOMENTA
		//functie(88,0);  //o populez odata--NU DECOMENTA
		//functie(55,5);  //afisez toate intrebarile
		
			while(parola_ok==0) //cat timp parola nu e ok
			{ 
				while(logi==0) //cat timp inca incerc sa ma loghez
				{
					if (read (cl, &user,sizeof(user))<= 0) //primesc userul
					{
					  perror ("Eroare la read() de la client.\n");
					  return errno;
					}
					
					inregistrare(16,user," ",5,1);//seteaza este=1 in caz ca exista in tabela
					
					if(write (cl, &este, sizeof(este))<=0) //notific si clientul in cazul in care utilizatorul exista in tabela
					{
					 perror ("[Thread]Eroare la write() catre client.\n");
					 return errno;
					}
					
					if (este==1) // daca este in tabela
					{
					
						inregistrare(15,user," ",5,1); //verific daca utilizatorul cu numele 'user' e logat deja 
													   //daca e logat variabila utilizator_logat  se face 1 
													   //daca nu, ramane 0
						
						if(write (cl, &utilizator_logat, sizeof(utilizator_logat))<= 0) //notificam si clientul
						{
						 perror ("[Thread]Eroare la write() catre client.\n");
						 return errno;
						}
						
						if(utilizator_logat==1) //daca e deja logat introducem nume din nou
						{
							printf("E deja logat, introdu din nou.\n");
							logi=0;
						} else logi=1;
					
					}
				}
				
				este=0;
				
				if (read (cl, &parola,sizeof(parola))<= 0) //primim parola de la client
				{
				  perror ("Eroare la read() de la client.\n");
				  return errno;
				}
				
				inregistrare(2,user,parola,0,1); //pune in new_par parola introdusa de la tastatura de catre utilizator
				
				if(strcmp(new_par,parola)==0)  //daca parola de la client si parola noastra din tabela USERI coincid
				{
					parola_ok=1; //parola e ok....altfel cerem un nou nume si tot asa
					printf("Gasit\n");
				}
				
				if (write (cl, &parola_ok, sizeof(parola_ok))<= 0)  //notificam si clientul ca si parola e ok
				{
				 perror ("[Thread]Eroare la write() catre client.\n");
				 return errno;
				}
				
			}
		
		}
	
		inregistrare(13,user," ",0,0); // selectam id-ul userului cu numele 'user'
									   //  si il pune in variabila id_nou
										 
		threadsPool[nr_cl].id=id_nou;	//punem in threadsPool[nr_cl].id id-ul acestui utilizator din tabela

		
		inregistrare(7,user," ",0,0); //setam ca userul cu numele 'user' e logat (setam LOGAT=1)
		
		flag_sau=1; 
		return 1; //return 1 ptc tocmai s-a facut o logare deci in main se va incrementa numarul de clienti si se va pune intr-un thread.
	}
	else if(sau==1) // daca clientul e admin - in main merg pe ramura else.. 
		{
			flag_sau=1;
			return 0;
		}
	 }
}


static void *grupare()
{
	int i;
	pthread_t joc;
	camera *camera_jucatori; //aceasta va fi structura ("camera") in care se va tine jocul
	int array[3];
	
	while (1)
	{
		
		inregistrare(8," "," ",0,1); //selecteaza ID-urile utilizatorilor logati
		
		inregistrare(9," "," ",0,1); //verifica cati sunt logati si pune aceasta valoare in variabila "disponibili"
		
		
		if (disponibili % 3==0) //din 3 in 3 facem un joc
		{
			inregistrare(10," "," ",0,1); //punem in array[0]primul utilizator LOGAT dpdv al id-lui in tabela
			array[0] = ar0;
			inregistrare(10," "," ",0,1);  //punem in array[0] al doilea utilizator LOGAT dpdv al id-lui in tabela
			array[1] = ar0;
			inregistrare(10," "," ",0,1);  //punem in array[0] al treilea utilizator LOGAT dpdv al id-lui in tabela
			array[2] = ar0;
			pp=0;
			
			camera_jucatori = (struct camera*)malloc(sizeof(struct camera)); //alocam memorie pt structura camera
			
			int t=0; //cand se face 3 (se conecteaza cei 3 jucatori) se pun intr-un thread nu sa joace
			char user[20]=" ";
			for (i = m; i < nr_cl; i++)  //facem urmatoarele actiuni pt grupari de 3 clienti pana cand terminam clentii
			{
				if (threadsPool[i].id == array[0])
				{	
					inregistrare(18," "," ",0,threadsPool[i].id); //selecteaza numele utilizatorului de la threadsPool[i].id si il punem in variabila 'user_pt_joaca'
					inregistrare(4,user_pt_joaca," ",0,threadsPool[i].id); //updatam campul JOACA=1 pentru acest user
					if(i%3==0)camera_jucatori->cl1=threadsPool[i].cl;	//realizam transmiterea intrebarilor in ordinea conectarii
					if(i%3==1)camera_jucatori->cl2=threadsPool[i].cl;
					if(i%3==2)camera_jucatori->cl3=threadsPool[i].cl;
					t++; 
				}
				if (threadsPool[i].id == array[1]) //idem mai sus
				{	
					inregistrare(18," "," ",0,threadsPool[i].id); 
					inregistrare(4,user_pt_joaca," ",0,threadsPool[i].id);
					if(i%3==0)camera_jucatori->cl1=threadsPool[i].cl;
					if(i%3==1)camera_jucatori->cl2=threadsPool[i].cl;
					if(i%3==2)camera_jucatori->cl3=threadsPool[i].cl;
					t++; 
				}
				if (threadsPool[i].id == array[2]) //idem mai sus
				{	
					inregistrare(18," "," ",0,threadsPool[i].id);
					inregistrare(4,user_pt_joaca," ",0,threadsPool[i].id);
					if(i%3==0)camera_jucatori->cl1=threadsPool[i].cl;
					if(i%3==1)camera_jucatori->cl2=threadsPool[i].cl;
					if(i%3==2)camera_jucatori->cl3=threadsPool[i].cl;
					t++;
				}
				if (t == 3) //se va ajunge aici abia cand se vor fi conectat 3 clienti iar in 
				{			//camera_jucatori->cl1camera_jucatori->cl2 si camera_jucatori->cl3
							//voi avea descriptorii returnati de accept pt fiecare client din joc
					
					
					m=m+3; //serverul va fi pregatit sa faca acelasi mecanism si pt urmatorii 3 conectati si tot asa
					
					printf("Intra serverul in camera jocuri \n");
					
					pthread_create(&joc, NULL, &quiz, camera_jucatori);
					while(oku);
					
					oku=0;
					break;
				}
			}
		}
		sleep(10);

	}
	return(NULL);
}

static void *quiz(void *arg)
{
	struct camera aux;
	
	aux = *((struct camera*)arg); //aici am cei 3 descriptori pentru cei 3 clienti din sesiune
	
	int rasp1,rasp2,rasp3,flag1_nou=0,flag2_nou=0,flag3_nou=0;
	int flag_deconect1=0,flag_deconect2=0,flag_deconect3=0;
	int user1 = aux.cl1, user2 = aux.cl2,user3 = aux.cl3;
	
	char mesaj[256],user_nou_1[20]=" ",user_nou_2[20]=" ",user_nou_3[20]=" ";
	
	char intrebare_de_ses[2000]=" "; 
	char raspunsuri_de_ses[2000]=" ";
	int sqlite3_threadsafe();
	
	
	if(write (user1,"Acum ma joc", sizeof("Acum ma joc"))<= 0) //pt primul conectat
	{
		 perror ("[Thread]Eroare la write() catre client.\n"); 
		 return errno;
	}
	if(write (user2,"Acum ma joc2", sizeof("Acum ma joc2"))<= 0)//pt al doilea conectat
	{
		 perror ("[Thread]Eroare la write() catre client.\n"); 
		 return errno;
	}
	if(write (user3,"Acum ma joc3", sizeof("Acum ma joc3"))<= 0)//pt al treilea conectat
	{
		 perror ("[Thread]Eroare la write() catre client.\n"); 
		 return errno;
	}
	
	//-----------------------------------------------------
	printf ("O noua sesiune.\n");
	
	functie(3,1); //pt max_id din tabela quiz
	
	int q=1;
	if(read (user1, &rez,sizeof(rez))<= 0)
	{
	 perror ("Eroare la read() de la client.\n");
	 return errno;
	}

	if(read (user2, &rez1,sizeof(rez1))<= 0)
	{
	 perror ("Eroare la read() de la client.\n");
	 return errno;
	}
	
	if(read (user3, &rez2,sizeof(rez2))<= 0)
	{
	 perror ("Eroare la read() de la client.\n");
	 return errno;
	}
	
	int y=0,random[10],z=0,r,flag_ctrlc,flag_ctrlc2,flag_ctrlc3;
	
	while( y < nr_intrebari_de_pus ) // nr_intrebari_de_pus e definita cu define ca 3
	{
		random[y]=rand() % max_id + 1; //aleg randm id-ul intrebarilor pe care le pun
		printf("random [%i]=%i\n",y,random[y]);
		for(z=0;z<y&&z!=y;z++)
			if(random[z]==random[y]) //am grija sa nu se repete. daca se repeta aleg di nou pt pozitia aia
				y--;
		y++;
		} 	 	
	y=0;
	
	
	//in vectorul random[] am id-ul intrebarior care se vor livra in ordine
	
	
	if (read (user1, &flag_ctrlc,sizeof(flag_ctrlc))<= 0) //daca primul client apasa ctrl-c
		{
		 perror ("Eroare la read() de la client.\n");
		 return errno;
		}
	if (read (user2, &flag_ctrlc2,sizeof(flag_ctrlc2))<= 0) //daca al doilea client apasa ctrl-c
		{
		 perror ("Eroare la read() de la client.\n");
		 return errno;
		}
	if (read (user3, &flag_ctrlc3,sizeof(flag_ctrlc3))<= 0) //daca al treilea client apasa ctrl-c
		{
		 printf("[Thread ]\n");
		 perror ("Eroare la read() de la client.\n");
		
		 return errno;
		}
	 
	while(q<=nr_intrebari_de_pus) //punem cele 3 intrebari
	{

		functie(1,random[y]); //pune in variabila intrebare intrebarea cu id-ul random[y]
		y++;
		
		strcpy(intrebare_de_ses,intrebare);
		strcpy(raspunsuri_de_ses,raspunsuri);
		
		//------------primul client in ordinea conectarii
		
		
		if(flag_deconect1==0) //daca nu ne-am deconectat cu primul client
		{
			
			if(flag_ctrlc==0) //daca primul client nu a apasat ctrl-c
			{
				if (write (user1, &intrebare_de_ses, sizeof(intrebare_de_ses))<= 0) //trimitem intrebarea la client
				{
				 perror ("[Thread]Eroare la write() catre client.\n"); 
				 return errno;
				}
				
			}else flag_deconect1=1; //altfel, daca cineva a apasat ctrl-c, ne deconectam
				
			if(flag_ctrlc==0)	 //daca primul client nu a apasat ctrl-c
			if (write (user1, &raspunsuri_de_ses, sizeof(raspunsuri_de_ses))<= 0) //trimitem si raspunsurile
			{
			 perror ("[Thread]Eroare la write() catre client.\n");
			 return errno;
			}
		
			
				
			if (read (user1, &rasp1,sizeof(rasp1))<= 0) //primim raspunsul de la client
			{
			 perror ("Eroare la read() de la client.\n");
			 return errno;
			}
		
			if(rasp1==5) //daca clientul a raspuns cu 5 atunci ne deconectam (setam flag_deconect1=1 )
			{
				flag_deconect1=1;
				qui=nr_intrebari_de_pus+2; //ne ducem direct la final pt a afisa situatia castigatorilor
			}
			
			if(qui!=nr_intrebari_de_pus+2) //daca nu se deconecteaza
			if(read (user1, &user_nou_1,sizeof(user_nou_1))<=0) //primim de la client numele utilizatorului
			{
			 perror ("Eroare la read() de la client.\n");
			 return errno;
			}
			
			
			functie(2,q); //selectam care e raspunsul corect al intrebarii q
			
			if(corect==rasp1)
				rez=rez+1; //daca e corect adaugam un punct acelui client
			
			if(rasp1==5)
			{
				if(read (user1, &user_nou_1,sizeof(user_nou_1))<=0)
				{
				 perror ("Eroare la read() de la client.\n");
				 return errno;
				}
				//printf("user_nou_1 este: %s\n",user_nou_1);
				functie(2,q); //SELECtam raspunsul corect DE LA ID si il punem in variabila corect  
			
				if(corect==rasp1) //daca raspunsul primit e corect
					rez=rez+1; //adaugam un punct utilizatorului
				
				if(write (user1, &rez, sizeof(rez))<=0) //trimitem rezultatul final inapoi la client pt a fi afisat
				{
				 perror ("[Thread]Eroare la write() catre client.\n"); 
				 return errno;
				}
				flag1_nou=1; 
			}
		}
		
	
		inregistrare(14,user_nou_1," ",rez,1); //inregistram rezultatul in tabela
		
		sleep(5); //de comentat
		
		
		
		
		//------------al doilea client in ordinea conectarii
		// comentarii idem ca in cazul primului
		
		
		if(flag_deconect2==0)
		{
			if(flag_ctrlc2==0)
			{
				if (write (user2, &intrebare_de_ses, sizeof(intrebare_de_ses))<= 0) 
				{
				 perror ("[Thread]Eroare la write() catre client.\n");
				 return errno;
				}
			}else 
				flag_deconect2=1;
			
			if(flag_ctrlc2==0)
				if (write (user2, &raspunsuri_de_ses, sizeof(raspunsuri_de_ses))<= 0) 
				{
				 perror ("[Thread]Eroare la write() catre client.\n");
				 return errno;
				}
					
				if (read (user2, &rasp2,sizeof(rasp2))<= 0) 
				{
				 perror ("Eroare la read() de la client.\n");
				 return errno;
				}
					
				if(rasp2==5)
				{
					flag_deconect2=1; 
					qui1=5;
				}
				
				if(qui1!=5)	
				if (read (user2, &user_nou_2,sizeof(user_nou_2))<= 0) 
				{
				 perror ("Eroare la read() de la client.\n");
				 return errno;
				}
				
			
				functie(2,q); //SELECT CORECT DE LA ID
				
				if(corect==rasp2)
					rez1=rez1+1;
				
				if(rasp2==5)
				{
					if(read (user2, &user_nou_2,sizeof(user_nou_2))<= 0)
					{
					 perror ("Eroare la read() de la client.\n");
					 return errno;
					}
				
				
					functie(2,q); //SELECT CORECT DE LA ID
				
					if(corect==rasp2)
						rez1=rez1+1;
					
					if(write (user2, &rez1, sizeof(rez1))<=0)
					{
					 printf("[Thread ] ");
					 perror ("[Thread]Eroare la write() catre client.\n");
					}
						
				flag2_nou=1;
				} 
		
		} 
		
		
		inregistrare(14,user_nou_2," ",rez1,1);
		
		sleep(5); //de comentat
		
		
		//------------al treilea client in ordinea conectarii
		// comentarii idem ca in cazul primului
		
		if(flag_deconect3==0)
		{
			if(flag_ctrlc3==0)
			{
				if (write (user3, &intrebare_de_ses, sizeof(intrebare_de_ses))<= 0) 
				{
				 perror ("[Thread]Eroare la write() catre client.\n");
				 return errno;
				}
			}else flag_deconect3=1;
			
			if(flag_ctrlc3==0)
			if (write (user3, &raspunsuri_de_ses, sizeof(raspunsuri_de_ses))<= 0) 
				{
				 perror ("[Thread]Eroare la write() catre client.\n");
				 return errno;
				}
			
			if (read (user3, &rasp3,sizeof(rasp3))<= 0) 
				{ 
				 perror ("Eroare la read() de la client.\n");
				 return errno;
				}
				
			if(rasp3==5)
				{
					flag_deconect3=1;
					qui2=5;
				}
				
			if(qui2!=5)	
				
				if(read (user3, &user_nou_3,sizeof(user_nou_3))<=0)
				{
				 perror ("Eroare la read() de la client.\n");
				 return errno;
				}
			
			
			functie(2,q); //SELECT CORECT DE LA ID
			
			if(corect==rasp3)
			rez2=rez2+1;
		
			if(rasp3==5)
			{
				if(read (user3, &user_nou_3,sizeof(user_nou_3))<=0)
				{
				 perror ("Eroare la read() de la client.\n");
				 return errno;
				}
				
				functie(2,q); //SELECT CORECT DE LA ID
			
				if(corect==rasp3)
					rez2=rez2+1;
				
				if(write (user3, &rez2, sizeof(rez2))<=0)
				{
				 perror ("[Thread]Eroare la write() catre client.\n");
				 return errno;
				}
				
			flag3_nou=1;
			}
	
	}
		inregistrare(14,user_nou_3," ",rez2,1);
		
		q++; //urmatoarea intrebare
	}
	
	if(flag1_nou==0)
		if(write (user1, &rez, sizeof(rez))<=0)	//trimitem primului client rezultatul lor pt afisre corespunzatoare
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
	
	if(flag2_nou==0)
		if(write (user2, &rez1, sizeof(rez1))<=0)//trimitem celui de-al doilea client rezultatul lor pt afisre corespunzatoare
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
	
	if(flag3_nou==0)
		if(write (user3, &rez2, sizeof(rez2))<=0)//trimitem celui de-al treilea client rezultatul lor pt afisre corespunzatoare
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
	
	int steag=0; //in functie de valoaea lui transmitem cati din 3 au castigat 
	// pt steag=0 - a castigat numai un jucator
	// pt steag=1 - au castigat 2 jucatori
	// pt steag=2 - au castigat toti cei 3 jucatori(cu punctaje egale)
	
	//rez-rezultatul primului jucator
	//rez1-rezultatul celui de-al doilea jucator
	//rez2-rezultatul celui de-al treilea jucator
	
	
	
	if(rez>rez1 && rez>rez2) //castiga primul
	{
		int steag=0;
		 
		if(write(user1, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la if(write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		if(write(user3, &steag, sizeof(steag))<=0) 
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &user_nou_1, sizeof(user_nou_1))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &user_nou_1, sizeof(user_nou_1))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &user_nou_1, sizeof(user_nou_1))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &rez, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &rez, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &rez, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
	}
	if(rez1>rez && rez1>rez2) //castiga al doilea
	{
		int steag=0;
		 
		if(write(user1, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		  
		if(write(user1, &user_nou_2, sizeof(user_nou_2))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &user_nou_2, sizeof(user_nou_2))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &user_nou_2, sizeof(user_nou_2))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &rez1, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &rez1, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &rez1, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
	}
	if(rez2>rez && rez2>rez1) //castiga al treilea
	{
		int steag=0;
		 
		if(write(user1, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &user_nou_3, sizeof(user_nou_3))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &user_nou_3, sizeof(user_nou_3))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &user_nou_3, sizeof(user_nou_3))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &rez2, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &rez2, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &rez2, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
	}
	if(rez==rez1 && rez1>rez2 && rez>rez2) //castiga primii doi cnectati
	{
		int steag=1;
		 
		if(write(user1, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &user_nou_2, sizeof(user_nou_2))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &user_nou_2, sizeof(user_nou_2))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &user_nou_2, sizeof(user_nou_2))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &user_nou_1, sizeof(user_nou_1))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &user_nou_1, sizeof(user_nou_1))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &user_nou_1, sizeof(user_nou_1))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &rez, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &rez, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &rez, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
	}
	if(rez==rez2 && rez2>rez1 && rez>rez1) //castiga primul si ultimul conectat
	{
		int steag=1;
		 
		if(write(user1, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &user_nou_1, sizeof(user_nou_1))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &user_nou_1, sizeof(user_nou_1))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &user_nou_1, sizeof(user_nou_1))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &user_nou_3, sizeof(user_nou_3))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &user_nou_3, sizeof(user_nou_3))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &user_nou_3, sizeof(user_nou_3))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &rez2, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &rez2, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &rez2, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
	}
	if(rez1==rez2 && rez1>rez && rez2>rez) //castiga ultimii doi conectati
	{
		int steag=1;
		 
		if(write(user1, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		
		 
		if(write(user1, &user_nou_2, sizeof(user_nou_2))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &user_nou_2, sizeof(user_nou_2))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &user_nou_2, sizeof(user_nou_2))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &user_nou_3, sizeof(user_nou_3))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &user_nou_3, sizeof(user_nou_3))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &user_nou_3, sizeof(user_nou_3))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &rez2, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &rez2, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &rez2, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
	}
	if(rez1==rez2 && rez2==rez && rez1==rez) //cei 3 ies la egalitate
	{
		int steag=2;
		 
		if(write(user1, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &steag, sizeof(steag))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &user_nou_1, sizeof(user_nou_1))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &user_nou_1, sizeof(user_nou_1))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &user_nou_1, sizeof(user_nou_1))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &user_nou_2, sizeof(user_nou_2))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &user_nou_2, sizeof(user_nou_2))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &user_nou_2, sizeof(user_nou_2))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &user_nou_3, sizeof(user_nou_3))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &user_nou_3, sizeof(user_nou_3))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &user_nou_3, sizeof(user_nou_3))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user1, &rez2, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user2, &rez2, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
		 
		if(write(user3, &rez2, sizeof(rez))<=0)
		{
		 perror ("[Thread]Eroare la write() catre client.\n");
		 return errno;
		}
	}
		 
	
	inregistrare(11,user_nou_1," ",5,1); //delogam clientul cand a terminat de jucat (updatam logat cu 0 )
	
	inregistrare(11,user_nou_2," ",5,1);

	inregistrare(11,user_nou_3," ",5,1);
	
	inregistrare(19,user_nou_1," ",5,1); //punem in tabela ca nu mai joaca (updatam joaca cu 0)
	
	inregistrare(19,user_nou_2," ",5,1);

	inregistrare(19,user_nou_3," ",5,1);
	
	//inregistrare(3," ", " ",0,1); //afisam utilizatorii
	oku=1;
	
	
	return(NULL);
}

	

