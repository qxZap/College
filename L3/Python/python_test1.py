import os,sys


def problema1(n):
    sum = 0
    for i in range(1,n+1):
        sum+=i
    return sum

    
    
def problema2(a,b,n):
    if n==1:
        return a
    if n==2:
        return b
    if n>2:
        return problema2(a,b,n-2)+3*problema2(a,b,n-1)

def problema3(n,m):
    a=n
    b=m
    while(a>1):
        swp = b -3*a
        b=a
        a=swp
    return (a,b)
    
def problema5(n):
    base10 = int(n,3)
    return base10 == int(str(base10)[::-1])
    
def problema7(matrix):
    n = len(matrix)
    m  = len(matrix[0])
    string = ''
    for i in range(0,m):
        sum = 0
        for j in matrix[i]:
            sum+=int(j)
        string +=chr(sum)
        
    return string
