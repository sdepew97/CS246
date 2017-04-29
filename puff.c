#include <stdio.h>
#include <stdlib.h>
#include "puff.h"

/*
Name: Sarah Depew
Resources: Professor, TA, Kennedy, Trista, Ruby, Sarah K., Amanda, and textbook (I know I did some web research, but I do not distinctly remember implementing details from the resources, but I acknowledge them anyway)
*/

int main(int argc, char *argv[]){
	
	//check there are enough input files
	if(argc!=3){
		printf("Sorry, there are not enough input files. Please use <program> <input file> <export file>.\n");
		return -1; 
	}
	
	FILE *input = fopen(argv[1], "rb"); 
	FILE *output = fopen(argv[2], "w"); 
	
	//there was an error in opening the file
	if(input==NULL||output==NULL){
		printf("Sorry, there was an error with opening one of the files.\n"); 
		return -1; 
	}
	
	//Read in the table of character frequencies in the first 256 ints in the input file.
	int char_counts[NUMCHARS] = {}; 
	
	fread(char_counts, sizeof(int), NUMCHARS, input); 
	
	//Build the Huffman tree (as in steps 2-4 above)
	//array of leaves
	huff_tree_node *leaves[NUMCHARS] = {};
	 
	//the number of leaves in the tree
	int array_leaves_size = 0; 
	
	for(int i=0; i<NUMCHARS; i++){
		if(char_counts[i]>0){
			//create a tree nodes and put into the array
			leaves[array_leaves_size] = new_leaf(i, char_counts[i], NULL, NULL); 
			array_leaves_size++; 
		}
	}
	
	//get smallest element and build the new tree while there is more than one node
	int num = -1; 
	while(array_leaves_size>1){
		//get two smallest nodes and construct tree
		huff_tree_node *x = get_smallest(leaves, &array_leaves_size); 
		huff_tree_node *y = get_smallest(leaves, &array_leaves_size);
		huff_tree_node *n = new_leaf(num, x->freq+y->freq, x, y); 
		
		//add the new huff_tree to the array 
		leaves[array_leaves_size] = n; 
		
		array_leaves_size++; 		
		num--; 
	}
	
	//read the characters from the input file 
		
	//current character read from the file :)
	int current; 
	int chars_read = 0; 
	huff_tree_node *current_node = leaves[0]; 
	int values_read = 0;
	int num_bits_gotten = 0; 
	
	current = fgetc(input);
	
	//read and print out the codes
	while(current!=EOF&&chars_read<leaves[0]->freq){
		for(int i=0; i<8&&chars_read<leaves[0]->freq; i++){
			int current_bit = get_bit(&values_read, &current);
			
			num_bits_gotten++; 
		
			if(current_bit==0){
				current_node = current_node->left;
			}
			
			else if(current_bit==1){
				current_node = current_node->right;
			}
			
			if(current_node->ch>=0){
				fprintf(output,"%c", current_node->ch);
				current_node = leaves[0]; //reset to start traversing tree, again and get next instruction 
				chars_read++; 
			}
		}
		values_read = 0;  
		current = fgetc(input);
	}
		
	//close the files and free the memory
	free_memory(leaves[0]); 
}

//get the smallest leaf from the array and need to decrement size of array outside
huff_tree_node* get_smallest(huff_tree_node *leaves[NUMCHARS], int *array_size){
	//get smallest to return and shift over the values
	huff_tree_node *smallest_leaf = leaves[0];
	int location_smallest = 0; 
	 
	for(int i=1; i<(*array_size); i++){
		if(leaves[i]->freq<smallest_leaf->freq){
			smallest_leaf = leaves[i]; 
			location_smallest = i;
		}
		//break the tie with the character
		else if(leaves[i]->freq==smallest_leaf->freq){
			if(leaves[i]->ch<smallest_leaf->ch){
				smallest_leaf = leaves[i]; 
				location_smallest = i;
			}	
		}
	}
	
	//shift over to remove this element
	for(int i=location_smallest; i<(*array_size)-1; i++){
		leaves[i] = leaves[i+1]; 
	}
	
	(*array_size)--; 
	return smallest_leaf; 
}

//function to get the bit from the current character gotten from the file
int get_bit(int *bits_read, int *current){
	unsigned char return_value = *current;
	 
	return_value = *current<<(*bits_read); 
	return_value = return_value>>(7); 
	*bits_read = (*bits_read)+=1; 

	return return_value; 
}

//Frees all the memory used in this part of the program
void free_memory(huff_tree_node *root){
	free_huff_tree(root); 
}	