from flask import Flask
from Crypto.Cipher import AES
from flask_restful import Api, Resource, reqparse
from base64 import b64encode
import os,sys
app = Flask(__name__)
api = Api(app)

k1 = os.urandom(16)
k2 = os.urandom(16)
k3 = 'C\xdd\x92\x1d\xd6\x98P\xaeP?avh5G\xc9'
iv = '\xb2\xec.]\x9f\xf78bnn\xf2\x84\x96\x85gg'



class Keys(Resource):
    def get(self,key):
        key = key.lower()
        if key == 'ecb':
            obj = AES.new(k3,AES.MODE_ECB)
            ctext = obj.encrypt(k1)
            print k1
            return b64encode(ctext),200
        elif key == 'cbc':
            obj = AES.new(k3,AES.MODE_ECB,iv)
            ctext = obj.encrypt(k2)
            print k2
            return b64encode(ctext),200
            

        
        
api.add_resource(Keys, "/<string:key>")


app.run(debug=True)