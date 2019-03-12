import nltk,re,math
from nltk.corpus import wordnet as wn
punctuations="?:!.,;"
text1 = """
The Nyetya campaign was also a supply chain attack, one of
many that Cisco threat researchers observed in 2017. One
reason Nyetya was successful at infecting so many machines
so quickly is that users did not see an automated software
update as a security risk, or in some cases even realize that
they were receiving the malicious updates.
"""
text2 = """
Before the rise of self-propagating ransomware, malware
was distributed in three ways: drive-by download, email, or
physical media such as malicious USB memory devices. All
methods required some type of human interaction to infect a
device or system with ransomware. With these new vectors
being employed by attackers, an active and unpatched
workstation is all that is needed to launch a network-based
ransomware campaign.
"""
text3 = """
Supply chain attacks appear to be increasing in velocity and
complexity. They can impact computers on a massive scale,
and can persist for months or even years. Defenders should
be aware of the potential risk of using software or hardware
from organizations that do not have a responsible security
posture. Look for vendors that issue CVEs, are quick to
address vulnerabilities, and consistently strive to ensure that
their build systems can’t be compromised. Also, users should
take time to scan new software before downloading it to verify
that it doesn’t contain malware.
"""
text4 = """
Cisco threat researchers analyzed email telemetry from
January through September 2017 to identify the types of
malicious file extensions in email documents that common
malware families employed most often. The analysis yielded
a top 10 list that shows the most prevalent group of malicious
file extensions (38 percent) was Microsoft Office formats such
as Word, PowerPoint, and Excel
"""
text5 = """
Archive files, such as .zip and .jar, accounted for about 37
percent of all the malicious file extensions observed in our
study. That adversaries heavily employ archive files is not
surprising, as they have long been favored hiding places
for malware. Users must open archive files to see the
contents—an important step in the infection chain for many
threats. Malicious archive files also often find success in
foiling automated analysis tools, especially when they contain
threats that require user interaction for activation. Adversaries
will also use obscure file types, such as .7z and .rar, to
evade detection. 
"""


documents = []
documents.append(text1)
documents.append(text2)
documents.append(text3)
documents.append(text4)
documents.append(text5)


wholeText=""
for i in documents:
	wholeText+=" "+i

def getScore(word,document,documents):
	pass

for doc in documents:
	listOfWords= re.split(r"[\s\.,\?\!\ ]+",doc)
	listOfWords.remove('')
	scoreDic = {}

	for i in listOfWords:
		tf = doc.count(i)
		idf=0
		for document in documents:
			if i in document:
				idf+=1
		idf = math.log(len(documents)/idf)
		score = tf*idf
		scoreDic[i]=score
		i.replace(i,"")


sortedDic =  sorted(scoreDic.items(), key=lambda kv: kv[1], reverse=True)
#for i in sortedDic:
#	print(i)
for ss in wn.synsets("small"):
	print(ss,dir(ss))

lemmedWords=[]
lemmed=[]
searchedWords=[]
for i in sortedDic:
	if i[1]>1.0:
		if i[0] not in lemmed:
			lemmed.append(i[0])
			print("\n\nWord: ",i[0])
			found=[]
			for ss in wn.synsets(i[0]):
				syns = ss.lemma_names()
				searchWords=[]
				found=[]
				for syn in syns:
					searchWords.append(syn.replace("_"," "))
				for syn in searchWords:
					searchedWords.append(syn)
					for document in documents:
						if syn in document and syn not in found:
							found.append(syn)
			if len(found):
				print("Found: ",found)
			else:
				print("Nothing else")



			#print(ss.name(),ss.lemma_names())

		#print("gotta look for sinonims for ",i[0])

