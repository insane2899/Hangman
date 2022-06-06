#include "WordPartitions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "WordList.h"

// Creates and empty red black tree
WordListPartitions *partitions_init(void){
  // Allocate everything
  WordListPartitions *ret = malloc(sizeof(WordListPartitions));
  ret->nil = malloc(sizeof(wp_node));
  ret->root = malloc(sizeof(wp_node));

  // Setup nil
  ret->nil->key = NULL;
  ret->nil->value = NULL;
  ret->nil->red = false;

  ret->nil->parent = ret->nil;
  ret->nil->left = ret->nil;
  ret->nil->right = ret->nil;

  // Setup root
  ret->root->key = NULL;
  ret->root->value = NULL;
  ret->root->red = false;

  ret->root->parent = ret->nil;
  ret->root->left = ret->nil;
  ret->root->right = ret->nil;

  return ret;
}

static void rb_free_dfs(wp_node *n, wp_node *nil) {
  // Basic post order traversal
  if (n == nil) {
    return;
  }

  if (n->left != nil) {
    rb_free_dfs(n->left, nil);
  }
  if (n->right != nil) {
    rb_free_dfs(n->right, nil);
  }

  free(n);
}

void partitions_free(WordListPartitions *t) {
  rb_free_dfs(t->root->left, t->nil);

  free(t->root);
  free(t->nil);
}

static void rb_print_dfs(wp_node *n, wp_node *nil) {
  // Basic pre-order DFS traversal
  if (n == nil) {
    // Hit a leaf
    return;
  }

  
  printf("%s: ", n->key);
  wordlist_print(*n->value);
  printf("\n");
  
  if (n->left != nil) {
    rb_print_dfs(n->left, nil);
  }
  if (n->right != nil) {
    rb_print_dfs(n->right, nil);
  }
}

// Prints out a tree in DFS order
void partitions_print(WordListPartitions *t) {
  rb_print_dfs(t->root->left, t->nil);
}

/*
// Following pseudocode from Introduction to Algorithms by Cormen et al 4th ed
static void rb_left_rotate(WordListPartitions *t, wp_node *x) {
  wp_node *y = x->right;

  // Turn y's left subtree into x's right subtree
  x->right = y->left;

  // If y's left subtree is not empty
  if (y->left != t->nil) {
    // x becomes the parent of the subtrees root
    y->left->parent = x;
  }

  // x's parent becomes y's parent
  y->parent = x->parent;

  
  // instead of checking if x->parent is the root as in the book, we 
  // count on the root sentinel to implicitly take care of this case
  // If x was the root
  if (x == x->parent->left) {
    // x was a left child
    // then y becomes that child
    x->parent->left = y;
  } else {
    // x was a right while, now y is
    x->parent->right = y;
  }

  // Make x become y's child
  y->left = x;
  x->parent = y;
}
static void rb_right_rotate(WordListPartitions *t, wp_node *y) {
  // Copy of above swapping all occurrences of left/right
  wp_node *x = y->left;
  y->left = x->right;

  if (x->right != t->nil) {
    x->right->parent = y;
  }
  x->parent = y->parent;
  
  if (y == y->parent->left) {
    y->parent->left = x;
  } else {
    y->parent->right = x;
  }
  x->right = y;
  y->parent = x;
}



static void rb_insert_fixup(WordListPartitions *t, wp_node *z) {
  // Following pseudocode from Cormen et al
  while (z->parent->red) {
    if (z->parent == z->parent->parent->left) {
      wp_node *y = z->parent->parent->right;
      if (y->red) {
        z->parent->red = false;
        y->red = false;
        z->parent->parent->red = true;
        z = z->parent->parent;
      } else {
        if (z == z->parent->right) {
          z = z->parent;
          rb_left_rotate(t, z);
        }
        z->parent->red = false;
        z->parent->parent->red = true;
        rb_right_rotate(t, z->parent->parent);
      }
    } else {
      // Same as true branch swapping left/right
      wp_node *y = z->parent->parent->left;
      if (y->red) {
        z->parent->red = false;
        y->red = false;
        z->parent->parent->red = true;
        z = z->parent->parent;
      } else {
        if (z == z->parent->left) {
          z = z->parent;
          rb_right_rotate(t, z);
        }
        z->parent->red = false;
        z->parent->parent->red = true;
        rb_left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->red = false;
}
*/

void partitions_insert(WordListPartitions *t, char *key, char *word) {
  // Following pseudocode from Cormen et al
  wp_node *x = t->root;
  wp_node *y = t->nil;

  // Descend tree looking for either node to add to or
  // nil indicating a new key
  while (x != t->nil) {
    y = x;
    int cmp = (x->key == NULL) ? -1 : strcmp(key, x->key);
    
    if (cmp < 0) {
      x = x->left;
    } else if (cmp > 0) {
      x = x->right;
    } else {
      // Found node with matching key, add to it
      // and stop
      wordlist_append(x->value, word);
      return;
    }
  }

  // Create a new node
  wp_node *new_node = malloc(sizeof(wp_node));

  // Setup key/value
  new_node->key = malloc(strlen(key) + 1);
  strcpy(new_node->key, key);
  new_node->value = malloc(sizeof(WordList));
  *new_node->value = wordlist_init(strlen(word));
  wordlist_append(new_node->value, word);

  // New nodes are red
  new_node->red = true;

  // Links to other nodes
  new_node->parent = y;
  new_node->left = t->nil;
  new_node->right = t->nil;

  // Link new node into existing tree as child of y
  // If tree was empty
  if (y == t->nil) {
    //printf("New node key %s is root\n", key);
    t->root = new_node;
  } else if (((y->key == NULL) ?  -1 : strcmp(key, y->key)) < 0) {
    y->left = new_node;
  } else {
    y->right = new_node;
  }

  // Correct any violations of red-black properties
  //rb_insert_fixup(t, new_node);
}


static size_t rb_max_partition_dfs(wp_node *n, wp_node *nil, char **key, WordList **value) {
  // Basic pre-order DFS traversal
  if (n == nil) {
    // Hit a leaf
    return 0;
  }

  char *left_key = NULL;
  WordList *left_value = NULL;
  size_t left_size = 0;
  if (n->left != nil) {
    left_size = rb_max_partition_dfs(n->left, nil, &left_key, &left_value);
  }

  char *right_key = NULL;
  WordList *right_value = NULL;
  size_t right_size = 0;
  if (n->right != nil) {
    right_size = rb_max_partition_dfs(n->right, nil, &right_key, &right_value);
  }

  // Find max, priority is current node, left child, right child
  if (n->value->size > left_size && n->value->size > right_size) {
    *key = n->key;
    *value = n->value;
    return n->value->size;
  }

  if (left_size > right_size) {
    *key = left_key;
    *value = left_value;
    return left_size;
  }

  *key = right_key;
  *value = right_value;
  return right_size;
}


void partitions_max_partition(WordListPartitions *t, char **key, WordList **value) {
  rb_max_partition_dfs(t->root->left, t->nil, key, value);
}
