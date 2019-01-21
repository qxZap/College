import uuid

dic={    
    "+": lambda a, b: a + b,     
    "*": lambda a, b: a * b,
    "/": lambda a, b: a / b,
    "-": lambda a, b: a - b,
    "**": lambda a, b: a ** b
}

def sort_uuids(inputfile):
    fileOb=open(inputfile,"r")
    lines=fileOb.readlines()
    for i in lines:+
        i=i[:-1]
        val=''
        try:
            ID = '{'+i+'}'
            
            val=uuid.UUID(ID)
        except ValueError:
            print('err with ',i, 'NOT UUID')
        
     
def isNOTok(caracter):
    if caracter.isdigit() or caracter.isalpha():
        return False
    return True
     
     
def isuuid(uid):
    segments = uid.split('-')
    if len(uid)!=36 or uid.count('-')!=4 or len(segments[0])!=8 or len(segments[1])!=4 or len(segments[2])!=4 or len(segments[3])!=4 or len(segments[4])!=12:
        return False
    
    for i in uid.replace('-',''):
        if isNOTok(i):
            return False
    return True
    


def sort_uuids2(inputfile):
    fileOb=open(inputfile,"r")
    lines=fileOb.readlines()
    toSort=[]
    for i in lines:
        i=i[:-1]
        if isuuid(i):
            toSort.append(i)
        else:
            print('err with ',i,' NOTUUID')
         
            
        
     
     
def evalite(inputfile):
    fileOb=open(inputfile,"r")
    lines=fileOb.readlines()
    for i in lines:
        i=i[:-1]
        args=i.split(' ')
        a,b,operator=int(args[0]),int(args[1]),args[2]
        print(a,operator,b,' = ',dic[operator](a,b))
            
    
sort_uuids2("sample.txt")


evalite("input.txt")