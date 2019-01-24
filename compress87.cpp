// Programmer: Brent E. Chambers
// Date: 10/19/2017  Autumn of Code
// Filename: compress87.cpp
// Compile with:  g++ -std=c++11 -Wall -Wextra -pedantic-errors -o compress87 compress87.cpp
// Description:  Improved Subsitition Cipher Implementation in C++

#include <iostream.h>
#include <fstream.h>
#include <ctype.h>
#include <stdlib.h>

using namespace std;

void compress(char *input, char *output);
void decompress(char *input, char *output);

main(int argc, char *argv[])
{
	if(argc!=4) {
		cout << "Usage: input output compress/decompress\n";
		exit(1);
	}
	
	if(toupper(*argc[3]) == 'C')
		compress(argv[1], argv[2]);
	else
		decompress(argv[1], argv[2]);
		
	return 0;
}

// Compress

void compress(char *input, char *output)
{
	char ch, ch2, done, t;
	ifstream in(input, ios::in | ios::binary);
	ofstream out(output, ios::out | ios::binary);
	
	if(!in) {
		cout << "Could not open input file.\n";
		exit(1);
	}
	
	if(!out)
	{
		cout << "Could not open output file.\n";
		exit(1);
	}
	
	done = 0;
	do {
		/* Get the first byte. This one will be distributed into the next 7 bytes. */
		ch = in.get();
		ch = ch << 1; //shift off top bit
		
		// distribute remaining 7 bits into next 7 bytes
		for (t=0; t<7; ++t) {
			ch2 = in.get();
			if(in.eof()) {
				ch2 = 0;
				done = 1;
			}
			ch2 = ch2 & 127; //turn off top bit of next byte
			// now, OR in next bit from the first byte
			ch2 = ch2 | ((ch << t) & 128);
			out.put((char) ch2);
		}
	} while(!done && !in.eof());
	
	in.close();
	out.close();
}

// Decompress
void decompress(char *input, char *output)
{
	unsigned char ch, ch2, t;
	char s[7];
	ifstream in(input, ios::in | ios::binary);
	ofsteam out(output, ios::out | ios::binary);
	
	if(!in) {
		cout << "Could not open input file.\n";
		exit(1);
	}
	if(!out) {
		cout << "Could not open output file.\n";
		exit(1);
	}
	
	do {
		ch = 0;
		// reconstruct the first byte
		for (t=0; t<7; ++t) {
			ch2 = in.get();
			s[t] = ch2 & 127; 		//turn off the top bit
			ch2 = ch2 & 128; 		//turn off all but top bit
			ch2 = ch2 >> (t+1); 	//shift over the existing bits
			ch = ch | ch2; 			// OR in the next bit
		}
		
		out.put((char) ch); 		//write out the first byte
		
		//write out the remaining 7
		for(t=0; t<7; ++t) out.put(s[t]);
	} while(!in.eof());
	
	in.close();
	out.close();
		
}
