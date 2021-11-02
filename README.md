# SyllabicGenerator
Program to generate passwords using random syllables, symbols, and numbers.
There are currently two versions:  
main.cpp uses a list of common English syllables plus some speec therapy syllables.
main2.cpp uses random combinations of consonant and vowel sounds from the 44 English phonemes to generate syllables.

PasswordGen.html is a JavaScript version.  It has been changed to use short 2 and 3 letter Scrabble ditionary words, rather than syllables, which keeps things more pronounceable.  JavaScript is handy because it runs in any browser, and it runs client-side, so the passwords never leave the browser's machine.  

The Excel spreadsheet contains counts of 2, 3, 4, 5, 6, and 7 letter words, plus the 7776 word Diceword list, and calculates the efficiency of various combinations in terms of expected entropy bits per character of passwords of various combinations.  The combination of 2 and 3 letter Scrabble words is the best combination checked, at 3.47 bits of entropy per character, which beats Dicewords list (average length 4.239 characters), which has 3.04 bits of entropy per character.  This is proof that, for a given number of characters in your password, you're better off with a larger number of short words than a smaller number of long words with the same letter count.  You're also better off than using random decimal digits, though random hex digits and random letters are more efficient, if harder to memorize.


