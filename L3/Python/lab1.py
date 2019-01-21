
##Ex1
def cmmdc(a,b):
    while a!=b:
        if a>b:
            a-=b
        else:
            b-=a
    return a


print("cmmdc(12,15)=",cmmdc(12,15))

##Ex2
def vowels(str):
    counter = 0
    for i in str:
        if i in ["a","e","i","o","u","A","E","I","O","U",]:
            counter += 1
    return counter
    
quick_str = "ana are mere"
print(quick_str,vowels(quick_str))


#Ex3
def words(str):
    str = str.replace(","," ")
    str = str.replace(";"," ")
    str = str.replace("?"," ")
    str = str.replace("!"," ")
    str = str.replace("."," ")
    str = str.replace("  "," ")
   
    return len(str.split(" "))-1
quick_str = "ana are multe mere, cel putin asa cred! cred? da, cred."
print(quick_str,words(quick_str))


#Ex4
def occurrences(first_string,second_string):
    return second_string.count(first_string)
    
first_string = "aba"
second_string = "abbaababbaba"
print("occurrences(",first_string,",",second_string,")=",occurrences(first_string,second_string))


#Ex5
def specialChars(str):
    for i in str:
        if i in ["\r","\t","\n","\a","\b","\f","\v"]:
            return True
    return False

quick_str="da/nnu/tpoate\vnormal"
print("specialChars(",quick_str,")= ",specialChars(quick_str))


#Ex6
def UpperCamelCase(str):
    for i in str:
        if i.isupper():
            str = str.replace(i,"_"+i.lower())
    return str[1:]

quick_str = "AnaAreMereMulteRau"
print("UpperCamelCase(",quick_str,")= ",UpperCamelCase(quick_str))



def isPrime(n):
    if n % 2 == 0 and n > 2:
        return False
    return all(n % i for i in range(3, int(n**0.5) + 1, 2))
#Ex8
def biggestPrime(str):
    biggest_prime=0
    for i in str:
        if not i.isnumeric():
            str = str.replace(i, " ")
    list = str.split(" ")
    for i in list:
        if i.isnumeric():
            if int(i)>biggest_prime and isPrime(int(i)):
                biggest_prime = int(i)

    return biggest_prime

quick_str = "ahsfaisd35biaishai23isisvdshcbsi271cidsbfsd97sidsda"

print("biggestPrime(",quick_str,")= ",biggestPrime(quick_str))