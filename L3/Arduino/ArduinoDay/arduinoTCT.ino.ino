#include <UTFT.h>

extern uint8_t SmallFont[];

String GPUTemp = "15";
String GPUProc = "-15?";

String GPUMemUsed = "??";
String GPUMemTotal = "??";

String incomeData = "RESETT";

String printo = "";


String CPUTemp = "15";
String CPUCores = "-1";
String CPUProc = "-15?";


String RAMPerc = "??";
String RAMUsed = "??";
String RAMTotal = "??";


String gpuname = "NOTYET";

extern uint8_t SmallFont[];
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];

UTFT myGLCD(ITDB32WC,38,39,40,41);
void resetScreen(){

  myGLCD.clrScr();
}

int dataRec=0;

void setup()
{
  Serial.begin(9600);
  //randomSeed(analogRead(0));
  
// Setup the LCD
  myGLCD.InitLCD();
  myGLCD.InitLCD(LANDSCAPE);

  resetScreen();
  myGLCD.setColor(16, 167, 103);
  myGLCD.fillRect (0, 0, 479, 319);

  myGLCD.setColor(0, 43, 114);
  myGLCD.setBackColor(16, 167, 103);
  myGLCD.setFont(BigFont);
  
}

// x - dreapta y - jos



void loop()
{

//CPU
  while (Serial.available())                       
  {

    incomeData = Serial.readString();
    delay(1000);
    
    CPUTemp = incomeData.substring(0,3);
    if (CPUTemp[0]=='0')
    {
      CPUTemp = CPUTemp.substring(1,3);
    }
    incomeData.remove(0,3);
    
    CPUCores = incomeData.substring(0,2);
    if(CPUCores[0]!='0'&&CPUCores[0]!='1')
      {
        incomeData = '0'+incomeData;
        CPUCores = incomeData.substring(0,3);
      }
    if (CPUCores[0]=='0')
    {
      CPUCores = CPUCores.substring(1,2);
    }
    incomeData.remove(0,2);
    
    CPUProc= incomeData.substring(0,3);
    if(CPUProc[0]!='0'&&CPUProc[0]!='1')
      {
        incomeData = '0'+incomeData;
        CPUProc = incomeData.substring(0,3);
      }
    if (CPUProc[0]=='0'&&CPUProc[1]=='0'){
      CPUProc = CPUProc.substring(2,3);
    }
    else if (CPUProc[0]=='0')
    {
      CPUProc = CPUProc.substring(1,3);
    }
    incomeData.remove(0,4);


     RAMPerc = incomeData.substring(0,3);
    if(RAMPerc[0]!='0'&&RAMPerc[0]!='1')
      {
        incomeData = '0'+incomeData;
        RAMPerc = incomeData.substring(0,3);
      }
    if (RAMPerc[0]=='0'&&RAMPerc[1]=='0'){
      RAMPerc = RAMPerc.substring(2,3);
    }
    else if (RAMPerc[0]=='0')
    {
      RAMPerc = RAMPerc.substring(1,3);
    }
    incomeData.remove(0,4);

    


    RAMUsed= incomeData.substring(0,3);
    if (RAMUsed[0]=='0'&&RAMUsed[1]=='0'){
      RAMUsed = RAMUsed.substring(1,2);
    }
    else if (RAMUsed[0]=='0')
    {
      RAMUsed = RAMUsed.substring(0,2);
    }
    incomeData.remove(0,3);
    if(RAMUsed[0]=='0')
      RAMUsed = RAMUsed.substring(1,2);

    
    RAMTotal = incomeData.substring(0,3);
    RAMTotal='0'+RAMTotal;
    if(RAMTotal[0]!='0'&&RAMTotal[0]!='1')
      {
        incomeData = '0'+incomeData;
        RAMTotal = incomeData.substring(0,3);
      }
    if (RAMTotal[0]=='0'&&RAMTotal[1]=='0'){
      RAMTotal = RAMTotal.substring(2,3);
    }
    else if (RAMTotal[0]=='0')
    {
      RAMTotal = RAMTotal.substring(1,3);
    }
    incomeData.remove(0,2);


    
    
    GPUTemp = incomeData.substring(0,3);
    if(GPUTemp[0]!='0'&&GPUTemp[0]!='1')
      {
        incomeData = '0'+incomeData;
        GPUTemp = incomeData.substring(0,3);
      }
    if (GPUTemp[0]=='0'&&GPUTemp[1]=='0'){
      GPUTemp = GPUTemp.substring(2,3);
    }
    else if (GPUTemp[0]=='0')
    {
      GPUTemp = GPUTemp.substring(1,3);
    }
    incomeData.remove(0,4);


    GPUProc = incomeData.substring(0,3);
    if(GPUProc[0]!='0'&&GPUProc[0]!='1')
      {
        incomeData = '0'+incomeData;
        GPUProc = incomeData.substring(0,3);
      }
    if (GPUProc[0]=='0'&&GPUProc[1]=='0'){
      GPUProc = GPUProc.substring(2,3);
    }
    else if (GPUProc[0]=='0')
    {
      GPUProc = GPUProc.substring(1,3);
    }
    incomeData.remove(0,4);

    if(GPUProc.length()==0){
      GPUProc='0';
    }
    
    GPUMemUsed = incomeData.substring(0,3);
    incomeData.remove(0,3);
    
   GPUMemTotal= incomeData.substring(0,4);
    incomeData.remove(0,5);
    
   
  }
  printo = "CPU: "+CPUTemp+char(176)+"C";
  myGLCD.print(printo, LEFT, 2);

  printo = CPUCores+" cores";
  myGLCD.print(printo, CENTER, 2);

  printo = CPUProc+"%";
  myGLCD.print(printo, RIGHT, 2);
  

  
  printo = "GPU: "+GPUTemp+char(176)+"C";
  myGLCD.print(printo, LEFT, 22);


  printo = GPUProc+" %";
  myGLCD.print(printo, RIGHT, 22);

  printo = "RAM: "+RAMPerc+" %";
  myGLCD.print(printo, LEFT, 42);


  printo = RAMUsed+"/"+RAMTotal+"Gb";
  myGLCD.print(printo, CENTER, 42);
  


  printo = GPUMemUsed+"/"+GPUMemTotal+"Mb";
  myGLCD.print(printo, CENTER, 122);


  incomeData=incomeData;
  myGLCD.print(incomeData, CENTER, 82);
  
  
  incomeData = "";

}
