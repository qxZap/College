import nltk
from nltk.corpus import treebank
from nltk.stem import WordNetLemmatizer
wordnet_lemmatizer = WordNetLemmatizer()
punctuations="?:!.,;"

text ="""We do not recommend downloading WORMHOLE.EXE from "EXE download" sites. These sites distribute EXE files that are unapproved by the official WORMHOLE.EXE file developer, and can often be bundled with virus-infected or other malicious files. If you require a copy of WORMHOLE.EXE, it is recommended that you obtain it directly from Microsoft."""
tokens = nltk.word_tokenize(text)
print("Tokens:",tokens)
tagged = nltk.pos_tag(tokens)
print("Tagged:",tagged)
entities = nltk.chunk.ne_chunk(tagged)
print("Entities:",entities)


sentence_words = nltk.word_tokenize(text)
for word in sentence_words:
    if word in punctuations:
        sentence_words.remove(word)

print("{0:20}{1:20}".format("Word","Lemma"))
for word in sentence_words:
    print ("{0:20}{1:20}".format(word,wordnet_lemmatizer.lemmatize(word)))

# for word in text:#.split('.').split(" ").split(','):
#     print ("{0:20}{1:20}".format(word,wordnet_lemmatizer.lemmatize(word, pos="v")))
#t = treebank.parsed_sents('nush.mrg')[0]
#t.draw()
