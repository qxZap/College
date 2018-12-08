#include "SD.h"
#define SD_ChipSelectPin 4
#include "TMRpcm.h"
#include "SPI.h"
TMRpcm tmrpcm;
String incomingData = "";
char command;
String songs[50];
int numSongs = 0;
File root;
void play()
{
  tmrpcm.speakerPin = 9;
  tmrpcm.setVolume(4);
  tmrpcm.play("test.wav");  
}
void stop()
{
  tmrpcm.stopPlayback(); 
}
void SearchSongs(File dir, int numTabs) {
   while(true) {    
     File entry =  dir.openNextFile();
     if (! entry) {
       break;
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       SearchSongs(entry, numTabs+1);
     } else {
       String testStr=entry.name();

       songs[numSongs]=entry.name();
          numSongs+=1;
          
        testStr.substring(strlen(entry.name())-3);
        if(testStr == "wav")
         {
          songs[numSongs]=entry.name();
          numSongs+=1;
         }
     }
     entry.close();
   }
}
void make(){
  root = SD.open("/");
  numSongs = 0;
  SearchSongs(root, 0);
}
void setup()
{
  root = SD.open("/");
pinMode(LED_BUILTIN, OUTPUT);
tmrpcm.speakerPin=9;
Serial.begin(9600);
if(!SD.begin(SD_ChipSelectPin))
{
  return;
}
tmrpcm.setVolume(6);
tmrpcm.play("test.wav");

}
void loop() {
  if (Serial.available())
  {
      char command = Serial.read();
      Serial.print(command);
      
      if (command == 'p')
      {
        play();
      }

      if (command == 's')
      {
        stop();
      }

      if (command == 'm'){
        make();
        Serial.print(numSongs);
      }
  }
}