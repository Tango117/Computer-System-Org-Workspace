#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"
#define SIZE 100

typedef struct Hashcell{
  char* word;
  struct Hashcell* next;
}HASHCELL;
HASHCELL hashmap[SIZE];

int hash_string(char* c){

  unsigned int hash = 1;
  for(int a = 0; a < strlen(c); a++){
    hash = (hash * 7) + c[a];
  }
  return hash % SIZE;
  
}

int insert_hash_cell(char* c){

  // find hash index
  unsigned int index = hash_string(c);
  // create temporary hash cell
  HASHCELL* dummyCell = (HASHCELL*) malloc(sizeof(HASHCELL));
  // create temporary word (+1 for 0 char at end of string)
  char* temp = (char*) malloc(strlen(c) + 1);
  // temporary cell word points to temp address
  dummyCell -> word = temp;
  // copy string c to temp, which temporary cell word points to
  strcpy(temp, c);
  // Check if cell in hashmap is empty
  if(hashmap[index].word == NULL){
    // Hashmap cell copies temporary hash cell
    hashmap[index] = *dummyCell;
    return 1;
  }
  else{
    // Iterate through linked list, check if temporary cell word is same as word in linked list
    HASHCELL *current = &hashmap[index];
    while(current -> next != NULL){
      // If same word, does not add to hashmap
      if(strcmp(c, current -> word) == 0){
        return 0;
      }
      current = current -> next;
    }
    if(strcmp(c, current -> word) == 0){
      return 0;
    }
    current -> next = dummyCell;
    return 1;
  }

}

void print_hash_table(){

  for(int i = 0; i < SIZE; i++){
    printf("%d:", i);
    if(hashmap[i].word == NULL){
      printf("\n");
      continue;
    }
    else{
      HASHCELL *current = &hashmap[i];
      while(current != NULL){
        printf(" %s", current -> word);
        current = current -> next;
      }
    }
    printf("\n");
  }

}

int main(){

  char str[100];
  while(scanf("%s", str) != EOF){
    insert_hash_cell(str);
  }
  // print_hash_table();

  for(int i = 0; i < SIZE; i++){
    if(hashmap[i].word == NULL){
      continue;
    }
    else{
      HASHCELL *current = &hashmap[i];
      while(current != NULL){
        insert_node(current -> word);
        current = current -> next;
      }
    }
  }

  print_tree(root);

}
