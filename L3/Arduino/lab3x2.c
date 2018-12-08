int string[256];
const int buton = A1;

int poz=0;
int fullString[55];
int text[4];

void push(){
  if(poz==52)
    poz=0;
  text[0]=fullString[poz];
  text[1]=fullString[poz+1];
  text[2]=fullString[poz+2];
  text[3]=fullString[poz+3];
  poz++;
}


int getChar(int i){
  return text[i];
}

void setup ()
{
  string['A']=0b10001000;//A
string['b']=0b10000011;//b
string['c']=0b11000110;//C
string['d']=0b10100001;//d
string['e']=0b10000110;//E
string['f']=0b10001110;//F
string['_']=0b11110111;//_
string['L']=0b11000111;//L
string['I']=0b11001111;//L
string['J']=0b11100001;//J
string['O']=0b11000000;//O
string['P']=0b10001100;//P
string['S']=0b10010010;//S
string['U']=0b11000001;//U


0b01110111;


fullString[0]=string['L'];
fullString[1]=string['U'];
fullString[2]=string['I'];
fullString[3]=string['_'];
fullString[4]=string['d'];
fullString[5]=string['E'];
fullString[6]=string['C'];
fullString[7]=string['E'];
fullString[8]=string['b'];
fullString[9]=string['A'];
fullString[10]=string['L'];
fullString[11]=string['_'];
fullString[12]=string['S'];
fullString[13]=string['I'];
fullString[14]=string['_'];
fullString[15]=string['F'];
fullString[16]=string['I'];
fullString[17]=string['U'];
fullString[18]=string['L'];
fullString[19]=string['U'];
fullString[20]=string['I'];
fullString[21]=string['_'];
fullString[22]=string['S'];
fullString[23]=string['A'];
fullString[24]=string['U'];
fullString[25]=string['_'];
fullString[26]=string['L'];
fullString[27]=string['E'];
fullString[28]=string['_'];
fullString[29]=string['P'];
fullString[30]=string['L'];
fullString[31]=string['A'];
fullString[32]=string['C'];
fullString[33]=string['E'];
fullString[34]=string['_'];
fullString[35]=string['S'];
fullString[36]=string['A'];
fullString[37]=string['_'];
fullString[38]=string['S'];
fullString[39]=string['E'];
fullString[40]=string['_'];
fullString[41]=string['J'];
fullString[42]=string['O'];
fullString[43]=string['A'];
fullString[44]=string['C'];
fullString[45]=string['E'];
fullString[46]=string['_'];
fullString[47]=string['C'];
fullString[48]=string['U'];
fullString[49]=string['_'];
fullString[50]=string['D'];
fullString[51]=string['A'];
fullString[52]=string['C'];
fullString[53]=string['I'];
fullString[54]=string['A'];

  pinMode(4,OUTPUT); // pinul pentru latch (blocare)
  pinMode(7,OUTPUT); // pin pentru sincronizarea ceasului
  pinMode(8,OUTPUT); // pin pentru transmiterea datelor
  
  pinMode(buton, INPUT);
  push();
}
 
void loop()
{
  if (!digitalRead(buton)) {
    push();
  }
  afiseazaValoare(getChar(0), 241); 
  delay(4);
  afiseazaValoare(getChar(1), 242); 
  delay(4);
  afiseazaValoare(getChar(2), 244); 
  delay(4);
  afiseazaValoare(getChar(3), 248); 
  delay(4);
  
}
 
void afiseazaValoare(byte valoare, byte segment)
{
  digitalWrite(4,LOW);
  shiftOut(8, 7, MSBFIRST, valoare);
  shiftOut(8, 7, MSBFIRST, segment);
  digitalWrite(4,HIGH);
}






int checkLogin(char givenUsername[22],char givenPassword[22])
{
    char username[22],password[22];
    FILE *fp;

    if((fp = fopen("users.txt", "r+")) == NULL) {
        printf("No such file\n");
        exit(1);
    }

    while (true) {
        int ret = fscanf(fp, "%s %s", username,password );
        if(ret == 2)
            if (strcmp(username, givenUsername) != 0)
                    if (strcmp(password, givenPassword) != 0)
                        return 1;
        else if(errno != 0) {
            perror("scanf:");
            break;
        } else if(ret == EOF) {
            break;
        } else {
            printf("No match.\n");
        }
        return 0;
    }
    
}


