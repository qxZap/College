from zeep import Client
import xml


def loadFile(file_name):
	return open(file_name,"r").read()

client = Client('https://svn.apache.org/repos/asf/airavata/sandbox/xbaya-web/test/Calculator.wsdl')

prt = client.wsdl.services.get('Calculator').ports
print(prt)
print(dir(prt))
#print(prt.Add(1,2))

file = loadFile("marcel.tx")

wsdl = 'http://nlptools.info.uaic.ro/WebPosRo/PosTaggerRoWS?wsdl'
client = Client(wsdl=wsdl)
res=client.service.parseText_XML(file)
print(res)


wsdl = 'http://nlptools.info.uaic.ro/WebNpChunkerRo/NpChunkerRoWS?wsdl'
client = Client(wsdl=wsdl)
res=client.service.chunkText(file)
print(res)



# print(prt.get('Calculator'))
# print(dir(prt.get('Calculator')))

-122.781, 2.265, 115.536