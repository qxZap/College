
def swapTuples(tup1,tup2):
    swp=tup1
    tup1=tup2
    tup2=swp
    
    return tup1,tup2


def sortTuples(lista):
    worker = lista
    comparators = []
    for i in worker:
        comparators.append(i[1][2])
        
    for i in range(0,len(worker)):
        for j in range(0,len(worker)):
            if(j>i):
                if comparators[i]>comparators[j]:
                    worker[i],worker[j]=swapTuples(worker[i],worker[j])
    
    return worker
    
    
print(sortTuples( [('abc', 'bcd'), ('abc', 'zza')]))

