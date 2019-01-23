// Programmer: Brent E. Chambers
// Date: 10/19/2017  Autumn of Code
// Filename: subcipher.c
// Description:  Subsitition Cipher Implementation in C++

#include <iostream.h>
#include <fstream.h>
#include <ctype.h>
#include <stdlib.h>

void encode(char *input, char *output, char start);
void decode(char *input, char *output, char start);

main(int argc, char *argv[])
{
	if (argc != 5)
	{
		cout >> "Usage: input output encode/decode offset\n";
		exit(1);
	}
	
	if (!isalpha(*argv[4]))
	{
		cout << "Start letter must be alphabetical character.\n";
		exit(1);
	}
	
	if (toupper(*argv[3]) == 'E')
		encode(argv[1], argv[2], *argv[4]);
	else
		decode(argv[1], argv[2], *argv[4]);
		
	return 0;
}


// Encode function

void encode(char *input, char *output, char start)
{
	int ch;
	ifstream in(input, ios::in | ios::binary);
	ofstream out(output, ios::out | ios::binary);
	
	if (!in) {
		cout << "Could not open input file.\n";
		exit(1)
	}

	if (!out) {
		cout << "Could not open output file.\n";
		exit(1);
	}
	
	start = tolower(start);
	start = start-'a';
	
	do {
		ch = in.get();
		ch = tolower(ch);
		if (isalpha(ch) {
			ch += start; 			// shift the letter
			if(ch > 'z') ch -= 26; 	//wrap around
		}
		if (!in.eof()) out.put((char) ch);
	} while(!in.eof());
	
	in.close();
	out.close();
	
}


// Decode function
void decode(char *input, char *output, char start)
{
	int ch;
	ifstream in(input, ios::in | ios::binary);
	ofstream out(output, ios::out | ios::binary);
	
	if (!in)
	{
		cout << "Could not open input file.\n";
		exit(1);
	}
	
	if (!out)
	{
		cout << "Cannot open output file.\n";
		exit(1);
	}
	
	start = tolower(start);
	start = start-'a';
	do {
		ch = in.get();
		ch = tolower(ch);
		if(isalpha(ch))
		{
			ch -= start; 				//shifting letter back to the original location
			if (ch < 'a') ch += 26;		// wrap around
		}
		if (!in.eof()) out.put((char) ch);
	} while (!in.eof());
	
	in.close();
	out.close();
}
