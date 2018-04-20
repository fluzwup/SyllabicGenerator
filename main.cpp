#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <string>
#include <vector>
using namespace std;

/*
* Random password generator.  This generates passwords from a number, symbol, and some randomly selected syllables.  This should make the password
* relatively easy to remember (since it's largely pronouncable) but still fulfill the requirements for upper and lower case letters, numbers, and 
* symbols.  To make it simpler, I've left out the shuffle for now, so it's syllable, syllable, syllable, symbol, number.  There are 566 syllables,
* 24 symbols, and a three digit number, for a total of 566^3 * 24 * 1000 = 4.35e+12 (trillion) possible passwords.
*/

// syllable list munged from the following sources:
// 322 most common English syllables, derived form the 5000 most commonly used English words
// http://www.fldoe.org/core/fileparse.php/7539/urlt/manual.pdf pages 35-36
// 400+ nonsense syllables for speech therapy practice
// http://www.home-speech-home.com/nonsense-syllables.html

const char *syllables[] =
{
	"a", "ab", "abe", "ac", "ace", "ach", "ache", "act", "ad", "ade", "adj", "af", "afe", "ag", "age", "ah", 
	"ahg", "ahm", "ahp", "ahr", "ahs", "aht", "ahv", "aich", "aig", "aij", "aim", "ain", "air", "aish", "aith", "aizh", 
	"al", "ale", "all", "als", "am", "an", "ap", "ape", "ar", "as", "at", "ate", "ath", "au", "ave", "ay", 
	"aze", "azh", "ba", "bah", "bat", "bay", "be", "bee", "ber", "bet", "bi", "ble", "bles", "bod", "boh", "boo", 
	"bor", "but", "buy", "by", "ca", "cal", "can", "cap", "car", "cat", "cate", "cen", "cent", "chai", "char", "chaw", 
	"chay", "chee", "cho", "choo", "ci", "cial", "cir", "cit", "cle", "co", "col", "com", "come", "con", "cor", "coun", 
	"cov", "cu", "cul", "cus", "cy", "da", "dah", "dan", "day", "de", "dee", "den", "der", "ders", "di", "die", 
	"dif", "dis", "dle", "doh", "doo", "dy", "e", "ear", "east", "eat", "ed", "ee", "eeb", "eech", "eed", "eef", 
	"eeg", "eej", "eek", "eel", "eem", "een", "eep", "ees", "eesh", "eeth", "eev", "eez", "eezh", "ef", "el", "en", 
	"ence", "end", "ent", "er", "erd", "ern", "ers", "es", "est", "et", "ev", "eve", "ex", "eye", "fa", "fac", 
	"fah", "fay", "fect", "fee", "fer", "fi", "fie", "fin", "fish", "fix", "foh", "foo", "for", "form", "ful", "gah", 
	"gan", "gay", "gee", "gen", "ger", "gi", "gle", "go", "goh", "goo", "grand", "great", "guy", "ha", "hap", "har", 
	"head", "heav", "hee", "hey", "hi", "high", "ho", "hoe", "hunt", "i", "ibe", "ic", "ice", "ide", "ies", "ife", 
	"ike", "il", "ile", "im", "in", "ine", "ing", "ings", "ion", "ipe", "ire", "is", "ish", "it", "ite", "ith", 
	"ithe", "its", "ive", "ize", "jaw", "jay", "jee", "jie", "jo", "joh", "joo", "kah", "kay", "ket", "key", "kie", 
	"knee", "koh", "koo", "la", "lah", "land", "lands", "lar", "lat", "lay", "lead", "lec", "lect", "lee", "lent", "less", 
	"let", "li", "lie", "light", "lin", "lo", "long", "loo", "low", "lu", "ly", "ma", "mag", "mah", "main", "mal", 
	"man", "mar", "mat", "may", "me", "meas", "mee", "mem", "men", "ment", "ments", "mer", "mi", "mil", "min", "mis", 
	"mo", "mon", "moo", "moth", "mow", "mu", "mul", "my", "na", "nal", "naw", "nay", "near", "nel", "ner", "ness", 
	"net", "new", "ni", "ning", "no", "nore", "nt", "nu", "ny", "nye", "o", "oaf", "oak", "oat", "oav", "ob", 
	"oc", "ock", "odd", "ode", "of", "off", "oh", "ohb", "ohch", "ohg", "ohj", "ohl", "ohm", "ohs", "ohsh", "ohth", 
	"ohz", "ohzh", "on", "one", "oo", "oob", "ooch", "ood", "oof", "oog", "ooj", "ook", "ool", "oom", "oon", "oop", 
	"oor", "oos", "oosh", "oot", "ooth", "oov", "ooze", "oozh", "op", "ope", "or", "osh", "ous", "out", "own", "oz", 
	"pa", "pah", "par", "part", "pay", "pe", "pee", "pen", "per", "pi", "pie", "play", "ple", "ples", "ply", "po", 
	"poh", "point", "poo", "por", "port", "pos", "prac", "pre", "pres", "press", "pro", "ra", "rah", "ral", "ray", "re", 
	"read", "rec", "rect", "ree", "rep", "ri", "ried", "ro", "roh", "roo", "round", "row", "ry", "rye", "sa", "sand", 
	"sat", "saw", "say", "se", "sec", "see", "self", "sen", "sent", "ser", "set", "shaw", "shay", "she", "ship", "shoo", 
	"show", "shy", "si", "side", "sigh", "sim", "sion", "sions", "sis", "so", "some", "son", "sons", "south", "stand", "su", 
	"sub", "sue", "sug", "sun", "sup", "sur", "ta", "tah", "tain", "tal", "tay", "te", "ted", "tee", "tel", "tem", 
	"ten", "tend", "ter", "ters", "thaw", "thay", "the", "thee", "ther", "they", "thigh", "tho", "thoo", "though", "thy", "ti", 
	"tic", "tie", "ties", "tin", "ting", "tion", "tions", "tive", "tle", "to", "toe", "tom", "ton", "too", "tor", "tors", 
	"tra", "tract", "tray", "tri", "tro", "try", "tu", "ture", "tures", "ty", "u", "uer", "um", "un", "up", "us", 
	"va", "vah", "val", "var", "vay", "vee", "vel", "ven", "ver", "vi", "vid", "vie", "vis", "voh", "voo", "wah", 
	"ward", "way", "wee", "west", "where", "who", "whoa", "why", "wil", "win", "won", "woo", "work", "writ", "y", "yaw", 
	"yay", "yee", "yie", "yoh", "yoo", "zah", "zay", "zee", "zha", "zhaw", "zhay", "zhee", "zho", "zhoo", "zhy", "zoh", 
	"zoo", "zye" 
};

