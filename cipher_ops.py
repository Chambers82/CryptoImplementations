# Programmer: Brent 'q0m' Chambers
# Date: 7/12/2018
# Filename: cipher_ops.py
# Summary: A collection of functions to perform cipher operations
# Description:

from Cryptodome.PublicKey import RSA
from Cryptodome.Random import get_random_bytes
from Cryptodome.Cipher import AES, PKCS1_OAEP

class CIPHER:
    password    = "supportfreeinfo"
    private_key = "private.key.bin"
    public_key  = "public.pem"

    def __init__(self, password="supportfreeinfo", private_key="private.key.bin", public_key="public.pem"):
            self.password = password
            self.private_key = private_key
            self.public_key = public_key

    def generate_new_rsa_key(self):
        key = RSA.generate(2048)
        print(key)
        # Create an encrypted (private) key from the RSA key, using the password
        encrypted_key = key.export_key(passphrase=self.password, pkcs=8, protection="scryptAndAES128-CBC")
        print(encrypted_key)
        # Save the private key to a .bin (binary) file
        with open(self.private_key, 'wb') as f:
            f.write(encrypted_key)
        # Generate a public key from the previously created private key
        with open(self.public_key, 'wb') as f:
            f.write(key.publickey().export_key())

    def encrypt_file(self, filename):
        with open('ciphertext.enc', 'wb') as out_file:
            recipient_key = RSA.import_key(open(self.public_key).read())
            session_key = get_random_bytes(16)

            cipher_rsa = PKCS1_OAEP.new(recipient_key)
            out_file.write(cipher_rsa.encrypt(session_key))
 
            cipher_aes = AES.new(session_key, AES.MODE_EAX)
            data = open(filename, 'rb').read()
            ciphertext, tag = cipher_aes.encrypt_and_digest(data)
 
            out_file.write(cipher_aes.nonce)
            out_file.write(tag)
            out_file.write(ciphertext)


    def decrypt_file(self, filename):
            password = input("Enter password to decrypt the file: ")
            with open(filename, 'rb') as file_obj:
                private_key = RSA.import_key(open(self.private_key).read(), passphrase=password)
                enc_session_key, nonce, tag, ciphertext = [ file_obj.read(x)
                                                            for x in (private_key.size_in_bytes(),16,16,-1) ]
                cipher_rsa = PKCS1_OAEP.new(private_key)
                session_key = cipher_rsa.decrypt(enc_session_key)
 
                cipher_aes = AES.new(session_key, AES.MODE_EAX, nonce)
                data = cipher_aes.decrypt_and_verify(ciphertext, tag)
 
            print(data)
