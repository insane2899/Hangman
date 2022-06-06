#ifndef WORDPARTITIONS_H
#define WORDPARTITIONS_H

#include <stdbool.h>

#include "WordList.h"

_Pragma("GCC diagnostic push")
_Pragma("GCC diagnostic ignored \"-Wpadded\"")
typedef struct wp_node_struct {
  char *key;
  WordList *value;
  
  bool red;

  struct wp_node_struct *parent;
  struct wp_node_struct *left;
  struct wp_node_struct *right;
  
} wp_node;
_Pragma("GCC diagnostic pop")


typedef struct {
  wp_node *root;
  wp_node *nil;
} WordListPartitions;

// Creates and empty red black tree
WordListPartitions *partitions_init(void);

// Frees up all memory
void partitions_free(WordListPartitions *t);

// Prints out a tree in DFS order
void partitions_print(WordListPartitions *t);

// Adds the (key, word) pair to the tree, appending to an existing
// word list if needed
void partitions_insert(WordListPartitions *t, char *key, char *word);

// Inserts into key and value, the pair that maximizes the word list size
void partitions_max_partition(WordListPartitions *t, char **key, WordList **value);

#endif
