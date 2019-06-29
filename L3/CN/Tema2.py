import numpy as np
import sys

def readFromFileNAB():
    matrixA = []
    matrixB = []
    with open("file.txt", 'r') as f:
        sizeN = int(f.readline())
        lines = 0
        while lines < sizeN:
            line = f.readline()
            matrixA += [[float(i) if '.' in i else int(i) for i in line.split(' ')]]
            lines += 1
        lines = 0
        while lines < sizeN:
            line = f.readline()
            matrixB += [[float(line) if '.' in line else int(line)]]
            lines += 1
    return [sizeN, matrixA, matrixB]



def LUdecomposition():
    data = readFromFileNAB()
    matrix = data[1]
    sizeN = data[0]
    matrixLU = [[0.0] * sizeN for i in range(sizeN)]
    for i in range(0, sizeN):
        matrixLU[i][0] = matrix[i][0]
    for i in range(1, sizeN):
        matrixLU[0][i] = matrix[0][i] / matrixLU[0][0]
    for i in range(1, sizeN // 2):
        matrixLU[i][i] = matrix[i][i] - matrixLU[i][i - 1] * matrixLU[i - 1][i]
    for i in range(1, sizeN):
        for j in range(1, sizeN):
            if i > j:
                sum = 0
                for k in range(0, j):
                    sum += matrixLU[i][k] * matrixLU[k][j]
                    matrixLU[i][j] = (matrix[i][j] - sum)
            if i == j:
                for l in range(sizeN // 2, sizeN):
                    sum = 0
                    for k in range(0, l):
                        sum += matrixLU[l][k] * matrixLU[k][l]
                        matrixLU[l][l] = matrix[l][l] - sum
            if i < j:
                sum = 0
                for k in range(0, i):
                    sum += matrixLU[i][k] * matrixLU[k][j]
                    matrixLU[i][j] = (matrix[i][j] - sum) / matrixLU[i][i]

    return matrixLU




def printMatrix(matrix):
	for i in matrix:
		print(i)


def getDeterminantOfA():
    matrix = LUdecomposition()
    data = readFromFileNAB()
    sizeN = data[0]
    for i in range(0, sizeN):
        for j in range(0, sizeN):
            if i < j:
                matrix[i][j] = 0
    return np.linalg.det(matrix)


def getXY():
    data = readFromFileNAB()
    sizeN = data[0]
    matrixB = data[2]
    matrix = LUdecomposition()
    y = [[0.0] for i in range(sizeN)]
    x = [[0.0] for i in range(sizeN)]
    

    for i in range(0, sizeN):
        k = 0
        sum = 0
        for j in range(0, i):
            sum += matrix[i][j] * y[k][0]
            k += 1
        y[i][0] = ((matrixB[k][0] - sum) / matrix[i][i])

    print("Y = ", y)
    
    for i in range(0, sizeN):
        for j in range(0, sizeN):
            if i == j:
                matrix[i][i] = 1
            if i > j:
                matrix[i][j] = 0
    for i in range(0, sizeN):
        k = 0
        sum = 0
        for j in range(0, i):
            sum += matrix[sizeN-i-1][sizeN-j-1] * x[k][0]
            k += 1
        x[i][0] = ((y[sizeN-k-1][0] - sum) / matrix[sizeN-i-1][sizeN-i-1])
    x = x[::-1]
    return x



def matrixTimes(matrixOne,matrixTwo):
    sizeN = len(matrixOne[0])
    firstListMatrix=[]
    secondListMatrix=[]
    if len(matrixOne[0])==len(matrixTwo):
        for i in range(0,len(matrixOne)):
            for j in range(0,len(matrixTwo[0])):
                l=0
                sum = 0
                for k in range(0,len(matrixTwo)):
                    l+=(matrixOne[i][k]*matrixTwo[k][j])
                secondListMatrix.append(l)
                l=0
            firstListMatrix.append(secondListMatrix)
            secondListMatrix=[]
        return firstListMatrix
    return 0



def calculateNorms():
    data = readFromFileNAB()
    sizeN = data[0]
    matrix = data[1]
    matrixB = data[2]
    xLU= getXY()
    matriceInversa = np.linalg.inv(matrix)
    print("Reversed matrix \n sizeN",matriceInversa)
    xlib = np.matmul(matriceInversa,matrixB)
    norma1 = np.linalg.norm(xLU-xlib)
    print("Norm ||xLU − xlib|| = ",norma1)
    rez = matrixTimes(matriceInversa,matrixB)
    norma2 = np.linalg.norm(np.subtract(xLU,rez))
    print("Norm ||xLU − matrixA^−1lib*binit|| = ",norma2)



def calculateNorm():
    x = getXY()
    matrix = data[1]
    matrixB = data[2]
    calculateNorm = np.dot(matrix, x)
    calculateNorm = np.subtract(calculateNorm,matrixB)
    calculateNorm = np.linalg.norm(calculateNorm)
    if calculateNorm < eps:
        print("The norm is ",calculateNorm," and is smaller than ", eps)


if(len(sys.argv)>1):
	t=sys.argv[1]
else:
	t = input("T:")

eps = 10**(-int(t))
print("matrixLU matrix")
printMatrix(LUdecomposition())
print("determinant of  matrixA = ", getDeterminantOfA())
print(" X = ", getXY())
calculateNorm()
calculateNorms()