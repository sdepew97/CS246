#define NUMCHARS 256
#include <stdlib.h>
#include <stdio.h>
#include "huff_tree_node.h"

huff_tree_node* get_smallest(huff_tree_node *leaves[NUMCHARS], int *array_size); 

int get_bit(int *bits_read, int *current); 

void free_memory(huff_tree_node *root);  