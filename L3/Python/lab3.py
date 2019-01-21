def ex1(lista,listb):
    toReturn = []   
    intersection = []
    for i in lista:
        for j in listb:
            if i==j:
                intersection.append(i)
    intersection = list(set(intersection))
    toReturn.append(intersection)
        
        
    #unire
    unire = list(set(lista + listb))
    toReturn.append(unire)
    
    #a-b    
    a_b = []    
    for i in lista:
        if i not in listb:
            a_b.append(i)
            
    a_b = list(set(a_b))        
    toReturn.append(a_b)
       
    #b-a
    b_a = []
    for i in listb:
        if i not in lista:
            b_a.append(i)
            
    b_a = list(set(b_a))
    toReturn.append(b_a)
    
        
    return toReturn

def ex2(text):
    dict = {}
    for i in text:
        dict[i] = dict.get(i,0)+1
    return dict
    
    
def ex3(dica,dicb):
    cheile_comune_dar_cu_valori_diferite = []
    cheile_care_se_gasesc_doar_in_primul_dict = []
    cheile_care_se_gasesc_doar_in_al_doilea_dict = []
            
    for i in dica:
        if i not in dicb:
            cheile_care_se_gasesc_doar_in_primul_dict.append(i)
        if i in dicb and dica[i]!=dicb[i]:
            cheile_comune_dar_cu_valori_diferite.append(i)
    
    for i in dicb:  
        if i not in dica:
            cheile_care_se_gasesc_doar_in_al_doilea_dict.append(i)
            
    return [cheile_comune_dar_cu_valori_diferite,cheile_care_se_gasesc_doar_in_primul_dict,cheile_care_se_gasesc_doar_in_al_doilea_dict]
    
    
def ex4(tag,content,**arguments):
    toReturn = "<"+tag+" "
    
    for i in arguments:
        toReturn+=i+"=\\\""+arguments[i]+"\\\" "
    toReturn = toReturn[:-1]
    toReturn+=">"+content
    toReturn+="</"+tag+">"
    
    return toReturn
    
 
 
 
print(ex1(["mihai","marcel","florin"],["marcel","florin","ciocan","florin"]))


print(ex2("Ana are mere."))


print(ex3({'a':2,'b':3},{'b':2,'c':4}))

print(ex4("a", "Hello there", href="http://python.org", _class="my-link", id="someid"))
 
end = raw_input("")