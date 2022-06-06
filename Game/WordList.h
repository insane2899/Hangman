#ifndef WORDLIST_H
#define WORDLIST_H

#include <stdlib.h>

// A dynamically resizing array that stores multiple words of a fixed length
// The actual words are stored in one big array with null terminators
// separating the individual words

// Due to how this is used, we only need to support inserting words
// into the data structure, no need for deletion, which simplifies
// some things

_Pragma("GCC diagnostic push")
_Pragma("GCC diagnostic ignored \"-Wpadded\"")
typedef struct {
  size_t size; // Actual number of words
  size_t capacity;
  size_t word_length;
  char *data;
} WordList;
_Pragma("GCC diagnostic pop")

// Creates a WordList
WordList wordlist_init(size_t word_length);

// Create a WordList from a file
WordList wordlist_from_file(char *filename, size_t word_length);

// Cleanup the list when done
void wordlist_free(WordList *l);

// Prints out a WordList with commas separating the words
void wordlist_print(WordList l);

// Returns a pointer to word at index
char *wordlist_get(WordList l, size_t index);

// Selects a random word out of the list
char *wordlist_select_random(WordList l);

// Adds a word to the end of the list
void wordlist_append(WordList *l, char *word);


#endif
