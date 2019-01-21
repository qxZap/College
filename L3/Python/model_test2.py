import urllib.request
import os
from hashlib import md5
from hashlib import sha256
import zipfile
import socket


def problema1(s):
	list = []
	word = ""
	for i in s:
		if i.isalpha() or i.isdigit() or i=='_':
			word+=i
		else:
			list.append(word)
			word=""

	list.append(word)
	while "" in list:
		list.remove("")
	return sorted(list)

def problema2(s,url):
	contents = urllib.request.urlopen(url).read()
	return s in str(contents)

def problema3(path):
	returned = []
	for (root,directories,files) in os.walk(path):
		for name in files:
			fullPathName = os.path.join(root,name)
			m = md5()
			m.update(open(fullPathName,"rb").read())
			returned.append(m.hexdigest())

	return sorted(returned)

def problema4(path):
	list = []
	z = zipfile.ZipFile(path)
	for i in z.infolist():
		if i.compress_size > 1000:
			list.append(os.path.basename(i.filename))


	return sorted(list)

def problema5(host,port,text):
	s = socket.socket()
	s.connect((host,port))
	#s.send(str.encode(text))
	s.send(text.encode())
	back = s.recv(32)

	#s.send(str.encode(sha256(back).hexdigest()))
	s.send((sha256(back).hexdigest()).encode())
	back = s.recv(32)

	return str( back, 'utf-8')


# print(problema1(s="@c3sta 3st3, un cuvant_."))
# print(problema2("facebook","https://mbasic.facebook.com/"))
# print(problema3("testare/incerc"))
# print(problema4("faza/cuzip/zapp.zip"))
# print(problema5('127.0.0.1',6636,'ciorba'))