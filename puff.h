#define NUMCHARS 256
#include <stdlib.h>
#include <stdio.h>
#include "huff_tree_node.h"

huff_tree_node* get_smallest(huff_tree_node *leaves[NUMCHARS], int *array_size); 

void inorder(huff_tree_node* root); 

void preorder(huff_tree_node* root); 

void free_memory(huff_tree_node *root);  