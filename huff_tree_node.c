#include <stdlib.h>

//Structure of a tree node that will populate the forest
typedef struct huff_tree_node {
  int ch;
  int freq;

  struct huff_tree_node* left;
  struct huff_tree_node* right;
} huff_tree_node;

//method to create a new node
huff_tree_node* new_leaf(int ch, int freq, huff_tree_node* left, huff_tree_node* right){
	huff_tree_node *new_node = malloc(sizeof(huff_tree_node)); 
	new_node->ch = ch; 
	new_node->freq = freq; 
	new_node->left = left; 
	new_node->right = right; 
	
	return new_node; 
}

void free_huff_tree(huff_tree_node* root){
	if(root){
		free_huff_tree(root->left); 
		free_huff_tree(root->right); 
		free(root); 
	}
} 