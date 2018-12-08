import psutil
import platform,wmi,serial,time,sys,os
computer = wmi.WMI()
gpu_info = computer.Win32_VideoController()[0]


CPUtemp = 0
CPUproc = 0
CPUCores = psutil.cpu_count()
CPUfreq = 0

GPUName = str(gpu_info.Name.encode("utf-8")[7:])
GPUmemTotal = 0
GPUmemUsed = 0
GPUProc = 0

RAMperc = 0
RAMtotal = int(psutil.virtual_memory()[0]/1073741824)+1
RAMused = 0

OS = platform.system()
OSRelease = platform.release()

GPUtemp = 0
battery = 0

processes = 0

def getTemps():
    global GPUtemp,CPUtemp,GPUmemTotal,GPUmemUsed,GPUProc
    w = wmi.WMI(namespace="root\OpenHardwareMonitor")
    temperature_infos = w.Sensor()
    for sensor in temperature_infos:
        name = sensor.Name.encode("utf-8")
        value = sensor.Value
        if sensor.SensorType == 'Temperature':
           
            if name == 'GPU Core':
                GPUtemp = value
            elif name == 'Temperature':
                CPUtemp = value
        elif  sensor.SensorType == 'SmallData':
            value = int(value)
            if name == 'GPU Memory Total':
                GPUmemTotal = value
            elif name == 'GPU Memory Used':
                GPUmemUsed = value
        elif sensor.SensorType == 'Load':
            if name == 'GPU Core':
                GPUProc = sensor.Min/sensor.Max
            
def getStats():
    global CPUfreq,CPUproc,RAMperc,RAMtotal,RAMused,processes,battery

    CPUproc = psutil.cpu_percent()
    CPUfreq = psutil.cpu_freq()[0]
    
    getTemps()
    
    RAMperc = psutil.virtual_memory()[2]
    RAMused = int(psutil.virtual_memory()[3]/1073741824)
    
    
    processes = len(psutil.pids())
    
    
    battery = psutil.sensors_battery()[0]
    
   
    
getStats()
arduino = serial.Serial('COM6', 9600, timeout=.1)
time.sleep(1)
while True:
    toSend = ""
    if CPUtemp<100:
        toSend+='0'+str(int(CPUtemp))
    else:
        toSend+=str(int(CPUtemp))
    
    if CPUCores<10:
        toSend+='0'+str(int(CPUCores))
    else:
        toSend+=str(int(CPUCores))
    
    if CPUproc<100:
        if CPUproc<10:
            toSend+='00'+str(int(CPUproc))
        else:
            toSend+='0'+str(int(CPUproc))
    else:
        toSend+=str(int(CPUproc))
    
    if RAMperc<100:
        if RAMperc<10:
            toSend+='00'+str(int(RAMperc))
        else:
            toSend+='0'+str(int(RAMperc))
    else:
        toSend+=str(int(RAMperc)) 
      
    if RAMused<100:
        if RAMused<10:
            toSend+='00'+str(int(RAMused))
        else:
            toSend+='0'+str(int(RAMused))
    else:
        toSend+=str(int(RAMused))
        
    if RAMtotal<100:
        if RAMtotal<10:
            toSend+='00'+str(int(RAMtotal))
        else:
            toSend+='0'+str(int(RAMtotal))
    else:
        toSend+=str(int(RAMtotal))
       
    if GPUtemp<100:
        if GPUtemp<10:
            toSend+='00'+str(int(GPUtemp))
        else:
            toSend+='0'+str(int(GPUtemp))
    else:
        toSend+=str(int(GPUtemp))
        
        
        
    if GPUProc<100:
        if GPUProc<10:
            toSend+='00'+str(int(GPUProc))
        else:
            toSend+='0'+str(int(GPUProc))
    else:
        toSend+=str(int(GPUProc))
        
    toSend+='00'+str(int(GPUmemUsed)) 

    toSend+=str(int(GPUmemTotal))
    
    toSend+=' '+GPUName
    
    
    
    
    
        
    arduino.write(toSend)
    print(toSend)
    
    time.sleep(5)
    getStats()
    
        
            
    
    
print(GPUName,
GPUtemp,
str(GPUmemTotal)+" VMb",
str(GPUmemUsed)+" VMb",

str(RAMperc)+"%" ,
str(RAMtotal)+" Gb",
str(RAMused)+" Gb" ,

OS,
OSRelease,

CPUtemp,
battery ,
processes )