#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

NODE* root;

void rec_insert_node(NODE* n, NODE* r){
  
  if(strcmp(n -> word, r -> word) < 0){
    if(r -> left == NULL){
      r -> left = n;
    }else{
      rec_insert_node(n, r -> left);
    }
  }else{
    if(r -> right == NULL){
      r -> right = n;
    }else{
      rec_insert_node(n, r -> right);
    }
  }

}

void insert_node(char* c){

  NODE* newNode = (NODE*) malloc(sizeof(NODE));
  newNode -> word = c;
  if(root == NULL){
    root = newNode;
  }else{
    rec_insert_node(newNode, root);
  }

}

void print_tree(NODE* r){

  if(r == NULL){
    return;
  }else{
    if(r -> left != NULL){
      print_tree(r -> left);
    }
    printf("%s\n" , r -> word);
    if(r -> right != NULL){
      print_tree(r -> right);
    }
  }

}
