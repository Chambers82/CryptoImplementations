# Programmer: Brent 'q0m' Chambers
# Date: 7/12/2018
# Filename: crypto_ops.py
# Summary: A collection of functions to perform crypto operations
# Description:
"""
The folllowing three types of cyrpto algorithms are used most often:
    Hash Functions (hashlib, hmac)
    Keyed Hash Functions
    Symmetric Encryption
    Public Key Algorithms
"""


# Hash Functions
# Used in cryptogrpahy to check the integrity of messages, digital signatures
# manipulation detection, fingerprints, and password storage.

### HMAC - Hashed Message Authentication Code
### Used to check for integrity and authenticity.

#### Create a HMAC hash with the default MD5 algorithm:

def create_hmac_hash(filename):
    import hmac
    import hashlib

    digest_maker = hmac.new('secret-key', '', hashlib.sha256)
    f = open('sample-file.txt', 'rb')
    try:
        while True:
            block = f.read(1024)
            if not block:
                break
            digest_maker.update(block)
    finally:
        f.close()
    digest = digest_maker.hexdigest()
    print(digest)


    
