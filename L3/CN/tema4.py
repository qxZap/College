import numpy as np
import time
start = time.time()
epsilon = 10 ** (-15)
import math

def createRareMatrix(fileName):
    file = open(fileName, 'r', closefd=True)
    matrixLen = int(file.readline())
    lista = []
    dictionar = {}
    linesCount = 0
    toSTR = []
    file.readline()
    vecLinie = file.readline().split(',')
    while len(vecLinie) == 3:
        number = float(vecLinie[0])
        linie = int(vecLinie[1])
        coloana = int(vecLinie[2])
        dictionar[linie,coloana] = number
        vecLinie = file.readline().split(',')
    while linesCount < matrixLen:
        toSTR.append(file.readline().strip())
        lista = [float(i) if '.' in i else float(i) for i in toSTR]
        linesCount += 1
    return dictionar, lista, matrixLen

def less(firstVector, secondVector):
    vecr = [0.0 for i in range(len(firstVector))]
    for i in range(len(firstVector)):
        vecr[i] = firstVector[i]-secondVector[i]
    return vecr

def checkDiag(matrix):
    for keyChain, value in matrix.items():
        if keyChain[0] == keyChain[1] and matrix[keyChain] < epsilon:
                return False
    return True

def timesMatrix(matrix, xVar, matrixLen):
        vecr = [0.0 for i in range(matrixLen)]
        for keyChain, value in matrix.items():
            vecr[keyChain[0]] += matrix[keyChain] * xVar[keyChain[1]]
        return vecr


def sort(matrix, vector, len, omega):
    finalStatus=""
    if checkDiag(matrix):
        xC = [0.0 for i in range(0, len)]
        kValue = 0
        deltaX = 10**(-15)
        kmax = 10000
        while epsilon <= deltaX <= 10 ** 8 and kValue <= kmax:
            xp = xC.copy()
            for keys, value in matrix.items():
                xC[keys[0]] = (1 - omega) * xC[keys[0]]
                firstS = 0
                secondS = 0
                if math.fabs(keys[1]) < keys[0]:
                    firstS += matrix[keys] * xC[keys[1]]
                if math.fabs(keys[1]) >= (keys[0] + 1):
                    secondS += matrix[keys] * xp[keys[1]]
                xC[keys[0]] += math.fabs(omega * (vector[keys[0]] - firstS - secondS)) / matrix[keys[0], keys[0]]
            deltaX = np.linalg.norm(less(xC, xp))
            kValue += 1
        if deltaX < epsilon:
            finalStatus="Solution: "+str(xC)+"\nNumber of iterations: "+str(kValue)+"\nNorm :"+str(np.linalg.norm(less(timesMatrix(matrix, xC, len), vector)))
        else:
            finalStatus="Divergent"

        print(finalStatus)
        return xC
    else:
        print("Can`t be calculated")
    return [False]

fileMatrix=2
fileName="m_rar_2019_"+str(fileMatrix)+".txt"
matrixV,vectorV,matrixLen = createRareMatrix(fileName)
solution = sort(matrixV, vectorV, matrixLen, 1.0)     
print("X1: ",solution[0])
end = time.time()
print("Time of execution: ",end - start)