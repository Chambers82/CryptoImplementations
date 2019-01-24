//Programmer: Brent Chambers
//Date: 5/6/2018
//Filename: aescrypt.c
//Description: AES Encryption Implementation 
//Compile



#include <stdio.h>
#include <windows.h>
#include <wincrypt.h>

#define ENCRYPT_ALGORITHM CALG_RC4  // we are using RC4 algorithm
#define KEYLENGTH  0x00800000
#define ENCRYPT_BLOCK_SIZE 8

HCRYPTPROV GetCryptContainer(LPCSTR keyContainer)
{
	HCRYPTPROV hCryptProv = NULL;

	if (CryptAcquireContext(&amp;hCryptProv,keyContainer,NULL,PROV_RSA_FULL,0))
	{
		printf("A cryptographic provider has been acquired.\n");
	}
	else
	{
	if (GetLastError() == NTE_BAD_KEYSET)
	{
		if (CryptAcquireContext(&amp;hCryptProv,keyContainer,NULL,PROV_RSA_FULL,CRYPT_NEWKEYSET))
		{
			printf("A new key container has been created.\n");
		}
		else
		{
			printf("Could not create a new key container.\n");
		}
	}
	else
	{
		printf("A cryptographic service handle could not be acquired.\n");
	}
}

return hCryptProv;
}

HCRYPTHASH CreateHashObj(HCRYPTPROV hCryptProv, const char * szPassword)
{
	HCRYPTHASH hHash = NULL;

	if(CryptCreateHash(hCryptProv,CALG_MD5,0,0,&amp;hHash))
	{
		printf("A hash object has been created. \n");

		//  hash the password
		if(CryptHashData(hHash,(BYTE *)szPassword,strlen(szPassword),0))
		{
			printf("The password has been added to the hash. \n");
		}
		else
		{
		// reset hash object to NULL
			CryptDestroyHash(hHash);
			hHash = NULL;
		}
	}
	else
	{
		printf("Failed to create hash object.\n");
	}

return hHash;
}

HCRYPTKEY GetDeriveKey(HCRYPTPROV hCryptProv, HCRYPTHASH hHash)
{
	HCRYPTKEY hKey = NULL;

	if(CryptDeriveKey(hCryptProv,ENCRYPT_ALGORITHM,hHash,KEYLENGTH,&amp;hKey))
	{
		printf("An encryption key is derived from the password hash. \n");
	}
	else
	{
		//MyHandleError("Error during CryptDeriveKey!\n");
	}
	return hKey;
}

int _tmain(int argc, char* argv[])
{
	HCRYPTPROV hCryptProv = NULL;       // handle for a cryptographic
	// provider context.
	HCRYPTHASH hHash    = NULL;       // handle for hash object
	
	LPCSTR keyContainer   = "MyKeyContainer"; // name of the key container
	// to be used.
	char szPassword[10]   = "12345";      // password for encryption and
	// decryption
	HCRYPTKEY hKey;               // handle for session key
	PBYTE pbBuffer;               // hold the data being encrypt
	// or decrypt
	DWORD dwCount;                // data length

	// make sure that we have enough argument to do the demo.
	// otherwise exit the program
	if(argc &lt; 2)
	{
		printf("Usage: CryptoConsole.exe [data]");
		exit(1);
	}

	// get CSP
	hCryptProv = GetCryptContainer(keyContainer);

	if(!hCryptProv)
		exit(1); // exit if failed to get CSP

		// get the hash object
		hHash = CreateHashObj(hCryptProv, szPassword);

	if(!hHash)
		exit(1); // exit if failed to get hash object

	// get the session key
	hKey = GetDeriveKey(hCryptProv, hHash);

	if(!hKey)
		exit(1); // exit if failed to get session key

	// determine input data length
	int len = strlen(argv[1]);

	// Determine the block size. If a block cipher is used,
	// it must have room for an extra block.
	DWORD dwBufferLen = len + ENCRYPT_BLOCK_SIZE - (len % ENCRYPT_BLOCK_SIZE);

	// Allocate memory
	if(pbBuffer = (BYTE *)malloc(dwBufferLen))
	{
		printf("Memory has been allocated for the buffer. \n");
	}
	else
	{
		printf("Failed to allocate memory for the buffer. \n");
		exit(1);
	}

	// copy the data value to buffer
	memcpy(pbBuffer, argv[1], dwBufferLen);
	printf("++++++++++++++++++++++++++++++++\n");
	printf("Before encryption: %s\n", argv[1]);
	dwCount = strlen(argv[1]);
	
	// encrypt data
	if(!CryptEncrypt(hKey,0,TRUE,0,pbBuffer,&amp;dwCount,dwBufferLen))
	{
		printf("Error during CryptEncrypt. \n");
	}
	else
	{
		printf("After encryption: %s\n", pbBuffer);
		unsigned char *pDecrypt = pbBuffer;
		DWORD delen = _mbslen(pbBuffer);
	
		// decrypt data
		if(CryptDecrypt(hKey,0,TRUE,0,pbBuffer,&amp;delen))	
		{
			printf("After decryption: %s\n", pbBuffer);	
			printf("++++++++++++++++++++++++++++++++\n");
		}
	}
	
	// Remove all references
	if(pbBuffer)
		free(pbBuffer);
	if(hKey)
		CryptDestroyKey(hKey);	
	if(hHash)
		CryptDestroyHash(hHash);
	if(hCryptProv)
		CryptReleaseContext(hCryptProv,0);
	
	return 0;
}