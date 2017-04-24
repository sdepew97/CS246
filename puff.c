#include <stdio.h>
#include <stdlib.h>
#include "puff.h"

/*

To decompress a file:

Read in the table of character frequencies in the first 256 ints in the input file.

Build the Huffman tree (as in steps 2-4 above).

For every bit read in from the input file, traverse down the Huffman tree (going left 
for 0 bits and right for 1 bits). When you reach a leaf, there will be a character stored in the leaf node. 
Write this character to the output file.

Repeat step 3 for every character in the input file (as determined by looking at the frequency 
of the root of the Huffman tree, which is necessarily the total number of characters in the input).

We’re done. Close the files and free your memory.

*/

int main(int argc, char *argv[]){
	
	//check there are enough input files
	if(argc!=3){
		printf("%d\n", argc); 
		printf("Sorry, there are not enough input files. Please use <program> <input file> <export file>.\n");
		return -1; 
	}
	printf("%s\n", argv[0]); 
	printf("%s\n", argv[1]);
	printf("%s\n", argv[2]);
	
	FILE *input = fopen(argv[1], "rb"); 
	FILE *output = fopen(argv[2], "w"); 
	
	//there was an error in opening the file
	if(input==NULL||output==NULL){
		printf("Sorry, there was an error with opening one of the files.\n"); 
		//fclose(input); 
		return -1; 
	}
	
	//Read in the table of character frequencies in the first 256 ints in the input file.
	int char_counts[NUMCHARS] = {}; 
	
	fread(char_counts, sizeof(int), NUMCHARS, input); 
	
	//check that char_counts is being filled with something :)
	for(int i=0; i<NUMCHARS; i++){
		printf("Character: %c Count: %d\n", i, char_counts[i]); 
	}
	
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
	printf("Number of leaves: %d\n", array_leaves_size); 
	
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
	
	//at this point, leaves has one element, size of 1, and this first element is a tree with all the nodes :)
	printf("Count: %d\n", leaves[0]->ch); 
	printf("Freq: %d\n", leaves[0]->freq); 
	
	//preorder and inorder traversals to check
	printf("Inorder: \n"); 
	inorder(leaves[0]); 
	printf("\nPreorder: \n"); 
	preorder(leaves[0]); 
	
	/*
	For every bit read in from the input file, traverse down the Huffman tree (going left 
	for 0 bits and right for 1 bits). When you reach a leaf, there will be a character stored in the leaf node. 
	Write this character to the output file.

	Repeat step 3 for every character in the input file (as determined by looking at the frequency 
	of the root of the Huffman tree, which is necessarily the total number of characters in the input).
	*/
	
	char current; 
	huff_tree_node *current_node = leaves[0]; 
	//rewind(input); 
	current = fgetc(input);
	for(int i=0; i<leaves[0]->freq; i++){
		while(current_node->ch<0 && current!=EOF){
			printf("%d\n", current_node->ch);
			  
			if(current=='0'){
				current_node = current_node->left;
				//current = fgetc(input);  
			}
		
			else if(current=='1'){
				current_node = current_node->right;
				//current = fgetc(input);  
			}
			current = fgetc(input);
		
		}
		fprintf(output,"%c", current_node->ch);
		printf("%c\n", current_node->ch);
		current_node = leaves[0]; //reset to start traversing tree, again and get next instruction :)
		//current = fgetc(input);
	}
	
	
	//close the files and free the memory
	free_memory(leaves[0]); 
}

//get the smallest leaf from the array and need to decrement size of array outside!! otherwise function malfunctions
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

// prints out all (string, int) pairs in the tree, doing an inorder traversal
void inorder(huff_tree_node* root)
{
  if(root)
  {
    inorder(root->left);
    printf("(%d,%d)\n", root->ch, root->freq);
    inorder(root->right);
  }
}

// prints out all (string, int) pairs in the tree, doing an inorder traversal
void preorder(huff_tree_node* root)
{
  if(root)
  {
    printf("(%d,%d)\n", root->ch, root->freq);
    preorder(root->left);
    preorder(root->right);
  }
}

//Frees all the memory used in this part of the program
void free_memory(huff_tree_node *root){
	free_huff_tree(root); 
}	