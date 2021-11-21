#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define READ_ENABLE_MASK 0x1
#define WRITE_ENABLE_MASK 0x2

struct Node{
  int data;
  struct Node* next;
};

uint32_t *main_memory;

int main() {
  // struct Node* head = NULL;
  // head = (struct Node*) malloc(sizeof(struct Node));
  // struct Node* second = malloc(sizeof(struct Node));
  // struct Node* third = malloc(sizeof(struct Node));
  // head->data = 1;
  // head->next = second;
  // second->data = 2;
  // second->next = third;
  // third->data = 3;
  // third->next = NULL;
  // printf("%d\n",head->next->data);
  // int x = 131072;
  // for (int i = 0; i < 8; i++){
  //   x = x >> 2;
  //   printf("%d\n", x);
  // }
  if((0)){
    printf("0\n" );
  }
  if((1)){
    printf("1\n" );
  }
  if(!(5424323343)){
    printf("24323343\n");
  }
  int x = 2;
  int* y = &x;
  *y = *y | 1;
  printf("%d\n", *y);
}

// uint32_t *main_memory;
//
// uint32_t main_memory_size_in_bytes;
//
// void main_memory_initialize(uint32_t size_in_bytes)
// {
//
//   //Check if size in bytes is divisible by 32.
//   if (size_in_bytes & 0x1F) { //lowest 5 bits should be 00000
//     printf("Error: Memory size (in bytes) must be a multiple of 8-word cache lines (32 bytes)\n");
//     exit(1);
//   }
//
//   //Allocate the main memory, using malloc
//
//   main_memory_size_in_bytes = size_in_bytes;
//   main_memory = (uint32_t*) malloc(main_memory_size_in_bytes);
//
//   //Write a 0 to each word in main memory. Note that the
//   //size_in_bytes parameter specifies the size of main memory
//   //in bytes, but, since main_memory is declared as an
//   //array of 32-bit words, it is written to a word at a time
//   //(not a byte at a time). Obviously, the size of main memory
//   //in words is 1/4 of the size of main memory in bytes.
//
//   for(int i = 0; i < main_memory_size_in_bytes; i += 4){
//     *(main_memory + i) = 0;
//   }
//
// }
//
// #define CACHE_LINE_ADDRESS_MASK ~0x1f
//
// void main_memory_access(uint32_t address, uint32_t write_data[],
//                         uint8_t control, uint32_t read_data[])
// {
//
//   //Need to check that the specified address is within the
//   //size of the memory. If not, print an error message and
//   //exit from the program.
//   uint32_t *p_address = (uint32_t*) address;
//   if(*(p_address) < *(main_memory) && *(p_address) >= *(main_memory + main_memory_size_in_bytes)){
//     printf("Error: Specified address (%u) must be within size of memory (%u)\n", address, main_memory_size_in_bytes);
//     exit(1);
//   }
//
//   //Determine the address of the start of the desired cache line.
//   //Use CACHE_LINE_ADDRESS_MASK to mask out the appropriate
//   //number of low bits of the address.
//
//   uint32_t cache_line_start = address & CACHE_LINE_ADDRESS_MASK;
//   uint32_t *cache_line_address = (uint32_t*) cache_line_start;
//
//   //If the read-enable bit of the control parameter is set (i.e. is 1),
//   //then copy the cache line starting at cache_line_address into read_data.
//   //See memory_subsystem_constants.h for masks that are convenient for
//   //testing the bits of the control parameter.
//
//   if(control & READ_ENABLE_MASK){
//     for(int i = 0; i < 8; i++){
//       read_data[i] = *(cache_line_address + (i * 4));
//     }
//   }
//
//   //If the write-enable bit of the control parameter is set then copy
//   //write_data into the cache line starting at cache_line_address.
//
//   if(control & WRITE_ENABLE_MASK){
//     for(int i = 0; i < 8; i++){
//       *(cache_line_address + (i * 4)) = write_data[i];
//     }
//   }
// }
