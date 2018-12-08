int string[6];
int poz=0;

int getChar(){
  if(poz==6)
    poz=0;
    
  return string[poz++];
}

void setup ()
{
string[0]=0b10001000;//A
string[1]=0b10000011;//b
string[2]=0b11000110;//C
string[3]=0b10100001;//d
string[4]=0b10000110;//E
string[5]=0b10001110;//F
  pinMode(4,OUTPUT); // pinul pentru latch (blocare)
  pinMode(7,OUTPUT); // pin pentru sincronizarea ceasului
  pinMode(8,OUTPUT); // pin pentru transmiterea datelor
}
 
void loop()
{
  afiseazaValoare(getChar(), 241); // scriem pe segmentul 241 (primul), valoarea 0.
  delay(5000);
//  afiseazaValoare(getChar(), 242);
//  delay(1000);
//  afiseazaValoare(getChar(), 244);
//  delay(1000);
//  afiseazaValoare(getChar(), 248); // tot valoarea 0 dar scrisa in binar
//  delay(1000);
}
 
void afiseazaValoare(byte valoare, byte segment)
{
  digitalWrite(4,LOW);
  shiftOut(8, 7, MSBFIRST, valoare);
  shiftOut(8, 7, MSBFIRST, segment);
  digitalWrite(4,HIGH);
}