const char *symbols[] = 
{
	"!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "-", "+", "=", "{", "[", "}", "]", "|", ":", ";", "?", "/", "~"
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

void Shuffle(vector<int> &deck)
{
	int count = deck.size();

	// can't shuffle 1 or fewer cards
	if(count < 2) return;

	// if there are only 2, they can be swapped or not
	if(count == 2)
	{
		if(GenerateRandom(2) < 1)
		{	
			int temp = deck[0];
			deck[0] = deck[1];
			deck[1] = temp;
		}
		return;
	}

	// swap each card with a random other card
	for(int i = 0; i < count; ++i)
	{
		int j = GenerateRandom(count);
		if(j != i)
		{
			int temp = deck[i];
			deck[i] = deck[j];
			deck[j] = temp;
		}
	}

	return;
}

float GeneratePassword(string pattern)
{
	int numSyllables = sizeof(syllables) / sizeof(char *);
	int numSymbols = sizeof(symbols) / sizeof(char *);
	float count = 1.0; // count of possible passwords matching the given pattern

//	Shuffle(segments);

	// assemble components
	string password = "";
	for(int i = 0; i < pattern.length(); ++i)
	{
		string syllable;
		char number[16];
		switch(pattern[i])
		{
		case 's':
			syllable = syllables[GenerateRandom(numSyllables)];
			password += syllable;
			count *= numSyllables;
			break;
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
			syllable = syllables[GenerateRandom(numSyllables)];
			syllable[0] = toupper(syllable[0]);
			password += syllable;
			count *= numSyllables;
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
	if(argc != 2)
	{
		int numSyllables = sizeof(syllables) / sizeof(char *);
		int numSymbols = sizeof(symbols) / sizeof(char *);

		printf("Usage:  %s template\n", argv[0]);
		printf("where template consists of the following characters:\n");
		printf("s for a random syllable in lower case\n");
		printf("S for a random syllable starting with a capital letter\n");
		printf("n for a random digit\n");
		printf("_ for a random symbol\n\n");
		printf("%2.1f bits of entropy per syllable, %2.1f per symbol, %2.1f per digit\n\n", 
			log10(numSyllables) * 3.32, log10(numSymbols) * 3.32, log10(10) * 3.32);
		return 0;
	}

	float count;
	for(int i = 0; i < 10; ++i)
		count = GeneratePassword(argv[1]);

//	printf("Number of possible passwords matching that pattern:  %3.10e\n", count);
	printf("Minimum bits of entropy: %3.2f\n", log10(count) * 3.32);	// log2(10) = 3.32

	return 0;
}

