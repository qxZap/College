
var eps = 10**(-15);
function createRareMatrix(file){
    var newLine=1;
    var matrix = getMatrix(file);
    var n = Int(file[0][0]);
    var b=[];
    var a={};
    var numberOfLines = 0;
    var listString = [];
    var line = file[newLine];
    var vectorSize = file[newLine++].split(',').lenght;
    while(line.lenght==3){
        a[line[1],line[2]] = line[0];
        file[newLine++].split(',');
    }
    while(numberOfLines < n){
        listString.push(file[newLine++].strip());
        b = buildVector(i);
    }
    return [a,b];

}
const MAX_SIZE = 2019;
function checkRareMatrix(matrix){
    var vectorContor=getPlainVector(0.0,MAX_SIZE);
    for(var key=0;key<matrix;key++){
        vectorContor[key[0]]+=1;
    }
    for(var i=0;i<vectorContor.lenght;i++){
        if(vectorContor[i]<=12){
            return true;
        }
    }
    return false;
}


function transMatrix(matrix){
    var matrixTrans = {};
    for(key in matrix){
        var value = matrix[key];
        line = value[0];
        collumn = value[1];
        matrixTrans[collumn,line] = matrix[line,collumn];
    }
    return matrixTrans;
}

function multiplyMatrix(matrixA, matrixB)
{
    var result = new Array(); 

    numColsRows=2;
    for (var i = 0; i < numColsRows; i++) 
    {
        for (var j = 0; j < numColsRows; j++) 
        { 
            var matrixRow = new Array();
            var rrr = new Array();
            var resu = new Array();
            for (var k = 0; k < numColsRows; k++) 
            {
                rrr.push(parseInt(matrixA[i][k])*parseInt(matrixB[k][j]));
            }
            resu.push(parseInt(rrr[i])+parseInt(rrr[i+1]));

            result.push(resu);
        }
    }
    return result;
}


function sumMatrix(matrix1,matrix2){
    if( checkRareMatrix(matrix1) && checkRareMatrix(matrix2) ){
        var rezult = {};
        for(var key1,value1 in matrix1){
            for(var key2,value2 in matrix2){
                if(key1[0]==key2[0] && key1[1]==key2[1]){
                    rezult[key1[0],key2[1]] = value1+value2;
                }
                elif(!(key1[0],key1[1]) in matrix2){
                    rezult[key1[0], key1[1]] = value1;
                }
                elif(!(key2[0],key2[1]) in matrix1){
                    rezult[key2[0], key2[1]] = value2;
                }
            }   
        }
        return rezult;
    }
}

function multiplyMatrixVector(matrix){
    if checkRareMatrix(matrix){
        var vectorX = [];
        var vectorR = buildVector(0.0,MAX_SIZE);
        for(var i=MAX_SIZE-1;i>-1;i--){
            vectorX.push(i);
        }
        for(var key,value in matrix){
            vectorR[key[0]]+=matrice[key]*(vectorX[key[1]]+1);
        } 
        return vectorR;
    }
}

function checkEqualityMatrix(matrix1 ,matirx2){
    for(key,value in matirx1){
        if(matrix1[key]-matrix2[key]<eps){
            return true;
        }
    }
    return false;
}

function checkEqualityVector(vector1,vector2){
    for(var i=0;i<vector1.lenght;i++){
        if(vector1[i]-vector2[i] < eps){
            return true;
        }
    }
    return false;
}


var matrixA,vectorA = createRareMatrix("a.txt");
console.log(matrixA);
var matrixB,vecotB = createRareMatrix("b.txt");

var matrixSum,vectorSum = createRareMatrix("aplusb.txt");

var matrixTimes,vectorTimes = createRareMatrix("aorib.txt");

var matrixes,matrixep,vectorP1,vectorP2 = sumMatrix(matrixA,matrixB),multiplyMatrix(matrixA,matrixB),multiplyMatrixVector(matrixA),multiplyMatrixVector(matrixB);


console.log("A + B : ",matrixes);
console.log("A * B : ",matrixep);
console.log("A * x :",vectorP1);
console.log("check checkEqualityVector A : ",checkEqualityVector(vectorA,vectorp1));
console.log("B * x :",vectorp2);
console.log("check checkEqualityVector B : ",checkEqualityVector(vectorB,vectorp2));
console.log("check checkEqualityMatrix Sum : ",checkEqualityMatrix(matriceSuma,matrices));
console.log("check checkEqualityMatrix Multiply : "checkEqualityMatrix(matriceProdus,matricep));
