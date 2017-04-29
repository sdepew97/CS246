#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "huff.h"

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
	
	FILE *input = fopen(argv[1], "r"); 
	FILE *output = fopen(argv[2], "wb"); 
	
	//there was an error in opening the file
	if(input==NULL||output==NULL){
		printf("Sorry, there was an error with opening one of the files.\n");  
		return -1; 
	}
	
	//count the frequencies of the characters in the file and initialize all to zero
	int counts[NUMCHARS] = {}; 
	
	//get counts of characters in input file
	count_chars(counts, input); 
	
	huff_tree_node *leaves[NUMCHARS] = {};
	 
	//the number of leaves in the tree
	int array_leaves_size = 0; 
	
	//create the leaves and put them into an array data structure
	for(int i=0; i<NUMCHARS; i++){
		if(counts[i]>0){
			leaves[array_leaves_size] = new_leaf(i, counts[i], NULL, NULL); 
			array_leaves_size++; 
		}
	}
	
	//get smallest element and build the new tree while there is more than one node
	int num = -1; 
	
	//build the tree
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
	
	//get codes for each of the characters with count>0
	char_code *letter_codes[NUMCHARS];
	
	//fill letter_codes with something to point to 
	for(int i=0; i<NUMCHARS; i++){
		letter_codes[i] = malloc(sizeof(char_code));
		letter_codes[i]->code=0; 
		letter_codes[i]->significant=0;  
	}
	
	//get the codes for the tree
	codes_for_tree(leaves[0], 0, 0, letter_codes); 
	
	//start printing to output file
	fwrite(counts, sizeof(int), NUMCHARS, output); 
	
	//print the letter's codes from the input file to the output file
	int current; 
	rewind(input); 
	
 	int bits = 0; 
 	int start = 0; 
 	bits_writer values = {output, start, bits}; 
 	
	while((current = fgetc(input))!=EOF){
		
		char values_2[(letter_codes[current]->significant)+1]; 
		convert_char_binary_string(letter_codes[current]->code, letter_codes[current]->significant, values_2); 
		
		//at this point, values is storing a correct string copy of the huff code for a character
		for(int j=0; j<(letter_codes[current]->significant); j++){
			
			if(values_2[j]=='0'){
				put_bit(&values, false); 
			}
			else if(values_2[j]=='1'){
				put_bit(&values, true); 
			}
		}
		
	}
	
	//clear or "flush" the buffer if there is an incomplete byte still remaining complete it and put in the file
	while(values.num_bits_written!=1){
		put_bit(&values, false); 
	}

	
	//close files
	fclose(input); 
	fclose(output); 
	 
	//free memory
	free_memory(letter_codes, leaves[0]); 	
}

//Functions

//function that is used to count the number of characters 
void count_chars(int counts[NUMCHARS], FILE *input){

	int current_char; 
	
	while((current_char = fgetc(input)) != EOF){
		counts[current_char]++;  
	}
}

//get the smallest leaf from the array and shifts the array elements to maintain size and prevent issues with finding value, again
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

//gets the codes for the tree recursively and if it is found then updates the appropriate code information
void codes_for_tree(huff_tree_node *root, int val, int count, char_code *codes[NUMCHARS]){
	if(root){ 
		if(root->ch>=0){
			codes[root->ch]->code = val;
			codes[root->ch]->significant = count; 
		} 

		codes_for_tree(root->left, val<<1, count+1, codes);
		codes_for_tree(root->right, (val<<1)|1, count+1, codes);
	}
}

//converts an integer to the correct binary string representation with a null terminator; this function is used to obtain the Huffman code as a string: the code only begins with 1's so 0001 would be represented in int form as 1, so this function first fills the integer code and then pads with any missing leading zeros of the code. 
void convert_char_binary_string(int val, int significant, char values[significant]){ 
	int quotient = val; 
	int remainder = val%2; 
	int index = significant-1; 
	
	if(val==0&&significant==1){
		values[index] = 0+48;
		values[significant]='\0';
		return;   
	}

	while(quotient>0&&index>=0){
		values[index] = remainder+48;  
		quotient/=2; 
		remainder = quotient%2; 
		index--; 
	}
	
	if(quotient==0&&index!=-1){
		while(index!=-1){
			values[index] = 0+48; 
			index--; 
		}
	}
	
	values[significant] = '\0';
}

//function to put the bit in the byte: if there is a complete byte, then it puts the byte to the file and resets the values otherwise shifts, adds current bit, and adds to number of bits
void put_bit(bits_writer *bw, bool bit){
	if(bw->num_bits_written%8==0&&bw->num_bits_written>0){
		unsigned char byte = 0; 
		byte = byte|bw->bits_written; 
			
		fputc(byte, bw->file);
		
		bw->bits_written = 0; 
		bw->num_bits_written = 0; 
	}
	
	bw->bits_written = bw->bits_written<<1; 
	bw->bits_written = bw->bits_written|bit; 
	bw->num_bits_written+=1;
}

//Frees all the memory used in this part of the program by first freeing the tree and then freeing all the letter codes 
void free_memory(char_code *letter_codes[NUMCHARS], huff_tree_node *root){
	free_huff_tree(root); 
	for(int i=0; i<NUMCHARS; i++){
		free(letter_codes[i]); 	
	}
}

//counts the characters in a string and is used to test count function
void count_chars_string(int counts[NUMCHARS], char *input){

	int current_char; 
	int i = 0; 
	while((current_char = input[i]) != '\n'){
		counts[current_char]++; 
		i++; 
	}
}

int get_smallest_int(int leaves[], int *array_size){
//test code and logic for get smallest leaf
	//get smallest to return and shift over the values
	int smallest_leaf = leaves[0];
	int location_smallest = 0; 
	 
	for(int i=1; i<(*array_size); i++){
		if(leaves[i]<smallest_leaf){
			smallest_leaf = leaves[i]; 
			location_smallest = i;
		}
	}
	
	//shift over to remove this element
	for(int i=location_smallest; i<(*array_size)-1; i++){
		leaves[i] = leaves[i+1]; 
	}
	
	(*array_size)--; 
	return smallest_leaf; 
}