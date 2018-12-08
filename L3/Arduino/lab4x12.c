// note redefinite de Stolniceanu Stefan dupa fisierul pitches.h (din Examples->Digital->toneMelody)
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440 // nota LA - interesant de citit si asta: http://dantanasescu.ro/2012/01/07/440-hz-frecventa-care-dezbina.html
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
/*
#define NOTE_D4  288
#define NOTE_E4  303
#define NOTE_F4  364
#define NOTE_G4  384
#define NOTE_A4  432 // nota LA - care respecta teoria conspiratiei de mai sus
#define NOTE_B4  456
#define NOTE_C5  512
#define NOTE_D5  576
*/



#define do  131
#define do_diez 139
#define re  147
#define re 156
#define mi  165
#define fa  175
#define fa_diez 185
#define sol  196
#define sol_diez 208
#define la  220
#define la_diez 233
#define si  247
 
// aici incepe cheia Sol (octava lui C4):
#define Do  262 
#define Do_diez 277
#define Re  294
#define Re_diez 311
#define Mi  330
#define Fa  349
#define Fa_diez 370
#define Sol  392 
#define Sol_diez 415
#define La  440
#define La_diez 466
#define Si  494
 
// aici incepe cheia sol cu o octava mai sus (octava lui C5)
#define DO  523
#define DO_diez 554
#define RE  587
#define RE_diez 622
#define MI  659
#define FA  698
#define FA_diez 740
#define SOL  784
#define SOL_diez 831
#define LA  880
#define LA_diez 932
#define SI  988

 
// vedeti mai mult in exemple -> Exemple -> Digital -> ToneMelody (are un pitches.h)
 
int buzzer_pin = 3;
int pauza_intre_note = 250;
int note[] = {
    do,mi,fa,sol,la,si,mi,mi,do,si,la,la,  re, do, si, si, mi, re, do, si, la, si, fa, sol, la, sol,fa, mi, fa, sol, la, si, la, sol, la, si, la, sol, fa, sol, mi,     mi,  mi, mi, mi, mi,          mi, re, re, mi, re, do,si, la, re, do, si, si, mi, re, do, si, la           
};
float timpi[]={
     0.25,0.75,0.125,0.25,0.25,0,0.25,0.25,0.75,0.125,0.25,0.25,     0.75,0.125,0.25,0.25,0.75,0.125,0.25,0.25,0.25,0.25,0,0.25,0.25,0.75,0.125,0.25,0.25,      0.75,0.125,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0,0,0,0.25,0.75,0.125,0.25,0.25,     0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25,0.25
     };

int lenSong = 63;
 
void setup() {

  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);
  
}

void afiseazaValoare(byte valoare, byte segment)
{
  digitalWrite(4,LOW);
  shiftOut(8, 7, MSBFIRST, valoare);
  shiftOut(8, 7, MSBFIRST, segment);
  digitalWrite(4,HIGH);
}
 
void loop() {
  for (int nota_curenta = 0; nota_curenta < lenSong+1; nota_curenta++) {
    //tone(buzzer_pin, note[nota_curenta], timpi[nota_curenta]*2000); // 3 = pinul cu buzzer-ul, nota, delay
    delay(timpi[nota_curenta]*1000);

    for(int i=10; i<14; i++)
    {
      float mediu = timpi[nota_curenta]*2000;
      int x = mediu;
      if(i-10 == millis()/x % 4)
         digitalWrite(i,LOW);
     else    
         digitalWrite(i,HIGH); 
    }

    
     if(timpi[nota_curenta]==0)
      noTone(buzzer_pin);
  }
   
  
  noTone(buzzer_pin);
  delay (5000);
}