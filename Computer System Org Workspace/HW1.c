#include <stdio.h>
#include <stdlib.h>

typedef struct cell {
 int val;
 struct cell *next;
} CELL;

typedef struct node {
  int value;
  struct node *left;
  struct node *right;
} NODE;

// void delete_mid(CELL* p){
//   p -> prev ->next = p -> next;
//   p -> next -> prev = p -> prev;
// }

void print_linkedlist(CELL *p){
  while(p -> next != NULL){
    printf("%d,", p -> val);
    p = p -> next;
  }
  printf("%d\n", p -> val);
}

// NODE *new_node(int val)
// {
//   NODE n;
//   n.left = NULL;
//   n.right = NULL;
//   n.value = val;
//   return &n;
// }

NODE *new_new_node(int val)
{
  NODE* n = (NODE*) malloc(sizeof(NODE));
  n -> left = NULL;
  n -> right = NULL;
  n -> value = val;
  return n;
}

unsigned int rotate(unsigned int num, unsigned int n)
{
  return (num >> n)|(num << (32 - n));
}

CELL *rev_copy(CELL *h)
{
  CELL *res = NULL;
  while (h != NULL) {
    CELL *temp = (CELL*)malloc(sizeof(CELL));
    temp->val = h -> val;
    temp->next = res;
    res = temp;
    h = h -> next;
  }
  return res;
}

int greater_or_equal_to_2(float f) {
  unsigned int x = *((unsigned int *) &f);
  unsigned int sign = (x >> 31) & 1;
  unsigned int actual_exp = ((x >> 23) & 0xFF) - 127;
  unsigned int mantissa = x & 0x7FFFFF;
  printf("%u %u %u\n", sign, actual_exp, mantissa);
  return (sign == 1) || actual_exp > 1;
}

int main()
{
  // float f = -321.43321452;
  // unsigned int x = *((unsigned int*) &f);
  // printf("%u\n", x);
  // CELL* head = (CELL*) malloc(sizeof(CELL));
  // head -> val = 1;
  // CELL* current = head;
  // for(int i = 2; i < 11; i++){
  //   CELL* temp = (CELL*) malloc(sizeof(CELL));
  //   temp -> val = i;
  //   current -> next = temp;
  //   current = temp;
  // }
  // printf("%d", head -> val);
  // print_linkedlist(head);
  // CELL* head2 = rev_copy(head);
  // print_linkedlist(head2);
  float f = 2;
  printf("%d", greater_or_equal_to_2(f));
}
