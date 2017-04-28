#define NUMCHARS 256
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "huff_tree_node.h"

/*
Name: 
Resources: 
*/

void count_chars(int counts[NUMCHARS], FILE *input); 

void count_chars_string(int counts[NUMCHARS], char *input); 

huff_tree_node* get_smallest(huff_tree_node *leaves[NUMCHARS], int *array_size); 

//taken from tree_node.h
// gets the height of the tree. That is, this returns the maximum number
// of nodes to get from the root to a leaf. An empty tree has height 0,
// whereas a tree with one node as a height of 1.
int height(huff_tree_node* root); 

char *get_code(char searching, int tree_height, huff_tree_node *root); 

typedef struct char_code {
  		int code;
  		int significant;
} char_code;
	
void codes_for_tree(huff_tree_node *root, int val, int count, char_code *codes[NUMCHARS]);

int print_binary(int val); 

void convert_char_binary_string(int val, int significant, char values[significant]); 

/*
typedef struct bits_writer{
	FILE *file; 
	int num_bits_written; 
	int *bits_written; 
} bits_writer; 
*/

//gotten from board in office hours
typedef struct bits_writer{
	FILE *file; 
	int num_bits_written; 
	int bits_written; 
} bits_writer;

//gotten from board in office hours
void put_bit(bits_writer *bw, bool bit);

void free_memory(char_code *letter_codes[NUMCHARS], huff_tree_node *root);  