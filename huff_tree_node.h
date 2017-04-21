#include <stdlib.h>

//Structure of a tree node that will populate the forest
typedef struct huff_tree_node {
  int ch;
  int freq;

  struct huff_tree_node* left;
  struct huff_tree_node* right;
} huff_tree_node;

//method to create a new node
huff_tree_node* new_leaf(int ch, int freq, huff_tree_node* left, huff_tree_node* right); 

void free_huff_tree(huff_tree_node* root); 