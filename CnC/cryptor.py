from Crypto.Cipher import AES
from Crypto import Random
import base64
import hashlib



def generate_key():
    aes_key = Random.new().read(32)
    aes_key = base64.b64encode(aes_key)
    return aes_key


def generate_cipher(aes_key, iv):
    return AES.new(aes_key, AES.MODE_CFB, iv)


def generate_unique_id(input1, input2):
    hashed = hashlib.md5()
    hashed.update(input1 + input2)
    return hashed.hexdigest()
