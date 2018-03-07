freq = [
0.08167,
0.01492,
0.02782,
0.04253,
0.12702,
0.02228,
0.02015,
0.06094,
0.06966,
0.00153,
0.00772,
0.04025,
0.02406,
0.06749,
0.07507,
0.01929,
0.00095,
0.05987,
0.06327,
0.09056,
0.02758,
0.00978,
0.02360,
0.00150,
0.01974,
0.00074]



def aprox_equal(first_num,second_num):
        if int(first_num*(10**3))==int(second_num*(10**3)):
                return True
        return False

def MIC(freq_vector,second_text):
        sum=0
        for i in range(0,26):
             sum+= (freq_vector[i]/26) *(times_i_in_a(second_text,chr(i+97))/len(second_text))
        return sum

def len_of_key(plain_text):
        m=0
        while True:
                for i in range(0,m+1):
                   if aprox_equal(0.065,IC(n_to_n_from_j(plain_text,m,i))):
                           break
                m+=1
        return m

def times_i_in_a(plain_text,searched):
        counter=0
        for i in plain_text:
                if i==searched:
                        counter+=1
        return counter

def CharShift(plain_char,no_poz):
        return chr((ord(plain_char)-97+no_poz)%26+97)

def getKey(chiper_text,key_len):
        key_to_return=""
        for j in range(1,key_len+1):
                s=(-1)
                while True:
                        if aprox_equal(MIC(freq,CharShift(n_to_n_from_j(chiper_text,m,j),s)),0.065):
                                break
                key_to_return=key_to_return.join(("",(26-s)%26+96))
        return key_to_return




def n_to_n_from_j(plain_text,n,j):
        iterator=j
        string_to_return=""
        while iterator<len(plain_text):
                string_to_return=string_to_return.join(("",plain_text[iterator]))
                iterator+=n
        return string_to_return

def IC(plain_text):
        sum=0
        for i in range(0,26):
                sum+=(times_i_in_a(plain_text,chr(i+97))/len(plain_text))*( (times_i_in_a(plain_text,chr(i+97))-1)/(len(plain_text)-1))
        return sum

def VEncrypt(plain_text,en_key):
        crypt_text = ""
        for i in range(0, len(plain_text)):
                tmp_char = chr((ord(plain_text[i]) + ord(en_key[i % len(en_key)]) - 2 * ord('a')) % 26 + ord('a'))
                crypt_text = crypt_text.join(('', tmp_char))
        return crypt_text


def Vdecrypt(chiper_text, key):
        decrypted_text = ""
        for i in range(0, len(chiper_text)):
                tmp_char=  chr(( (ord(chiper_text[i])) - ord(key[i%len(key)])     )%26+97)
                decrypted_text=decrypted_text.join(("",tmp_char))
        return decrypted_text

def Shiting(plain_text,no_poz):
        no_poz=int(no_poz)*(-1)
        return plain_text[no_poz:]+plain_text[:no_poz]


#input_text=input("Text to encrypt=")
input_text="this is my dog"
#input_key=input("Encryption key=")
input_key="key"

input_text=input_text.replace(" ","")
print (VEncrypt(input_text,input_key))
print (Shiting(input_text,4))
print (times_i_in_a(input_text,'i'))
print (IC(input_text))
print (CharShift('a',3))
print (n_to_n_from_j(input_text,2,3))
print (aprox_equal(0.06565645,0.06524234))

print (Vdecrypt(VEncrypt(input_text,input_key),input_key))
print (len_of_key(VEncrypt(input_text,input_key)))


