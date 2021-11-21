#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define SIZE 100

// typedef struct Hashcell{
//   char *word;
//   struct Hashcell *next;
// }HASHCELL;
//
// HASHCELL hashmap[SIZE];
//
// struct cell {
//   int value;
//   struct cell *next;
// } CELL;
//
//
// struct book {
//     int bid;
//     char *bname;
// };
//
// int hash_string(char *c){
//   unsigned int hash = 1;
//   for(int a = 0; a < strlen(c); a++){
//     hash = (hash * 7) + c[a];
//   }
//   return hash % SIZE;
// }
// #define MASK 0x2


int main(){
    // int A[6] = {1000, 2, 3, 7, 50, 20};
    // int *p = &(A[5]);
    // printf("%d", -4(*p));
    // int x = 2;
    // printf("%d\n", x << 6);
    // int *p = malloc(1);
    // for(int i=0; i < 15; i++)
    //   p[i] = i;
    // for(int i=0; i < 15; i++){
    //   printf("%d\n", p[i]);
    // }

}

void g(int x, int y, int z, int *b) {
  int a[3];
  a[0] = x;
  a[1] = y;
  a[2] = z;
  memcpy(b,a,12);
}

// int factorial(int n){
//     if(n<=1)
//         return 1;
//     return n * factorial(n - 1);
// }
