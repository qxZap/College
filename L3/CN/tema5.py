import random as rd
import numpy as np

epsilon = 10**-9

files=["m_rar_sim_2019_500.txt","m_rar_sim_2019_1000.txt","m_rar_sim_2019_1500.txt","m_rar_sim_2019_2019.txt"]

def createRareMatrix(filename):
    file = open(filename,'r')
    sizeN = int(file.readline())
    matrix = [dict() for i in range(sizeN)]
    file.readline()
    vecLinie = file.readline().split(',')
    while len(vecLinie) == 3:
        nr = float(vecLinie[0])
        line = int(vecLinie[1])
        collumn = int(vecLinie[2])
        matrix[line][collumn] = nr
        vecLinie = file.readline().split(',')
    file.close()
    return matrix, sizeN

def createRareMatrixSimetric(sizeN,k):
    matrix = [dict() for i in range(sizeN)]
    for line in range(sizeN):
        nr = rd.randint(0,12-len(matrix[line]))
        for j in range(nr):
            collumn = rd.randint(0,sizeN-1)
            matrix[line][collumn] = rd.randint(1,k)
            matrix[collumn][line] = matrix[line][collumn]
    return matrix

# matrix vimes
def timesVectorMatrix(matrix, vector, sizeN):
    toReturn = np.zeros(sizeN)
    for line in range(sizeN):
        for collumn in sorted(matrix[line].keys()):
            toReturn[line] += matrix[line][collumn] * vector[collumn]
    return toReturn




def doDashOperation(omega,matrix,vector,sizeN,k):
    return 1/np.linalg.norm(omega) * omega,timesVectorMatrix(matrix, vector, sizeN),np.dot(omega, vector),k+1

def powMethod(matrix, sizeN):
    kmax = 10**6
    vector =[]
    for i in range(sizeN):
        vector += [rd.random()]
    norm = np.linalg.norm(vector)
    if (norm + epsilon) > 1 or (norm - epsilon) < 1:
        vector = [i/norm for i in vector]
    vector = np.array(vector)
    omega = timesVectorMatrix(matrix, vector, sizeN)
    toReturn = np.dot(omega, vector)
    k = 0
    while np.linalg.norm(omega - toReturn * vector) > sizeN*epsilon and k <= kmax:
        vector,omega,toReturn,k = doDashOperation(omega,matrix,vector,sizeN,k)
    else:
        vector,omega,toReturn,k = doDashOperation(omega,matrix,vector,sizeN,k)
    return toReturn, vector




def SVDMethod(matrix, vector, sizeN):
    (U,S,V) = np.linalg.svd(matrix)
    p = len(matrix[0])
    m = sizeN
    if m > p:
        m = p
    Si = np.zeros((sizeN,p))
    for i in range(m):
        Si[i][i] = 1/S[i]
    pseudoInv = V.dot(Si.dot(U.transpose()))
    nrOfCond = max([i for i in S if i>0])/min([i for i in S if i>0])
    rang = np.ndim(np.matrix(S))
    vectorx = pseudoInv.dot(vector)
    toReturn = vector - matrix.dot(vectorx)
    return S,rang,nrOfCond,pseudoInv,vectorx,np.linalg.norm(toReturn)
    



targetFile=files[0]
rareMatrix=createRareMatrix(targetFile)

print(createRareMatrixSimetric(5,10))


matrix,lenght = rareMatrix
print(powMethod(matrix, lenght))


matrix = np.array([[1,2],[3,4]])
S,rang,nrOfCond,pseudoInv,vectorx,norm=SVDMethod(matrix,b,2)


print("Singular Values: ",S,"\nRang: ",rang,"\nConditional number: ",nrOfCond,"\nPseudo Reverse: ",pseudoInv,"\nNorm ||b-Ax||: ",norm,"\nVector X: ",vectorx)