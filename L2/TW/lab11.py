lst = [str.upper, str.lower]

uses={"6636":99999}
ep={
  "title": "Battle of the bastards",
  "duration": 61
}
episodes=[
        [ep,ep,ep],
        [ep,ep,ep],
        [ep,ep,ep],
        [ep,ep,ep],
        [ep,ep,ep],
        [ep,ep,ep],
        [ep,ep,ep],
        [ep,ep,ep],
        [ep,ep,ep],
        [ep,ep,ep]
]



def dumpFiles():
        filename = 'tokens.json'
        outfile = open(filename,'wb')
        pickle.dump(uses,outfile)
        outfile.close()

        
        filename = 'episodes.json'
        outfile = open(filename,'wb')
        pickle.dump(episodes,outfile)
        outfile.close()

def getFiles():
        filename = "tokens.json"
        infile = open(filename,'rb')
        uses = pickle.load(infile)
        infile.close()

        
        filename = "episodes.json"
        infile = open(filename,'rb')
        episodes = pickle.load(infile)
        infile.close()
getFiles()


def id_generator(size=8, chars=string.ascii_uppercase + string.digits):
    return ''.join(random.choice(chars) for _ in range(size))

@app.route('/get_api/key',methods=['GET'])    
@cross_origin(origin="*")
def getApiKey():
    key=''.join(choice(lst)(c) for c in id_generator())
    uses[key]=15
    return jsonify(
                key=key
          ),200

def isKeyValid(key):
        return key in uses

def useKey(key):
        if uses[key]<1:
                return False
        else:
                uses[key]-=1
                return True


@app.route('/got_api/season/<int:s_no>/episode/<int:e_no>',methods=['GET'])    
@cross_origin(origin="*")
def getEpisode(s_no,e_no):
        key=request.headers['Bearer']
        if isKeyValid(key):
                if useKey(key):
                        episode = None
                        try:
                                episode=episodes[s_no][e_no]
                        except IndexError:
                                episode = None
                        if episode==None:
                                return jsonify(
                                        reason="Episode does not exist"
                                        ),404
                        return jsonify(
                                        info=episodes[s_no][e_no]
                                        ),200
                else:
                        return jsonify(
                                        reason="Expired"
                                        ),401 
        else:
               return jsonify(
                                reason="Invalid"
                                ),401 



@app.route('/got_api/season/<int:s_no>/episode/<int:e_no>',methods=['PUT'])    
@cross_origin(origin="*")
def putEpisode(s_no,e_no):
        key=request.headers['Bearer']
        if isKeyValid(key):
                if useKey(key):
                        content = request.get_json()
                        episodes[s_no][e_no] = content
                        if "title" not in content or "duration" not in content:
                              return jsonify(
                                reason="Body Invalid"
                                ),400  
                        
                        dumpFiles()
                        return jsonify(
                                reason="Created"
                                ),201
                else:
                        return jsonify(
                                reason="Expired"
                                ),401 
        else:
               return jsonify(
                                reason="Invalid"
                                ),401 


@app.route('/got_api/season/<int:s_no>/episode/<int:e_no>',methods=['POST'])    
@cross_origin(origin="*")
def postEpisode(s_no,e_no):
        key=request.headers['Bearer']
        if isKeyValid(key):
                if useKey(key):
                        episode = None
                        try:
                                episode=episodes[s_no][e_no]
                        except IndexError:
                                episode = None
                        if episode==None:
                                return jsonify(
                                        reason="Episode does not exist"
                                        ),404
                        content = request.get_json()
                        episodes[s_no][e_no] = content
                        if "title" not in content or "duration" not in content:
                              return jsonify(
                                reason="Body Invalid"
                                ),400
                        if "title" in content:
                                episodes[s_no][e_no]["title"]=content["title"]
                                episodes[s_no][e_no]["duration"]=content["duration"]
                        dumpFiles()
                        return jsonify(
                                reason="Updated"
                                ),200
                else:
                        return jsonify(
                                reason="Expired"
                                ),401 
        else:
               return jsonify(
                                reason="Invalid"
                                ),401 

  

@app.route('/got_api/season/<int:s_no>/episode/<int:e_no>',methods=['DELETE'])   
@cross_origin(origin="*")
def deleteEpisode(s_no,e_no):
        key=request.headers['Bearer']
        if isKeyValid(key):
                if useKey(key):
                        episode = None
                        try:
                                episode=episodes[s_no][e_no]
                        except IndexError:
                                episode = None
                        if episode==None:
                                return jsonify(
                                        reason="Episode does not exist"
                                        ),404
                        episodes[s_no][e_no]=None
                        dumpFiles()
                        return jsonify(
                                        reason="Delete successful!"
                                        ),200
                else:
                        return jsonify(
                                        reason="Expired"
                                        ),401 
        else:
               return jsonify(
                                reason="Invalid"
                                ),401 




@app.route('/post/<string:name>', methods = ['POST'])
@cross_origin(origin="*")
def doPost(name):
    content = request.get_json()
    if(uses[content["key"]]>1):
        uses[content["key"]]-=1
        return jsonify(
                text="ok"
          ),200
    else:
        return jsonify(
                key="NO"
          ),403
