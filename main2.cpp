#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <string>
#include <vector>
using namespace std;

/* A slightly different syllable generator, that uses (most of) the 44 English phonemes to construct arbitrary
 * syllables, and combines those with numbers and symbols to make random but pronouncable passwords.
 * 44 English phonemes from http://www.boardman.k12.oh.us/userfiles/363/Phonological%20Awareness/44Phonemes.pdf
 */

// combine a consonant sound and a vowel sound to make a syllable
// 24 consonant sounds (would be 25 but soft and hard "th" cannot be differentiated in writing)
const char *consonants[] = 
{
	"b", "d", "f", "g", "h", "j", "k", "l", "m", "n", "p", "r", 
	"s", "t", "v", "w", "y", "z", "th", "ng", "sh", "ch", "zh", "wh"
};
// 19 vowel sounds; 5 each hard and soft vowel sounds, short and long oo, two dipthongs, 5 vowels + r
const char *vowels[] = 
{
	"a", "e", "i", "o", "u", "ai", "ee", "ie", "oa", "ew",
	"oo", "ou", "ow", "oy", "ar", "air", "irr", "or", "ur"
};

// 27 symbols
const char *symbols[] = 
{
	"!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "-", 
	"+", "=", "{", "[", "}", "]", "|", ":", ";", "?", "/", "~",
	".", ",", "\\"
};

// generates a random integer between 0 and size; not perfect distribution but close for numbers under a million
unsigned int GenerateRandom(unsigned int size)
{
	FILE *fd = fopen("/dev/urandom", "r");
	if(fd == NULL) throw;

	int random;
	size_t bytesRead = fread((char *)&random, 1, 4, fd);
	fclose(fd);

	return random % size;
}

float GeneratePassword(string pattern)
{
	int numConsonants = sizeof(consonants) / sizeof(char *);
	int numVowels = sizeof(vowels) / sizeof(char *);
	int numSymbols = sizeof(symbols) / sizeof(char *);
	float count = 1.0; // count of possible passwords matching the given pattern

	// assemble components
	string password = "";
	for(int i = 0; i < pattern.length(); ++i)
	{
		string syllable;
		char number[16];
		switch(pattern[i])
		{
		case '_':
			password += symbols[GenerateRandom(numSymbols)];
			count *= numSymbols;
			break;
		case 'n':
			sprintf(number, "%01i", GenerateRandom(10));
			password += number;
			count *= 10;
			break;
		case 'S':
			syllable = consonants[GenerateRandom(numConsonants)];
			syllable += vowels[GenerateRandom(numVowels)];

			syllable[0] = toupper(syllable[0]);
			password += syllable;
			count *= numConsonants * numVowels;
			break;
		default:
			printf("Error:  unknown token %c\n", pattern[i]);
			return -1;
		}
	}

	printf("Password: %s\n", password.c_str());
	
	return count;
}

int main(int argc, char **argv)
{
	// print usage statement if argument list wasn't correct
	if(argc != 2)
	{
		int numSyllables = (sizeof(consonants) / sizeof(char *)) * 
			(sizeof(vowels) / sizeof(char *));
		int numSymbols = sizeof(symbols) / sizeof(char *);

		printf("Usage:  %s template\n", argv[0]);
		printf("where template consists of the following characters:\n");
		printf("S for a random syllable starting with a capital letter\n");
		printf("n for a random digit\n");
		printf("_ for a random symbol\n\n");
		printf("%2.1f bits of entropy per syllable, %2.1f per symbol, %2.1f per digit\n\n", 
			log10(numSyllables) * 3.32, log10(numSymbols) * 3.32, log10(10) * 3.32);
		return 0;
	}

	// list passwords for the user to choose from, capture entropy level for the pattern
	float count;
	for(int i = 0; i < 10; ++i)
		count = GeneratePassword(argv[1]);

//	printf("Number of possible passwords matching that pattern:  %3.10e\n", count);
	printf("Minimum bits of entropy: %3.2f\n", log10(count) * 3.32);	// log2(10) = 3.32

	return 0;
}

