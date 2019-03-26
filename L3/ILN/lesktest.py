import nltk
from nltk.tokenize import sent_tokenize
from nltk import word_tokenize
from nltk import wsd
from nltk.corpus import stopwords
from nltk import word_tokenize, pos_tag, ne_chunk
import spacy
from spacy import displacy
from collections import Counter
import en_core_web_sm
from nltk.chunk import conlltags2tree, tree2conlltags
from pprint import pprint
import textstat
SynSets =[]

text = """The Nyetya campaign was also a supply chain attack, one of
many that Cisco threat researchers observed in 2017. One
reason Nyetya was successful at infecting so many machines
so quickly is that users did not see an automated software
update as a security risk, or in some cases even realize that
they were receiving the malicious updates.
Before the rise of self-propagating ransomware, malware
was distributed in three ways: drive-by download, email, or
physical media such as malicious USB memory devices. All
methods required some type of human interaction to infect a
device or system with ransomware. With these new vectors
being employed by attackers, an active and unpatched
workstation is all that is needed to launch a network-based
ransomware campaign. Supply chain attacks appear to be increasing in velocity and
complexity. They can impact computers on a massive scale,
and can persist for months or even years. Defenders should
be aware of the potential risk of using software or hardware
from organizations that do not have a responsible security
posture. Look for vendors that issue CVEs, are quick to
address vulnerabilities, and consistently strive to ensure that
their build systems can’t be compromised. Also, users should
take time to scan new software before downloading it to verify
that it doesn’t contain malware. Cisco threat researchers analyzed email telemetry from
January through September 2017 to identify the types of
malicious file extensions in email documents that common
malware families employed most often. The analysis yielded
a top 10 list that shows the most prevalent group of malicious
file extensions (38 percent) was Microsoft Office formats such
as Word, PowerPoint, and Excel. Archive files, such as .zip and .jar, accounted for about 37
percent of all the malicious file extensions observed in our
study. That adversaries heavily employ archive files is not
surprising, as they have long been favored hiding places
for malware. Users must open archive files to see the
contents—an important step in the infection chain for many
threats. Malicious archive files also often find success in
foiling automated analysis tools, especially when they contain
threats that require user interaction for activation. Adversaries
will also use obscure file types, such as .7z and .rar, to
evade detection."""
sentences = sent_tokenize(text)
stopwordsList =  stopwords.words('english')


def preprocess(sent):
    sent = nltk.word_tokenize(sent)
    sent = nltk.pos_tag(sent)
    return sent

synsDic = {}

for sentence in sentences:
    raw_tokens =  word_tokenize(sentence)
    final_tokens = [token.lower() for token in raw_tokens 
                if(not token in stopwordsList)
                #and (len(token) > 3) 
                and not token.isdigit()]
    for token in final_tokens:
        synset = wsd.lesk(sentence, token)
        if not synset is None:
            SynSets.append(synset)
            synsDic[synset]=token


SynSets = set(SynSets)

SynSets = sorted(SynSets)
with open("synsets.txt", "a") as file:
    file.write("\n---------------------\n")
    for synset in SynSets:
        file.write("{} -> {}\n".format(str(synset.__str__()),synsDic[synset]))
file.close()
sentencesCount = textstat.sentence_count(text)
print("Sentences: ",sentencesCount)
prepro = preprocess(text)

# # for i in prepro:
# #     print(i,"\n")

print("Tokens inc:",len(prepro))
pattern = 'NP: {<DT>?<JJ>*<NN>}'

cp = nltk.RegexpParser(pattern)
cs = cp.parse(prepro)
print("Tokens ex:",len(cs))
iob_tagged = tree2conlltags(cs)
ne_tree = ne_chunk(pos_tag(word_tokenize(text)))
nlp = en_core_web_sm.load()
doc = nlp(text)
print("Entities: ",len(doc.ents))
pprint([(X.text, X.label_) for X in doc.ents])

tuples = [(X.text, X.label_) for X in doc.ents]



# persons = 0
# location = 0
# organizations = 0
# date = 0
# event = 0

totalDic={}

for i  in tuples:
    if(i[1] not in totalDic):
        totalDic[i[1]]=1
    else:
        totalDic[i[1]]+=1
for i in totalDic:
    print(i,":",totalDic[i])
# print(totalDic)

 
# print(ne_chunk(pos_tag(word_tokenize(text))))