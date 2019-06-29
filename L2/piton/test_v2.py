import re
from urllib.request import urlopen
import os
import hashlib
import zipfile
import socket

def problema1( s ):
	

    s=re.sub('[^a-zA-Z]+', ' ', s)
    s=s.split(' ')
    s.sort(key=lambda x:x[::-1])
    print(s)
    return s

    
def problema2():
    print("prob2")

def problema3(path):
    f = {}
    for (dirpath, dirnames, filenames) in os.walk(path):
        dir=os.path.basename(os.path.normpath(dirpath))
        dir=hashlib.md5(dir.encode()).hexdigest()
        filenames={filename:dir for filename in filenames}
        print(filenames)
        f={**f, **filenames}
    return f
    
    
    

def problema4(director,arhiva):
    arhiva=zipfile.ZipFile(arhiva)
    d=[]
    for (dirpath, dirnames, filenames) in os.walk(director):
        d.extend(filenames)
    a=set(arhiva.namelist())
    d=set(d)
    return list(d|a)

def problema5():
    print("prob5")
