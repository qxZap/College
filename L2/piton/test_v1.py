#! /usr/bin/env python3

import re
from urllib.request import urlopen
import os
import hashlib
import zipfile
import socket
import json


def problema1(s):
    words = re.findall(r"[a-z]+", s, re.I)
    return list(sorted(words, key=lambda arg: arg[-1]))
    

def problema2(url, cheie):
    with urlopen(url) as stream:
        data = stream.read().decode()
    details = json.loads(data)
    #print(details)
    length = len(details.get(cheie, []))
    #print(length)
    return length


def problema3(path):
    hashes = {}
    for ents in os.walk(path):
        value = hashlib.md5(ents[0].encode()).hexdigest()
        for fname in ents[2]:
            hashes[fname] = value
    return hashes


def problema4(director, arhiva):
    # Lista totala de fisiere.
    files = []

    # Parcurge recursiv directorul.
    for ents in os.walk(director):
        #print(ents)
        files.extend(ents[2])

    # Scoatem fisierele din arhiva.
    zp = zipfile.ZipFile(arhiva)
    for zip_file in zp.filelist:
        path = zip_file.filename
        name = os.path.basename(path)
        if name:
            files.append(name)

    #print(files)
    return files


def problema5(url):
    # Citim datele de conectare primite de la URL.
    with urlopen(url) as stream:
        data = stream.read().decode()
    details = json.loads(data)

    # Deschidem on socket de comunicare cu serverul primit.
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    host, port, info = [details[key] for key in ("ip", "port", "info")]
    # Ne conectam la adresa.
    sock.connect((host, port))
    # Trimitem informatia ca bytes si obtinem un string din raspunsul primit.
    sock.send(info.encode())
    resp = sock.recv(10)
    obtained = resp.decode()
    #print(obtained)
    return obtained
