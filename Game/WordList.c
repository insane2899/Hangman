#include "WordList.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Creates a WordList
WordList wordlist_init(size_t word_length) {
  WordList ret;

  ret.word_length = word_length;

  // Start with capacity of 4 words
  ret.size = 0;
  ret.capacity = 4;
  // The +1 is for the null terminator
  ret.data = malloc((ret.word_length + 1) * ret.capacity);

  return ret;
}


// Create a WordList from a file
WordList wordlist_from_file(char *filename, size_t word_length) {
  FILE *input = fopen(filename, "r");
  if (input == NULL) {
    perror("fopen() of dictionary file failed");
    exit(1);
  }

  WordList list = wordlist_init(word_length);

  // Read file word by word
  char buffer[40];
  while (fscanf(input, "%s", buffer) != EOF) {
    if (strlen(buffer) == word_length) {
      wordlist_append(&list, buffer);
    }
  }

  fclose(input);

  return list;
}

// Cleanup the list when done
void wordlist_free(WordList *l) {
  free(l->data);
  l->data = NULL;
  l->size = 0;
  l->capacity = 0;
  l->word_length = 0;
}



// Prints out a WordList with commas separating the words
void wordlist_print(WordList l) {
  for (size_t i = 0; i < l.size; ++i) {
    printf("%s, ", wordlist_get(l, i));
  }
}


// Returns a pointer to word at index
char *wordlist_get(WordList l, size_t index) {
  return l.data + (l.word_length + 1) * index;
}

// Resizes the list to be bigger, should not be externally called
static void wordlist_resize(WordList *l) {
  // Calculate new capacity
  // Use growth factor of 2
  l->capacity *= 2;

  // Store old pointer and allocate new space
  char *old = l->data;
  l->data = malloc((l->word_length + 1) * l->capacity);

  // Copy old values
  memcpy(l->data, old, (l->word_length + 1) * l->size);

  // Cleanup the old
  free(old);
}

// Selects a random word out of the list
char *wordlist_select_random(WordList l) {
  // Should make this not use modulo bias
  size_t choice = (size_t) rand() % l.size;
  
  return wordlist_get(l, choice);
}



// Adds a word to the end of the list
void wordlist_append(WordList *l, char *word) {
  // Ensure space exists
  if (l->size == l->capacity) {
    // At capacity, resize
    wordlist_resize(l);
  }

  // Copy the string to the correct location
  strcpy(l->data + (l->word_length + 1) * l->size, word);
  ++l->size;
}

