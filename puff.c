#include <stdio.h>
#include <stdlib.h>
#include "puff.h"

/*

To decompress a file:

Read in the table of character frequencies in the first 256 ints in the input file.

Build the Huffman tree (as in steps 2-4 above).

For every bit read in from the input file, traverse down the Huffman tree (going left for 0 bits and right for 1 bits). When you reach a leaf, there will be a character stored in the leaf node. Write this character to the output file.

Repeat step 3 for every character in the input file (as determined by looking at the frequency of the root of the Huffman tree, which is necessarily the total number of characters in the input).

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
	
	for(int i=0; i<NUMCHARS; i++){
		printf("Character: %c Count: %d\n", i, char_counts[i]); 
	}
}
	