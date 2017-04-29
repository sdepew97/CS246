#define NUMCHARS 256
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "huff_tree_node.h"

/*
Name: Sarah Depew
Resources: Professor, TA, Kennedy, Trista, Ruby, Sarah K., Amanda, and textbook (I know I did some web research, but I do not distinctly remember implementing details from the resources, but I acknowledge them anyway)
*/

void count_chars(int counts[NUMCHARS], FILE *input); 

void count_chars_string(int counts[NUMCHARS], char *input); 

huff_tree_node* get_smallest(huff_tree_node *leaves[NUMCHARS], int *array_size); 

char *get_code(char searching, int tree_height, huff_tree_node *root); 

typedef struct char_code {
  		int code;
  		int significant;
} char_code;
	
void codes_for_tree(huff_tree_node *root, int val, int count, char_code *codes[NUMCHARS]);

int print_binary(int val); 

void convert_char_binary_string(int val, int significant, char values[significant]); 

//gotten from board in office hours
typedef struct bits_writer{
	FILE *file; 
	int num_bits_written; 
	int bits_written; 
} bits_writer;

//gotten from board in office hours
void put_bit(bits_writer *bw, bool bit);

void free_memory(char_code *letter_codes[NUMCHARS], huff_tree_node *root);  

int get_smallest_int(int leaves[], int *array_size); 