import math
import random
import time

LIST_SIZE = 10000
LONG_RUN = 100000
HIGH_PI = math.pi/2
LOW_PI = (-math.pi)/2

def fact(n):
	if n==1:
		return 1
	else:
		return n*fact(n-1)

c1=1.0/fact(3)
c2=1.0/fact(5)
c3=1.0/fact(7)
c4=1.0/fact(9)
c5=1.0/fact(11)
c6=1.0/fact(13)

def P1(x):
	return x-c1*x**3+c2*x**5

def P2(x):
	return x-c1*x**3+c2*x**5-c3*x**7

def P3(x):
	return x-c1*x**3+c2*x**5-c3*x**7+c4*x**9

def P4(x):
	return x-0.166*x**3+0.00833*x**5-c3*x**7+c4*x**9

def P5(x):
	return x-c1*x**3+c2*x**5-c3*x**7+c4*x**9-c5*x**11

def P6(x):
	return x-c1*x**3+c2*x**5-c3*x**7+c4*x**9-c5*x**11+c6*x**13

functions=[0,P1,P2,P3,P4,P5,P6]


def P1x(x):
	y=x**2
	return x*(1-y*(c1-c2*y))

def P2x(x):
	y=x**2
	return x*(1-y*(c1-y*(c2-y*c3)))

def P3x(x):
	y=x**2
	return x*(1-y*(c1-y*(c2-y*(c3-y*c4))))

def P4x(x):
	y=x**2
	return x*(1-y*(0.166-y*(0.00833-y*(c3-y*c4))))
	#return x*(1-y*(c1-y*(c2-y*(c3-y*c4))))

def P5x(x):
	y=x**2
	return x*(1-y*(c1-y*(c2-y*(c3-y*(c4-y*c5)))))

def P6x(x):
	y=x**2
	return x*(1-y*(c1-y*(c2-y*(c3-y*(c4-y*(c5-y*c6))))))

print(P3(1.11)==P3x(1.11))
#functions=[0,P1x,P2x,P3x,P4x,P5x,P6x]
#print(c1,c2,c3,c4,c5,c6)


# Prob1
def prob1():
	m=0
	s=0
	while True:
	    s=1+pow(10,-m)
	    if s==1:
	        break
	    else:
	        m=m+1
	return m


def pacanele():
	x = random.uniform(0.9, 1.0)
	u=prob1()
	y = pow(10,-u)
	z = pow(10,-u)
	while (x*y)*z == x*(y*z):		
		x = random.uniform(0.9, 1.0)
		u=prob1()
		y = pow(10,-u)
		z = pow(10,-u)
		print(x,math.log(y,10),math.log(z,10))
	return x,y,z


def prob2():
	u = prob1()
	u = pow(10,-u)
	x = 1.0
	adunare =  (x+u)+u != x+(u+u)
	#x,y,z = pacanele()
	#x,y,z=10.0,pow(10,-1),pow(10,-1)
	x,y,z=10.0,u,u
	print((x*y)*z,x*(y*z))
	inmultire = (x*y)*z != x*(y*z)
	return adunare,inmultire

def prob2x():
	u = prob1()
	u = pow(10,-u)
	true = 0
	false =0
	while True:
		x= random.uniform(0.9, 1.0)
		inmultire = (x*u)*u != x*(u*u)
		if inmultire:
			true+=1
		else:
			false+=1
		print(true,false)

def generateList(amount,low,high):
	toReturn = []
	for i in range(0,amount):
		newNum = random.uniform(low,high)
		toReturn.append(newNum)
	return toReturn


def doFunction(function,input):
	#print(functions[function](input),math.sin(input))
	return functions[function](input)==math.sin(input)

def runFunction(function,inputList):
	rate = 0
	for i in inputList:
		if doFunction(function,i):
			rate+=1
	return rate


def prob3():
	listNums = generateList(LIST_SIZE,LOW_PI,HIGH_PI)
	results = [0,0,0,0,0,0,0]
	for i in range(1,7):
		results[i]=runFunction(i,listNums)
	bestOnes = sorted(range(len(results)), key=lambda k: results[k])[-3:][::-1]
	print(bestOnes)
	return results

def prob3Bonus(runs=LONG_RUN):
	listNums = generateList(runs,LOW_PI,HIGH_PI)
	results = [0,0,0,0,0,0]
	times = [0,0,0,0,0,0]
	for i in range(1,7):
		ts = time.time()
		results[i-1]=runFunction(i,listNums)
		times[i-1]=time.time()-ts
	fastestOnes = sorted(range(len(times)), key=lambda k: times[k])
	fastestOnes = [x+1 for x in fastestOnes]
	bestOnes = sorted(range(len(results)), key=lambda k: results[k])[::-1]
	bestOnes = [x+1 for x in bestOnes]
	
	return fastestOnes,times,results,bestOnes

from tkinter import *
from tkinter import messagebox as mb

class ZAPFrm:
	def __init__(self):
		self.runs = None

	def doRun(self):
		
		try:
			self.fresh = False
			input = int(self.runs.get())
			output = prob3Bonus(runs=input)
			fastestOnes = output[0]
			times = output[1]
			results = output[2]
			bestOnes = output[3]
			for i in range(1,7):
				labelText = "P"+str(i)+":"
				Label(self.root, text=labelText).grid(row=i,column=0)
				rantime=str(times[i-1])+"s"
				Label(self.root, text=rantime).grid(row=i,column=1)
				howmany=str(results[i-1])
				Label(self.root, text=howmany).grid(row=i,column=2)
				precentage=str(results[i-1]/float(self.runs.get())*100.0)+"%"
				Label(self.root, text=precentage).grid(row=i,column=3)

			Label(self.root, text="Top by runtime").grid(row=7,column=0)
			counter=1
			for i in fastestOnes:
				labelText="P"+str(i)+":"
				Label(self.root, text=labelText).grid(row=7+counter,column=0)
				labelText=str(times[i-1])+"s"
				Label(self.root, text=labelText).grid(row=7+counter,column=1)
				counter+=1

			Label(self.root, text="Top by success rate").grid(row=7,column=2)
			counter=1
			for i in bestOnes:
				labelText="P"+str(i)+":"
				Label(self.root, text=labelText).grid(row=7+counter,column=2)
				labelText=str(results[i-1])
				Label(self.root, text=labelText).grid(row=7+counter,column=3)
				counter+=1
		except ValueError as e:
			mb.showerror("Error", "Wrong input. Integer values only")

	def CreateForm(self):
		self.root = Tk()
		self.root.iconbitmap(r'C:\Dock\icons\piton.ico')
		self.root.title("Tema 1 CN")
		Label(self.root, text="Runs:").grid(row=0,column=0)
		self.runs = Entry(self.root)

		self.runs.grid(row=0, column=1)
		Button(self.root, text="Run", command=self.doRun).grid(row=0,column=2)
		self.root.mainloop()


af = ZAPFrm()
af.CreateForm()


print(prob1())
print(prob2())
print(prob3())
print(prob3Bonus())


