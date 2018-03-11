#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <signal.h>
#include <poll.h>
#include <fcntl.h>

/* codul de eroare returnat de anumite apeluri */
extern int errno; 

/* portul de conectare la server*/
int port;

#define nr_intrebari_de_pus 3


#define FLUSH_STDIN(x) {if(x[strlen(x)-1]!='\n'){do fgets(Junk,16,stdin);while(Junk[strlen(Junk)-1]!='\n');}else x[strlen(x)-1]='\0';}
char Junk[16]; // buffer-ul pentru aruncarea imputului excesiv 
               // folosint pt "FLUSH_STDIN"




void  INThandler(int); //functie, bottom of the page

int flag_gata_raspunsul=0,flag_ctrlc=0,rasp,question=1;


int main (int argc, char *argv[])
{
	
	signal(SIGINT, INThandler); //daca clientul va apasa din acest punct ctrl-C, 
								//acesta functia signal va duce clientul in fuctia 
								//INThandler, unde SIGINT e semnalul pt ctrl-C
	 
	
  int sd;			// descriptorul de socket
  struct sockaddr_in server;	// structura folosita pentru conectare 

   int nr=0;
  char buf[10];

  /* exista toate argumentele in linia de comanda? */
  if (argc != 3) 
    {
      printf ("Sintaxa: %s <adresa_server> <port>\n", argv[0]);
      return -1;
    }

  /* stabilim portul */
  port = atoi (argv[2]);

  /* cream socketul */
  if ((sd = socket (AF_INET, SOCK_STREAM, 0)) == -1)
    {
      perror ("Eroare la socket().\n");
      return errno;
    }

  /* umplem structura folosita pentru realizarea conexiunii cu serverul */
  /* familia socket-ului */
  server.sin_family = AF_INET;
  /* adresa IP a serverului */
  server.sin_addr.s_addr = inet_addr(argv[1]);
  /* portul de conectare */
  server.sin_port = htons (port);
  
  /* ne conectam la server */
  if (connect (sd, (struct sockaddr *) &server,sizeof (struct sockaddr)) == -1)
    {
      perror ("[client]Eroare la connect().\n");
      return errno;
    }


	int optiune,max_id,op_start,sau,flag_meniu=0;
	char intrebare[2000]=" ",raspunsuri[2000]=" ",not1[50]=" ",not2[750]=" ";
	char user[20]=" ",parola[20]=" ",final[800]=" ";bzero(user,20);bzero(parola,20);
	int parola_ok=0,ut_existent=0,exista=0,ok3=0,ii,logi=0,rasp_corect,alegere_admin,par_admin;
	int utilizator_logat,este=0,flag_loop_start=0,op_valid=0;
	char usr_del[20]=" ", usr_par[20]=" ", par_par[20]=" ";
	int flag_meniu_adm=0,flag_afisare_meniu=0;
	
	
	system("clear");
	printf("===============================================================================\n");
	printf("========▓▓▓▓▓▓===▓====▓=▓+=▓▓▓▓▓▓=====▓▓▓▓▓▓?=== ▓▓===?▓===== =▓=▓▓▓▓▓▓▓=======\n");
	printf("======+▓=======▓?▓====▓=▓+====▓~=====+▓==+======▓~=▓==?▓▓=== +▓▓ ▓=============\n");
	printf("======+▓=======▓?▓====▓=▓+====▓~=====+▓==+======▓~=▓==?▓▓=== +▓▓ ▓=============\n");
	printf("======+▓=======▓?▓====▓=▓+===▓+======+▓===▓▓▓===▓▓▓▓==?▓~▓==?▓=▓=▓▓▓▓▓=========\n");
	printf("=======▓+===~▓▓~=▓====▓=▓+==▓:========▓I====▓==▓===~▓=?▓=~▓?▓==▓=▓=============\n");
	printf("========?▓▓▓▓▓▓▓=~▓▓▓▓~=▓+=▓▓▓▓▓▓▓==  ▓▓▓▓▓▓▓7=▓=== ▓ ?▓==~▓===▓=▓▓▓▓▓▓▓=======\n");  
	printf("===============================================================================\n");
	sleep(3);
	system("clear");
	printf("\n\n 		3..... \n");
	sleep(1);
	system("clear");
	printf("\n\n 		2.... \n");
	sleep(1);
	system("clear");
	printf("\n\n 		1.... \n");
	sleep(1);
	system("clear");
	
	
	while(flag_meniu==0) //sa ne intoarcem mereu la meniul principal in caz ca am terminat cu un client sau am dat disconnect din admin  
	{
		printf("\n\n                          Jucator sau admin? \n\n");
		printf("                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		printf("                              1. Admin \n");
		printf("                              0. Jucator \n");
		printf("                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		
		while (scanf("%d",&sau) != 1) //punem in variabila "sau" alegerea facuta (1 pentru admin, 0 pentru client)
			{	
				system("clear");
				printf("Nu ati introdus o optiune valida.\n");
				scanf("%*s");
			}
		system("clear");
			
		if(write(sd,&sau,sizeof(sau))<=0) // anuntam si serverul de optiunea facuta
		{
		  perror ("[client]Eroare la write() spre server la alegerea admin/client.\n");
		  return errno;
		}
		
		if(sau==1) //daca am ales sa facem operatiuni cu adminul
		{
			printf("Parola pentru admin?: ");
			
			scanf("%i",&par_admin); 
			
			if(par_admin==321) //parola aceasta va fi stiuta doar de admin
			{
				system("clear");
				
				while(flag_meniu_adm==0) //daca flag_meniu_adm==0 ne vom intoarce la meniul adminului 
				{						 //daca ne deconectam sau dam back flag_meniu_adm=1 si iesim din bucla
				
					if(flag_afisare_meniu==0) //afisam meniul numai odata si setam flag_afisare_meniu=0 din nou numai in cazul back-ului 
					{						  // sau deconnect-ului ptc ca urmatoarea data cand intram iar in admin sa afisam din nou	
						
						
						printf("                   Ce alegi sa faci?\n\n");
						printf("---------------------------------------------------------\n");
						printf("|1. Adauga intrebare.                                   |\n"); 
						printf("|2. Sterge clientul cu numele...                        |\n"); 
						printf("|3. Schimba parola client.                              |\n"); 
						printf("|4. Sterge ultima intrebare adaugata.                   |\n");
						printf("|5. Afiseaza informatiile utilizatorului cu id-ul...    |\n");
						printf("|6. Afiseaza informatiile utilizatorului cu numele....  |\n");
						printf("|7. Afiseaza intrebarea cu id-ul....                    |\n");
						printf("|8. Back                                                |\n");
						printf("|9. Disconnect                                          |\n");
						printf("---------------------------------------------------------\n\n");
						flag_afisare_meniu=1;
						
						
					}
					
					printf("Alege o optiune din cele de mai sus. Orice altceva va fi ignorat ^_^ .\n");
					while (scanf("%d",&alegere_admin) != 1) //alegem o varianta din cele prezentate in meniul de mai sus
					{
					printf("Nu ati introdus un numar valid.\n");
					scanf("%*s");
					}
					
					if(write(sd,&alegere_admin,sizeof(alegere_admin))<=0) //trimitem si la server
					{
					  perror ("[client]Eroare la write() spre server.\n");
					  return errno;
					}
					
					if(alegere_admin==1) //adaugare intrebare
					{
						printf("Care e intrebarea?\n");
						
						read(1, &not1, sizeof(not1)); //citim de la stdin intrebarea
						
						strcpy(final, not1);
						int len;
						len=strlen(final);
						final[len-1]='\0'; //taiem din final \n -ul ca sa arate frumos cand afisam intrebarea :D
						
						if(write(sd,&final,sizeof(final))<=0) //trimitem intrebarea la server pt a o introduce in baza de date
						{
						  perror ("[client]Eroare la write() spre server.\n");
						  return errno;
						}
						bzero(not1,50);
						
						
						printf("Care e prima varianta de raspuns?\n");
						
						read(1, &not1, sizeof(not1)); //citim de la stdin primul raspuns
						
						strcpy(final, "1. ");
						strcat(final, not1);
						len=strlen(final);
						final[len-1]='\0'; //taiem din final \n 
						
						
						if(write(sd,&final,sizeof(final))<=0)
						{
						  perror ("[client]Eroare la write() spre server.\n");
						  return errno;
						}
						bzero(not1,50);
						
						
						
						printf("Care e a doua varianta de raspuns?\n");  //idem ca mai sus
						
						read(1, &not1, sizeof(not1));
						strcpy(final, "2. ");
						strcat(final, not1);
						len=strlen(final);
						final[len-1]='\0';
						
						if(write(sd,&final,sizeof(final))<=0)
						{
						  perror ("[client]Eroare la write() spre server.\n");
						  return errno;
						}
						bzero(not1,50);
						
						
						
						printf("Care e a treia varianta de raspuns?\n"); //idem ca mai sus
						read(1, &not1, sizeof(not1));
						strcpy(final, "3. ");
						strcat(final, not1);
						len=strlen(final);
						final[len-1]='\0';
						
						if(write(sd,&final,sizeof(final))<=0)
						{
						  perror ("[client]Eroare la write() spre server.\n");
						  return errno;
						}
						bzero(not1,50);
						
						
						
						printf("Care e a patra varianta de raspuns?\n"); //idem ca mai sus
						read(1, &not1, sizeof(not1));
						strcpy(final, "4. ");
						strcat(final, not1);
						len=strlen(final);
						final[len-1]='\0';
						
						if(write(sd,&final,sizeof(final))<=0)
						{ 
						  perror ("[client]Eroare la write() spre server.\n");
						  return errno; 
						}
						
						bzero(not1,50);
						
						
						
						printf("Care e raspunsul corect?\n"); //idem ca mai sus
						//scanf("%d",&rasp_corect);
						while (scanf("%d",&rasp_corect) != 1) //bucla pt introducerea unui raspuns de tip int
						{
						printf("Nu ati introdus un numar valid.\n");
						scanf("%*s");
						}
						if(write(sd,&rasp_corect,sizeof(rasp_corect))<=0)
						{
						  perror ("[client]Eroare la write() spre server.\n");
						  return errno;
						}
						printf("---------------------------------------------------------\n");
					}
					if(alegere_admin==4) //sterge ultima intrebare
					{
						printf("Ati sters ultima intrebare cu succes! \n");
						printf("---------------------------------------------------------\n");
					}
					if(alegere_admin==2) //sterge client
					{
						printf("Introdu nume: \n");
						scanf("%s", &usr_del);
						if(write(sd,&usr_del,sizeof(usr_del))<=0) //trimitem numele userului de sters 
 						{
						  perror ("[client]Eroare la write() spre server.\n");
						  return errno;
						}
						printf("Ai sters cu succes \n");
						printf("---------------------------------------------------------\n");
					}
					if(alegere_admin==3) //schimba parola clinet
					{
						printf("Introdu numele carui utilizator vrei sa-i schimbi parola: \n");
						scanf("%s", &usr_par);
						if(write(sd,&usr_par,sizeof(usr_par))<=0)
						{
						  perror ("[client]Eroare la write() spre server.\n");
						  return errno;
						}
						printf("Ce parola noua sa fie? ");
						scanf("%s", &par_par);
						if(write(sd,&par_par,sizeof(par_par))<=0)
						{
						  perror ("[client]Eroare la write() spre server.\n");
						  return errno;
						}
						printf("Ai modificat cu succes \n");
						printf("---------------------------------------------------------\n");
					}
					
					if(alegere_admin==5) //afisare informatiile utilizatorului cu id-ul....
					{
						int id_usr;
						char infos[200];
						printf("Introdu id-ul utilizatorului: \n");
						while (scanf("%d",&id_usr) != 1) //bucla pt introducerea unui raspuns de tip int
						{
						printf("Nu ati introdus un numar valid.\n");
						scanf("%*s");
						}
						if(write(sd,&id_usr,sizeof(id_usr))<=0)
						{
						  perror ("[client]Eroare la write() spre server.\n");
						  return errno;
						}
						if (read(sd, &infos,sizeof(infos))< 0) 
						{
						  perror ("[client]Eroare la read() de la server.\n");
						  return errno;
						}
						printf("%s", infos); //afisam informatiile primite de la server
						printf("---------------------------------------------------------\n");

					}
					if(alegere_admin==6) //afisare informatiile utilizatorului cu numele....
					{
						char num_usr[100];
						char infos[200];
						printf("Introdu numele utilizatorului: \n");
						scanf("%s", &num_usr);
						if(write(sd,&num_usr,sizeof(num_usr))<=0)
						{
						  perror ("[client]Eroare la write() spre server.\n");
						  return errno;
						}
						if (read(sd, &infos,sizeof(infos))< 0) //citim informatiile de la server
						{
						  perror ("[client]Eroare la read() de la server.\n");
						  return errno;
						}
						printf("%s", infos);//le afisam
						printf("---------------------------------------------------------\n");
						
						
					}	
					if(alegere_admin==7) //afisam o intrebare dupa id
					{
						int id_intr;
						char infos[200];
						printf("Introdu id-ul intrebarii: \n");
						while (scanf("%d",&id_intr) != 1) //bucla pt introducerea unui raspuns de tip int
						{
						printf("Nu ati introdus un numar valid.\n");
						scanf("%*s");
						}
						if(write(sd,&id_intr,sizeof(id_intr))<=0)
						{
						  perror ("[client]Eroare la write() spre server.\n");
						  return errno;
						}
						if (read(sd, &infos,sizeof(infos))< 0) 
						{
						  perror ("[client]Eroare la read() de la server.\n");
						  return errno;
						}
						printf("%s", infos);
						printf("---------------------------------------------------------\n");
						
						
					}
					if(alegere_admin==8) //Back 
					{
						system("clear");
						printf("Inapoi la meniu principal...\n");
						flag_afisare_meniu=0;
						flag_meniu_adm=1;
					}
					if(alegere_admin==9) //deconnect
					{
						system("clear");
						flag_meniu=1;
						flag_afisare_meniu=0;
						flag_meniu_adm=1;
					}
				}
				
			}
			else printf("NU ESTI ADMIN! Inapoi la meniu...\n");
			flag_meniu_adm=0;
		}
		else if(sau==0)
		{
		while(op_valid==0)  //pt ca optiunea sa fie valida ea trebuie sa fie 0 sau 1
		{
			printf("\n\n                          Logare sau inregistrare? \n\n");
			printf("                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			printf("                              1. Logare \n");
			printf("                              0. Inregistrare\n");
			printf("                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			while (scanf("%d",&op_start) != 1) //bucla pt int
			{
				printf("Nu ati introdus un numar valid.\n");
				scanf("%*s");
			}
			
			system("clear");
			if(op_start==1||op_start==0) //daca am ales altceva inafara de logare sau inregistrare ne intoarcem inapoi
			{
				if(write(sd,&op_start,sizeof(op_start))<=0) //trimitem la server
				{
				  perror ("[client]Eroare la write() spre server.\n");
				  return errno;
				}
				op_valid=1; //daca optiunea e valida iesim din bucla
			}else 
			{
				printf("\n\n                            Introdu 1 sau 0!\n");
				sleep(2);
				system("clear");
			}
			
		}
		
		
	while(op_start==0) //inregistrare
	{
		
		if(write(sd,&flag_loop_start,sizeof(flag_loop_start))<=0)
		{
		  perror ("[client]Eroare la write() spre server.\n");
		  return errno;
		}
		
		if(flag_loop_start==1) //ne intoarcem la meniul de start dupa inregistrare
		{	
			printf("\n\n                          Logare sau inregistrare? \n\n");
			printf("                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			printf("                              1. Logare \n");
			printf("                              0. Inregistrare\n");
			printf("                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			//scanf("%d",&op_start);
			while (scanf("%d",&op_start) != 1) //bucla pt int
			{
			printf("Nu ati introdus un numar valid.\n");
			scanf("%*s");
			}
			
			if(write(sd,&op_start,sizeof(op_start))<=0)	//al 5lea
			{
			  perror ("[client]Eroare la write() spre server.\n");
			  return errno;
			}
		}
		
		if(op_start==0) //inregistrare
		{
				printf("                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
				printf("                                  1. Inregistrare \n");
				printf("                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
			
			while(ut_existent==0) //cat timp utilizatorul exista deja, nu iesim din bucla
			{
				
				
				printf("	User? ");
				scanf("%s",&user);
				if (write(sd,&user,sizeof(user))<= 0)
				{
				  perror ("[client]Eroare la write() spre server.\n");
				  return errno;
				}

				printf("	Parola? ");
				scanf("%s",&parola);
				if (write(sd,&parola,sizeof(parola))<= 0)  
				{
				  perror ("[client]Eroare la write() spre server.\n");
				  return errno;
				}

				
				if (read(sd, &exista,sizeof(exista))< 0) //citim de la server daca acesta exista sau nu
				{
				  perror ("[client]Eroare la read() de la server.\n");
				  return errno;
				}
				
				if(exista==0)
					ut_existent=1;
			}
		}
		
		flag_loop_start=1;
		
	}
		
		
		if(op_start==1) //logare
		{
			printf("                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			printf("                                  1. Autentificare \n");
			printf("                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
			while(parola_ok==0) //daca parola nu e buna pt utilizatorul dorit facem bucla pana se introduce o parola buna
			{
					
				while(logi==0) //bucla daca un anumit utilizator e logat sa caute altul
				{
					
					printf("	User? ");
					scanf("%s",&user);
					
					if (write(sd,&user,sizeof(user))<= 0) 
					{
					  perror ("[client]Eroare la write() spre server.\n");
					  return errno;
					}
					
					if(read(sd, &este,sizeof(este))<0) //serverul transmite daca utilizatorul e in baza de date(1) sau nu(0)
					{
					  perror ("[client]Eroare la read() de la server.\n");
					  return errno;
					}
					
					if(este==1)
					{
					
						if(read(sd, &utilizator_logat,sizeof(utilizator_logat))<0) //primeste daca utilizatorul e logat sau nu
						{
						  perror ("[client]Eroare la read() de la server.\n");
						  return errno;
						}
						
						if(utilizator_logat==1) //daca utilizatorul e logat 
						{
							printf("	E deja logat, introdu din nou.\n");
							logi=0;
						}
						else logi=1;
					}
				}
				
				//printf("	\n\n ");
				printf("	Parola? ");
				scanf("%s",&parola);
				if (write(sd,&parola,sizeof(parola))<=0) 
				{
				  perror ("[client]Eroare la write() spre server.\n");
				  return errno;
				}
				
				printf("\n\n	Acum asteptam jucatori......\n");
				
				if (read(sd, &parola_ok,sizeof(parola_ok))< 0) //daca parola e ok(=1) sau nu(=0)
				{
				  perror ("[client]Eroare la read() de la server.\n");
				  return errno;
				}
				
			}
			
			
			
		}
		
		
		char print[40]=" ",user_castigator[20]=" ",user_castigator1[20]=" ",user_castigator2[20]=" ",user_castigator3[20]=" ";
		bzero(print,40);
		
		
		if(read(sd, &print,sizeof(print))<0) //afiseaza "Acum ma joc 1/2/3"
		{
			 perror ("[client]Eroare la read() de la server.\n");
			 return errno;
		}
		sleep(2);
		system("clear");
		
		
		printf("%s\n",print);
		
		int rez=0,rez_castigator,flag_999=0,flag_deconect=0;
		char rasp2[10]=" ";
		
		struct pollfd fd;
		int ret;

		fd.fd = 0; // stdin-ul
		fd.events = POLLIN; //eventul este input-ul
		
		
		if(write(sd,&rez,sizeof(rez))<=0) //trimite rez=0 la server ca un jucator sa inceapa mereu de la 0
		{
		  perror ("[client]Eroare la write() spre server.\n");
		  return errno;
		}
		 
		if(write(sd,&flag_ctrlc,sizeof(flag_ctrlc))<=0) // se trimite serverului in caz ca utilizatorul a dat ctrl-c inainte de a incepe intebarile a sa nu-i dea niciuna
		{												//daca s-a apasat, se trimite 1 daca u se trimite 0 si jocul continua normal 
		  perror ("[client]Eroare la write() spre server.\n");
		  return errno;
		}
		if(flag_ctrlc==1) //daca cineva apasa ctrl-c se comporta ca o deconectare
		{
			question++; 
			if(write(sd,&rasp,sizeof(rasp))<=0) //va fi 5 (ca si cum ar fi apasat deconectare)
			{
			  perror ("[client]Eroare la write() spre server.\n");
			  return errno;
			}
		}
		
		
		while(question <= nr_intrebari_de_pus )    //punerea intrebarilor
		{
			
			if (read(sd, &intrebare,sizeof(intrebare))< 0) //citeste intebarea trimisa de server
			{
			  perror ("[client]Eroare la read() de la server.\n");
			  return errno;
			}
			
			
			if (read(sd, &raspunsuri,sizeof(raspunsuri))< 0) //citeste variantele de raspuns pentru intrebarea de mai sus
			{
			  perror ("[client]Eroare la read() de la server.\n");
			  return errno;
			}
			
			system("clear");
			printf("                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
			printf("                                  Intrebarea nr. %i \n", question);
			printf("                           ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
			printf("\n\n%s\n",intrebare);//se afiseaza
			printf("%s\n ",raspunsuri);
			printf("Raspunsul este(1,2,3 sau 4) (5 pt deconectare): \n");
			
			
			//se pune in variabila gunoi altceva decat primul int introdus de la tastaura
			char gunoi;
			int flags = fcntl(STDIN_FILENO, F_GETFL, 0); 
			fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
			while(read( STDIN_FILENO, &gunoi, sizeof(gunoi))!=-1 );
			fcntl(STDIN_FILENO, F_SETFL, flags);
			
			//STDIN_FILENO - default standard input file descriptor number which is 0
			//F_GETFL - specifica pt fcntl faptul ca ar trebui returnate flagurile asociate cu STDIN_FILENO
			//F_SETFL - seteaza descriptorii de fisiere asociati cu STDIN_FILENO. pentru asta e nevoie de campul flags
			
			
	
			
			ret = poll(&fd, 1, 5000); // 5 secunde pt input (sa se intample evenimentul fd.events = POLLIN ) 
			while(flag_gata_raspunsul==0) //daca flag_gata_raspunsul=0 inseamna ca inca se mai poate raspunde
			{								// flag_gata_raspunsul devine 1 fie atunci cand exista un raspuns de trimis serverului
				switch (ret)				//fie ca ai raspuns corect sau gresit (sau nu ai raspuns ceea ce inseamna ca ai gresit)
				{
					case -1: //eroare la poll
						break;
					case 0: //am iesit din timp
						if(flag_ctrlc==0)
						{
							flag_gata_raspunsul=1;
							printf("Ai iesit din timp. Ai 0 puncte pt intrebarea asta\n\n");
							flag_999=1; //trimite un raspuns gresit la server astfel incat sa nu se potriveasca cu varianta corecta 
						}				//nu se primeste niciun punct
						break;
					default:
						if(flag_ctrlc==0) //daca clientul n-a apasat ctrl-c ....
						{
							if(read(0,&rasp2,sizeof(rasp2))<0) //..se citeste de la input raspunsul dat de jucator
							{
							  perror ("[client]Eroare la read() de la server.\n");
							  return errno;
							}
							rasp=atoi(rasp2);
							printf("Ai raspuns  %i. Orice mai scrii e igonrat.\n",rasp);
							if(rasp==1||rasp==2||rasp==3||rasp==4) flag_gata_raspunsul=1;
								else if(rasp==5) //clientul a apasat deconectare
								{
									
									question= nr_intrebari_de_pus + 2; // ca sa ies din bula mare ((while<=nr_intrebari_de_pus)) si acel utilizator sa nu mai primeasca intrebari
									flag_gata_raspunsul=1;
									flag_deconect=1;
								}
							else
							{ printf("Introdu 1,2,3,4! \n");
							  printf("Raspunsul este(1,2,3 sau 4) (5 pt deconectare): ");
							}
						}
						break;
				}
				
			}
			
			flag_gata_raspunsul=0; // sa pot raspunde la urmatoarea intrebare
			
			if(flag_999==1) // daca s-a iesit din timp la intrebarea curenta
			{
				rasp=999;
				if(write(sd,&rasp,sizeof(rasp))<=0) //trimitem un raspuns gresit la server (fie 999)
				{
				  perror ("[client]Eroare la write() spre server.\n");
				  return errno;
				}
			}else if (write(sd,&rasp,sizeof(rasp))<= 0) //daca nu s-a iest din timp trimitem raspunsul care s-a dat la server
					{
					  perror ("[client]Eroare la write() spre server.\n");
					  return errno;
					}
			
			flag_999=0;	

		
			if(question!= nr_intrebari_de_pus + 2) //daca nu deconectam - sincronizare
				if(write(sd,&user,sizeof(user))<= 0) 
				{
				  perror ("[client]Eroare la write() spre server.\n");
				  return errno;
				}
			question++;

		}
		
		
		if(question== (nr_intrebari_de_pus + 3))		//daca se deconecteaza clientul
			if(write(sd,&user,sizeof(user))<= 0) 
			{
			  perror ("[client]Eroare la write() spre server.\n");
			  return errno;
			}
		
		if(read(sd, &rez,sizeof(rez))<0) //primim cat a facut pana in final 
			{
			  perror ("[client]Eroare la read() de la server.\n");
			  return errno;
			}
			
			
			
		//-----aici afisam situatia finala pentru fiecare utilizator in parte------	
			
			system("clear");
			
			printf("\n\n");
		if(rez==1) 
			printf("		%s ! Ai raspuns la o intrebare corect !\n",user);
		else if(rez==0)
			printf("		Mai %s! N-ai raspuns la nicio intrebare corect...\n",user);
		else 
			printf("		%s ! Ai raspuns la %i intrebari corect !\n",user,rez);
		
		
		int stegusor;
		if(read(sd, &stegusor,sizeof(stegusor))<0) //vedem raportul de castigatori/pierzatori
			{									   
			  perror ("[client]Eroare la read() de la server.\n");
			  return errno;						  
			}									 
			
		if(stegusor==0) //stegusor=0 - un castigator
		{   
			if(read(sd, &user_castigator,sizeof(user_castigator))<0)
			{
			  perror ("[client]Eroare la read() de la server.\n");
			  return errno;
			}
			
			if(read(sd, &rez_castigator,sizeof(rez_castigator))<0)
			{
			  perror ("[client]Eroare la read() de la server.\n");
			  return errno;
			}
				
			printf("\n\n\n 		%s a castigat cu %i intrebari raspunse corect !\n",user_castigator,rez_castigator);}
		else if(stegusor==1)  //stegusor=1 - 2 castigatori
		{
			if(read(sd, &user_castigator1,sizeof(user_castigator1))<0)
			{
			  perror ("[client]Eroare la read() de la server.\n");
			  return errno;
			}
			if(read(sd, &user_castigator2,sizeof(user_castigator2))<0)
			{
			  perror ("[client]Eroare la read() de la server.\n");
			  return errno;
			}
			if(read(sd, &rez_castigator,sizeof(rez_castigator))<0)
			{
			  perror ("[client]Eroare la read() de la server.\n");
			  return errno;
			}
			printf("\n\n\n 		%s si %s au castigat cu %i intrebari raspunse corect !\n",user_castigator1,user_castigator2 ,rez_castigator);
		}
		else if(stegusor=2)   //stegusor=2 - toti 3 castigatori
		{
			if(read(sd, &user_castigator1,sizeof(user_castigator1))<0)
			{
			  perror ("[client]Eroare la read() de la server.\n");
			  return errno;
			}
			if(read(sd, &user_castigator2,sizeof(user_castigator2))<0)
			{
			  perror ("[client]Eroare la read() de la server.\n");
			  return errno;
			}
			if(read(sd, &user_castigator3,sizeof(user_castigator3))<0)
			{
			  perror ("[client]Eroare la read() de la server.\n");
			  return errno;
			}
			if(read(sd, &rez_castigator,sizeof(rez_castigator))<0)
			{
			  perror ("[client]Eroare la read() de la server.\n");
			  return errno;
			}
			printf("\n\n\n 		%s si %s si %s au castigat impreuna cu %i intrebari raspunse corect !\n",user_castigator1,user_castigator2, user_castigator3,rez_castigator);
				
		}
		
		
		flag_meniu=1;
		char bla;
		int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
		fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
		while(read( STDIN_FILENO, &bla, sizeof(bla))!=-1 );
		fcntl(STDIN_FILENO, F_SETFL, flags);
		//idem mai sus
		}
	}
	
  /* inchidem conexiunea, am terminat */
  close (sd);
}

void  INThandler(int sig)
{ 
     char  c;

     signal(sig, SIG_IGN); //primeste signalul (s-a apasat ctrl-c)
     printf(" -> Ati apasat ctrl-c, rasp se va transforma in 5 si veti fi deconectat.\n");
     flag_gata_raspunsul=1;
	 flag_ctrlc=1; // o sa facem flag_ctrlc la else-uri ca sa putem da write de oriunde
	 rasp=5;
	 question= nr_intrebari_de_pus + 2;
	
     signal(SIGINT, INThandler); //  cand iese din functie se continua programul in mod normal
									// dar cu modificarile facute 
}

