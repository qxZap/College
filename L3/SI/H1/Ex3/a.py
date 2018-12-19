import urllib2,sys
from Crypto.Cipher import AES
from base64 import b64decode
from base64 import b64encode
import socket
import time
baseURL  = 'http://127.0.0.1:5000/'
mode = 0

TCP_IP = '127.0.0.1'
TCP_PORT = 6636
BUFFER_SIZE = 1024
MESSAGE = 'ciorbaluisanducu'
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
connect_info = (TCP_IP,TCP_PORT)
if sys.argv[1] == 'A':
    if sys.argv[2] == 'ecb':
        mode = AES.MODE_ECB
    elif sys.argv[2] == 'cbc':
        mode = AES.MODE_CBC
    
key = ''
k3 = 'C\xdd\x92\x1d\xd6\x98P\xaeP?avh5G\xc9'
iv = '\xb2\xec.]\x9f\xf78bnn\xf2\x84\x96\x85gg'



def readFile():
    data = ''
    with open('data.txt', 'r') as myfile:
        data=myfile.read().replace('\n', '')
    return data

            
def encrypt(message):
    ctext = ''
    if mode == AES.MODE_ECB:
        obj = AES.new(key,mode)
        ctext = obj.encrypt(message)
    elif mode == AES.MODE_CBC:
        obj = AES.new(key,mode,iv)
        ctext = obj.encrypt(message)
    
    return ctext
    
def decrypt(ctext):
    text = ''
    if mode == AES.MODE_ECB:
        obj = AES.new(key,mode)
        text = obj.decrypt(ctext)
    elif mode == AES.MODE_CBC:
        obj = AES.new(key,mode,iv)
        text = obj.decrypt(ctext)
    
    return text

def fullEncrypt(message):
    messageList = [message[idx:idx+16] for idx,val in enumerate(message) if idx%16 == 0]
    for i in messageList:
        if mode == AES.MODE_ECB:
            obj = AES.new(key,mode)
            i = obj.encrypt(i)
        elif mode == AES.MODE_CBC:
            obj = AES.new(key,mode,iv)
            i = obj.encrypt(i)
    return ''.join(messageList)
    
def fullDecrypt(message):
    messageList = [message[idx:idx+16] for idx,val in enumerate(message) if idx%16 == 0]
    for i in messageList:
        if mode == AES.MODE_ECB:
            obj = AES.new(key,mode)
            i = obj.decrypt(i)
        elif mode == AES.MODE_CBC:
            obj = AES.new(key,mode,iv)
            i = obj.decrypt(i)
    return ''.join(messageList)
    
    
    
def getKey(URL,kind):
    key = ''
    URL+=kind
    contents = b64decode(urllib2.urlopen(URL).read())
    if mode == AES.MODE_ECB:
        obj = AES.new(k3,mode)
        key = obj.decrypt(contents)
    elif mode == AES.MODE_CBC:
        obj = AES.new(k3,mode,iv)
        key = obj.decrypt(contents)
    return key
        
       


if sys.argv[1]=='A':
    key = getKey(baseURL,sys.argv[2])
    s.connect(connect_info)
    s.send(sys.argv[2])
    data = s.recv(BUFFER_SIZE)
    
    
    if data != 'ALLGOOD':
        print 'error'
        sys.exit(0)
    print "got: ",data
    
    print fullDecrypt(fullEncrypt('tuicafacepedeste'))
    
    MESSAGE = readFile()
    while len(MESSAGE)%16!=0: MESSAGE+='+'
    MESSAGE = MESSAGE+len(MESSAGE)%16*'+'
    s.send(encrypt(MESSAGE))
    print 'sending ',encrypt(MESSAGE), ' || ',MESSAGE
     
    
    
    data = s.recv(BUFFER_SIZE)
    text = ''
    if mode == AES.MODE_ECB:
        obj = AES.new(key,mode)
        text = obj.decrypt(data)
    elif mode == AES.MODE_CBC:
        obj = AES.new(key,mode,iv)
        text = obj.decrypt(data)
    
    print "got: ",text
    
    
    s.close()
    
    
elif sys.argv[1]=='B': 
    s.bind(connect_info)
    s.listen(1)
    commKey = ''
    text = ''
    conn,addr = s.accept()
    finalString = ''
    while 1:
        data = conn.recv(BUFFER_SIZE)
        if not data: break
        
        if commKey == '':
            if data == 'ecb':
                mode = AES.MODE_ECB
            elif commKey == 'cbc':
                data = AES.MODE_CBC
            commKey = data
            key = getKey(baseURL,data)
            conn.send('ALLGOOD')
        else:
            if mode == AES.MODE_ECB:
                obj = AES.new(key,mode)
                text = obj.decrypt(data)
            elif mode == AES.MODE_CBC:
                obj = AES.new(key,mode,iv)
                text = obj.decrypt(data)
            finalString+=text
            
            conn.send(fullEncrypt("finalstringwords"))
        
    conn.close()
    while finalString[-1:]=='+': finalString = finalString[:-1]
    print finalString
    
    
    
    