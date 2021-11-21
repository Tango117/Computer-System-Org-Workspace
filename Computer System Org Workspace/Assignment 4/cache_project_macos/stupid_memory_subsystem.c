#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "memory_subsystem_constants.h"

uint32_t *main_memory;

uint32_t main_memory_size_in_bytes;


/************************************************************************
                 main_memory_initialize
This procedure allocates main memory, according to the size specified in bytes.
The procedure should check to make sure that the size is a multiple of 32 (since
there are 4 bytes per word and 8 words per cache line).
*************************************************************************/

void main_memory_initialize(uint32_t size_in_bytes)
{

  //Check if size in bytes is divisible by 32.
  if (size_in_bytes & 0x1F) { //lowest 5 bits should be 00000
    printf("Error: Memory size (in bytes) must be a multiple of 8-word cache lines (32 bytes)\n");
    exit(1);
  }

  //Allocate the main memory, using malloc
  main_memory_size_in_bytes = size_in_bytes;
  main_memory = (uint32_t*) malloc(main_memory_size_in_bytes);
  //Write a 0 to each word in main memory. Note that the
  //size_in_bytes parameter specifies the size of main memory
  //in bytes, but, since main_memory is declared as an
  //array of 32-bit words, it is written to a word at a time
  //(not a byte at a time). Obviously, the size of main memory
  //in words is 1/4 of the size of main memory in bytes.
  for(int i = 0; i < (main_memory_size_in_bytes >> 2); i++){
    main_memory[i] = 0;
  }
}


//zeroing out the lowest five bits of an address indicates
//the address of the start of the corresponding cache line
//in memory. Use this mask to zero out the lowest 5 bits,
//since 1F hex = 000...0011111 binary, and using ~ to flip the bits
//gives 111...1100000 in binary.
#define CACHE_LINE_ADDRESS_MASK ~0x1f


/********************************************************************
               main_memory_access

This procedure implements the reading and writing of cache lines from
and to main memory. The parameters are:

address:  unsigned 32-bit address. This address can be anywhere within
          a cache line.

write_data:  an array of unsigned 32-bit words. On a write operation,
             8 words are copied from write_data to the appropriate cache
             line in memory.

control:  an unsigned byte (8 bits), of which only the two lowest bits
          are meaningful, as follows:
          -- bit 0:  read enable (1 means read, 0 means don't read)
          -- bit 1:  write enable (1 means write, 0 means don't write)

read_data: an array of unsigned 32-bit integers. On a write operation,
           8 32-bit words are copied from the appropriate cache line in
           memory to read_data.

*********************************************************/

void main_memory_access(uint32_t address, uint32_t write_data[],
                        uint8_t control, uint32_t read_data[])

{

  //Need to check that the specified address is within the
  //size of the memory. If not, print an error message and
  //exit from the program.
  if(address >= main_memory_size_in_bytes){
    printf("Error: Specified address (%u) must be within size of memory (%u)\n", address, main_memory_size_in_bytes);
    exit(1);
  }

  //Determine the address of the start of the desired cache line.
  //Use CACHE_LINE_ADDRESS_MASK to mask out the appropriate
  //number of low bits of the address.

  uint32_t cache_line_address = address & CACHE_LINE_ADDRESS_MASK;

  //If the read-enable bit of the control parameter is set (i.e. is 1),
  //then copy the cache line starting at cache_line_address into read_data.
  //See memory_subsystem_constants.h for masks that are convenient for
  //testing the bits of the control parameter.

  if(control & READ_ENABLE_MASK){
    for(int i = 0; i < 8; i++){
      read_data[i] = main_memory[(cache_line_address >> 2) + i];
    }
  }

  //If the write-enable bit of the control parameter is set then copy
  //write_data into the cache line starting at cache_line_address.

  if(control & WRITE_ENABLE_MASK){
    for(int i = 0; i < 8; i++){
      main_memory[(cache_line_address >> 2) + i] = write_data[i];
    }
  }

}

#define L2_NUM_CACHE_ENTRIES (1<<15)


/***************************************************
 This struct defines the structure of a single cache
 entry in the L2 cache. It has the following fields:
   v_d_tag: 32-bit unsigned word containing the
            valid (v) bit at bit 31 (leftmost bit),
            the dirty bit (d) at bit 30, and the tag
            in bits 0 through 15 (the 16 rightmost bits)
  cache_line: an array of 8 words, constituting a single
              cache line.
****************************************************/
typedef struct {
  uint32_t v_d_tag;
  uint32_t cache_line[WORDS_PER_CACHE_LINE];
} L2_CACHE_ENTRY;



//The valid bit is bit 31 (leftmost bit) of v_d_tag word
//The mask is 1 shifted left by 31
#define L2_VBIT_MASK (0x1 << 31)

//The dirty bit is bit 30 (second to leftmost bit) of v_d_tag word
//The mask is 1 shifted left by 30
#define L2_DIRTYBIT_MASK (0x1 << 30)

//The tag is lowest 12 bits of v_d_tag word, so the mask is FFF hex
#define L2_ENTRY_TAG_MASK 0xfff

// The L2 is just an array off cache entries

L2_CACHE_ENTRY l2_cache[L2_NUM_CACHE_ENTRIES];

// Note that since l2_cache is an array of L2_CACHE_ENTRY structs,
// not an array of pointers, you would NOT use "->" to access the
// a field of an entry. For example, you could write
// l2_cache[i].v_d_tag, but NOT l2_cache[i]->v_d_tag.


/************************************************
            l2_initialize()

This procedure initializes the L2 cache by clearing
(i.e. setting to 0) the valid bit of each cache entry.
************************************************/

void l2_initialize()
{

  for(int i = 0; i < L2_NUM_CACHE_ENTRIES; i++){
    l2_cache[i].v_d_tag = l2_cache[i].v_d_tag & (~L2_VBIT_MASK);
  }

}


//The upper 12 bits (bits 20-31) of an address are used as the tag bits.
//So the mask is 12 ones (so FFF hex) shifted left by 20 bits.
#define L2_ADDRESS_TAG_MASK (0xfff << 20)
#define L2_ADDRESS_TAG_SHIFT 20

//Bits 5-19 (so 15 bits in total) of an address specifies the index of the
//cache line within the L2 cache.
//The value of the mask is 15 ones (so 7FFF hex) shifted left by 5 bits
#define L2_INDEX_MASK (0x7fff << 5)
#define L2_INDEX_SHIFT 5


#define L2_HIT_STATUS_MASK 0x1

/****************************************************

          l2_cache_access()

This procedure implements the reading and writing of cache lines from
and to the L2 cache. The parameters are:

address:  unsigned 32-bit address. This address can be anywhere within
          a cache line.

write_data:  an array of unsigned 32-bit words. On a write operation,
             if there is a cache hit, 8 32-bit words are copied from this
             array to the appropriate cache line in the L2 cache.

control:  an unsigned byte (8 bits), of which only the two lowest bits
          are meaningful, as follows:
          -- bit 0:  read enable (1 means read, 0 means don't read)
          -- bit 1:  write enable (1 means write, 0 means don't write)

read_data: an array of unsigned 32-bit integers. On a read operation,
           if there is a cache hit, 8 32-bit words are copied from the
           appropriate cache line in the L2 cache to this array.

status: this in an 8-bit output parameter (thus, a pointer to it is
        passed).  The lowest bit of this byte should be set to
        indicate whether a cache hit occurred or not:
              cache hit: bit 0 of status = 1
              cache miss: bit 0 of status = 0

If the access results in a cache miss, then the only
effect is to set the lowest bit of the status byte to 0.

**************************************************/

void l2_cache_access(uint32_t address, uint32_t write_data[],
                     uint8_t control, uint32_t read_data[], uint8_t *status)
{

  //Extract from the address the index of the cache entry in the cache.
  //Use the L2_INDEX_MASK mask to mask out the appropriate
  //bits of the address and L2_INDEX_SHIFT
  //to shift the appropriate amount.

  int index = (address & L2_INDEX_MASK) >> L2_INDEX_SHIFT;

  //Extract from the address the tag bits.
  //Use the L2_ADDRESS_TAG_MASK mask to mask out the appropriate
  //bits of the address and L2_ADDRESS_TAG_SHIFT
  //to shift the appropriate amount.

  int tag = (address & L2_ADDRESS_TAG_MASK) >> L2_ADDRESS_TAG_SHIFT;

  //if the selected cache entry has a zero valid bit or
  //if the entry's tag does not match the tag bits of
  //the address, then it is a cache miss: Set the
  //low bit of the status byte appropriately.

  if(!(l2_cache[index].v_d_tag & L2_VBIT_MASK) || ((l2_cache[index].v_d_tag & L2_ENTRY_TAG_MASK) != tag)){
    *status = *status & 0x0;
  }

  //Otherwise, it's a cache hit:
  //If the read-enable bit of the control parameter is set (i.e. is 1)
  //then copy the cache line data in the cache entry into the read_data
  //array. If the write-enable bit of the control parameter is set, then
  //copy the data in the write_data array into the cache line starting at
  //the cache line index and set the dirty bit. Set the low bit
  //of the status byte appropriately.

  else{
    if(control & 0x1){
      for(int i = 0; i < WORDS_PER_CACHE_LINE; i++){
        read_data[i] = l2_cache[index].cache_line[i];
      }
    }
    if(control & 0x2){
      for(int i = 0; i < WORDS_PER_CACHE_LINE; i++){
        l2_cache[index].cache_line[i] =  write_data[i];
      }
      l2_cache[index].v_d_tag = l2_cache[index].v_d_tag | L2_DIRTYBIT_MASK;
    }
    *status = *status | L2_HIT_STATUS_MASK;
  }

}



/********************************************************

             l2_insert_line()

This procedure inserts a new cache line into the L2 cache.

The parameters are:

address: 32-bit memory address for the new cache line.

write_data: an array of unsigned 32-bit words containing the
            cache line data to be inserted into the cache.

evicted_writeback_address: a 32-bit output parameter (thus,
          a pointer to it is passed) that, if the cache line
          being evicted needs to be written back to memory,
          should be assigned the memory address for the evicted
          cache line.

evicted_writeback_data: an array of 32-bit words. If the cache
          line being evicted needs to be written back to memory,
          the cache line data for the evicted cache line should be
          written to this array.  Since there are 8 words per cache line,
          the actual parameter will be an array of at least 8 words.

status: this in an 8-bit output parameter (thus, a pointer to it is
        passed).  The lowest bit of this byte should be set to
        indicate whether the evicted cache line needs to be
        written back to memory or not, as follows:
            0: no write-back required
            1: evicted cache line needs to be written back.

*********************************************************/

void l2_insert_line(uint32_t address, uint32_t write_data[],
                    uint32_t *evicted_writeback_address,
                    uint32_t evicted_writeback_data[],
                    uint8_t *status)
{

  //Extract from the address the index of the cache entry in the cache.
  //See l2_cache_access, above.

  int index = (address & L2_INDEX_MASK) >> L2_INDEX_SHIFT;

  //Extract from the address the tag bits.
  //See l2_cache_access, above.

  int tag = (address & L2_ADDRESS_TAG_MASK) >> L2_ADDRESS_TAG_SHIFT;

  //If the cache entry has a zero valid bit or a zero dirty bit,
  //then the entry can simply be overwritten with the new line.
  //Copy the data from write_data to the cache line in the entry,
  //set the valid bit of the entry, clear the dirty bit of the
  //entry, and write the tag bits of the address into the tag of
  //the entry. Clear the low bit of the status byte
  //to indicate that no write-back is needed. Nothing further
  //needs to be done, the procedure can return.

  if(!(l2_cache[index].v_d_tag & L2_VBIT_MASK) || !(l2_cache[index].v_d_tag & L2_DIRTYBIT_MASK)){
    for(int i = 0; i < WORDS_PER_CACHE_LINE; i++){
      l2_cache[index].cache_line[i] =  write_data[i];
    }
    l2_cache[index].v_d_tag = l2_cache[index].v_d_tag | L2_VBIT_MASK;
    l2_cache[index].v_d_tag = l2_cache[index].v_d_tag & (~L2_DIRTYBIT_MASK);
    l2_cache[index].v_d_tag = l2_cache[index].v_d_tag | tag;
    *status = *status & 0x0;
    return;
  }

  //Otherwise (i.e. both the valid and dirty bits are 1), the
  //current entry has to be written back before the
  //being overwritten by the new cache line:
  //The address to write the current entry back to is constructed from the
  //entry's tag and the index in the cache by:
  // (evicted_entry_tag << L2_ADDRESS_TAG_SHIFT) | (index << L2_INDEX_SHIFT)
  //This address should be written to the evicted_writeback_address output
  //parameter.
  //The cache line data in the current entry should be copied to the
  //evicted_writeback_data_array.
  //The low bit of the status byte should be set to 1 to indicate that
  //the write-back is needed.

  else{
    int evicted_entry_tag = l2_cache[index].v_d_tag & L2_ENTRY_TAG_MASK;
    *evicted_writeback_address = (evicted_entry_tag << L2_ADDRESS_TAG_SHIFT) | (index << L2_INDEX_SHIFT);
    for(int i = 0; i < WORDS_PER_CACHE_LINE; i++){
      evicted_writeback_data[i] = write_data[i];
    }
    *status = (*status & 0x0) | 0x1;

  //Then, copy the data from write_data to the cache line in the entry,
  //set the valid bit of the entry, clear the dirty bit of the
  //entry, and write the tag bits of the address into the tag of
  //the entry.

    for(int i = 0; i < WORDS_PER_CACHE_LINE; i++){
      l2_cache[index].cache_line[i] =  write_data[i];
    }
    l2_cache[index].v_d_tag = l2_cache[index].v_d_tag | L2_VBIT_MASK;
    l2_cache[index].v_d_tag = l2_cache[index].v_d_tag & (~L2_DIRTYBIT_MASK);
    l2_cache[index].v_d_tag = (l2_cache[index].v_d_tag & (~L2_ENTRY_TAG_MASK)) | tag;
  }
}

typedef struct {
  uint32_t v_r_d_tag;
  uint32_t cache_line[WORDS_PER_CACHE_LINE];
} L1_CACHE_ENTRY;

//4-way set-associative cache, so there are
//4 cache lines per set.
#define L1_LINES_PER_SET 4

/***************************************************
  This structure defines an L1 cache set. Its only
  field, lines, is an array of four cache lines.
***************************************************/

typedef struct {
  L1_CACHE_ENTRY lines[L1_LINES_PER_SET];
} L1_CACHE_SET;

//There are 512 sets in the L1 cache
#define L1_NUM_CACHE_SETS 512

//The L1 cache itself is just an array of 512 cache sets.
L1_CACHE_SET l1_cache[L1_NUM_CACHE_SETS];

//Mask for v bit: Bit 31 of v_r_d_tag
#define L1_VBIT_MASK (1 << 31)

//Mask for r bit: Bit 30 of v_r_d_tag
#define L1_RBIT_MASK (1 << 30)

//Mask for d bit: Bit 29 of v_r_d_tag
#define L1_DIRTYBIT_MASK (1 << 29)

//The tag is the low 18 bits of v_r_d_tag
//The mask is just 18 ones, which is 3FFFF hex
#define L1_ENTRY_TAG_MASK 0x3ffff

//Bits 2-4 of an address specifies the offset of the addressed
//word within the cache line
// Mask is 11100 in binary = 0x1C

#define WORD_OFFSET_MASK 0x1C

//After masking to extract the word offset, it needs
//to be shifted to the right by 2.
#define WORD_OFFSET_SHIFT 2

//The upper 18 bits (bits 14-31) of an address are used as the tag bits
//The mask is 18 ones (so 3FFFF hex) shifted left by 14 bits.
#define L1_ADDRESS_TAG_MASK (0x3FFFF << 14)

//After masking to extract the tag from the address, it needs to
//be shifted right by 14.
#define L1_ADDRESS_TAG_SHIFT 14

//Bits 5-13 are used to extract the set index from an address.
//The mask is 9 ones (so 1FF hex) shifted left by 5.
#define L1_SET_INDEX_MASK (0x1ff << 5)

//After masking to extract the set index from an address, it
//needs to be shifted to the right by 5
#define L1_SET_INDEX_SHIFT 5

//This can be used to set or clear the lowest bit of the status
//register to indicate a cache hit or miss.
#define L1_CACHE_HIT_MASK 0x1


/************************************************
            l2_initialize()

This procedure initializes the L1 cache by clearing
the valid bit of each cache entry in each set in
the cache.
************************************************/

void l1_initialize()
{
  for(int i = 0; i < L1_NUM_CACHE_SETS; i++){
    for(int j = 0; j < L1_LINES_PER_SET; j++){
      l1_cache[i].lines[j].v_r_d_tag = l1_cache[i].lines[j].v_r_d_tag & (~L1_VBIT_MASK);
    }
  }
}


/**********************************************************

             l1_cache_access()

This procedure implements the reading or writing of a single word
to the L1 cache.

The parameters are:

address:  unsigned 32-bit address. This address can be anywhere within
          a cache line.

write_data: a 32-bit word. On a write operation, if there is a cache
          hit, write_data is copied to the appropriate word in the
          appropriate cache line.

control:  an unsigned byte (8 bits), of which only the two lowest bits
          are meaningful, as follows:
          -- bit 0:  read enable (1 means read, 0 means don't read)
          -- bit 1:  write enable (1 means write, 0 means don't write)

read_data: a 32-bit output parameter (thus, a pointer to it is passed).
         On a read operation, if there is a cache hit, the appropriate
         word of the appropriate cache line in the cache is copied
         to read_data.

status: this in an 8-bit output parameter (thus, a pointer to it is
        passed).  The lowest bit of this byte should be set to
        indicate whether a cache hit occurred or not:
              cache hit: bit 0 of status = 1
              cache miss: bit 0 of status = 0

If the access results in a cache miss, then the only
effect is to set the lowest bit of the status byte to 0.

**********************************************************/


void l1_cache_access(uint32_t address, uint32_t write_data,
                     uint8_t control, uint32_t *read_data, uint8_t *status)
{

  //Extract from the address the index of the cache set in the cache.
  //Use the L1_SET_INDEX_MASK mask to mask out the appropriate
  //bits of the address and L1_SET_INDEX_SHIFT to shift the
  //bits the appropriate amount.

  int index = (address & L1_SET_INDEX_MASK) >> L1_SET_INDEX_SHIFT;

  //Extract from the address the tag bits.
  //Use the L1_ADDRESS_TAG_MASK mask to mask out the appropriate
  //bits of the address and L1_ADDRESS_TAG_SHIFT to shift the
  //bits the appropriate amount.

  int tag = (address & L1_ADDRESS_TAG_MASK) >> L1_ADDRESS_TAG_SHIFT;

  //Extract from the address the word offset within the cache line.
  //Use the WORD_OFFSET_MASK to mask out the appropriate bits of
  //the address and WORD_OFFSET_SHIFT to shift the bits the
  //appropriate amount.

  int word_offset = (address & WORD_OFFSET_MASK) >> WORD_OFFSET_SHIFT;

  //Within the set specified by the set index extracted from the address,
  //look through the cache entries for an entry that 1) has its valid
  //bit set AND 2) has a tag that matches the tag extracted from the address.

  //If no such entry exists in the set, then the result is a cache miss.
  //The low bit of the status output parameter should be set to 0. There
  //is nothing further to do in this case, the function can return.

  //Otherwise, if an entry is found with a set valid bit and a matching tag,
  //then it is a cache hit. The reference bit of the cache entry should be set
  //and the low bit of status output parameter should be set to 1.

  //If a read operation was specified, the appropriate word (as specified by
  //the word offset extracted from the address) of the entry's
  //cache line data should be written to read_data.

  //If a write operation was specified, the value of write_data should be
  //written to the appropriate word of the entry's cache line data and
  //the entry's dirty bit should be set.

  for(int i = 0; i < L1_LINES_PER_SET; i++){
    if((l1_cache[index].lines[i].v_r_d_tag & L1_VBIT_MASK) && ((l1_cache[index].lines[i].v_r_d_tag & L1_ENTRY_TAG_MASK) == tag)) {
      l1_cache[index].lines[i].v_r_d_tag = l1_cache[index].lines[i].v_r_d_tag | L1_RBIT_MASK;
      *status = *status | L1_CACHE_HIT_MASK;
      if(control & 0x1){
        *read_data = l1_cache[index].lines[i].cache_line[word_offset];
        printf(" ** L1 Read (%d @ %d, %d, %d) **", l1_cache[index].lines[i].cache_line[word_offset], index, i, word_offset);
      }
      if(control & 0x2){
        l1_cache[index].lines[i].cache_line[word_offset] = write_data;
        printf(" ** L1 Write (%d @ %d, %d, %d) **", l1_cache[index].lines[i].cache_line[word_offset], index, i, word_offset);
        if(index == 0 && i == 0 && word_offset == 1){
          printf("\nAccess %d (%d, %d = %d) @ %d\n", l1_cache[index].lines[i].cache_line[word_offset], l1_cache[index].lines[i].v_r_d_tag & L1_VBIT_MASK,
          l1_cache[index].lines[i].v_r_d_tag & L1_ENTRY_TAG_MASK, tag, address);
        }
        l1_cache[index].lines[i].v_r_d_tag = l1_cache[index].lines[i].v_r_d_tag | L1_DIRTYBIT_MASK;
      }
      return;
    }
  }
  *status = *status & 0x0;
}


/************************************************************

                 l1_insert_line()

This procedure inserts a new cache line into the L1 cache.

The parameters are:

address: 32-bit address of the new cache line.

write_data: an array of unsigned 32-bit words containing the
            cache line data to be inserted into the cache.

evicted_writeback_address: a 32-bit output parameter (thus,
          a pointer to it is passed) that, if the cache line
          being evicted needs to be written back to memory,
          should be assigned the memory address for the evicted
          cache line.

evicted_writeback_data: an array of 32-bit words. If the cache
          line being evicted needs to be written back to memory,
          the cache line data for the evicted cache line should
          be copied to this array. Since there are 8 words per
          cache line, the actual parameter will be an array of
          at least 8 words.

status: this in an 8-bit output parameter (thus, a pointer to it is
        passed).  The lowest bit of this byte should be set to
        indicate whether the evicted cache line needs to be
        written back to memory or not, as follows:
            0: no write-back required
            1: evicted cache line needs to be written back.


 The cache replacement algorithm uses a simple NRU
 algorithm. A cache entry (among the cache entries in the set) is
 chosen to be written to in the following order of preference:
    - valid bit = 0
    - reference bit = 0 and dirty bit = 0
    - reference bit = 0 and dirty bit = 1
    - reference bit = 1 and dirty bit = 0
    - reference bit = 1 and dirty bit = 1
*********************************************************/


void l1_insert_line(uint32_t address, uint32_t write_data[],
                    uint32_t *evicted_writeback_address,
                    uint32_t evicted_writeback_data[],
                    uint8_t *status)
{

  //Extract from the address the index of the set in the cache.
  //see l1_cache_access above

  int index = (address & L1_SET_INDEX_MASK) >> L1_SET_INDEX_SHIFT;

  //Extract from the address the tag bits.
  //see l1_cache_access above.

  int tag = (address & L1_ADDRESS_TAG_MASK) >> L1_ADDRESS_TAG_SHIFT;

  // The cache replacement algorithm uses a simple NRU
  // algorithm. A cache entry (among the cache entries in the set) is
  // chosen to be written to in the following order of preference:
  //    - valid bit = 0
  //    - reference bit = 0 and dirty bit = 0
  //    - reference bit = 0 and dirty bit = 1
  //    - reference bit = 1 and dirty bit = 0
  //    - reference bit = 1 and dirty bit = 1
  //  The search loops through the entries in the set. If it
  //  finds a valid bit = 0, then that entry can be overwritten and
  //  we can exit the loop.
  //  Otherwise, we remember the *first* line we encounter which has r=0 and d=0,
  //  the *first* line that has r=0 and d=1, etc. When we're done looping,
  //  we choose the entry with the highest preference on the above list to evict.

  //This variable is used to store the index within the set
  //of a cache entry that has its r bit = 0 and its dirty bit = 0.
  //Initialize it to ~0x0 (i.e. all 1's) to indicate an invalid value.
  uint32_t r0_d0_index = ~0x0;

  //This variable is used to store the index within the set
  //of a cache entry that has its r bit = 0 and its dirty bit = 1.
  uint32_t r0_d1_index = ~0x0;

  //This variable isused to store the index within the set
  //of a cache entry that has its r bit = 1 and its dirty bit = 0.
  uint32_t r1_d0_index = ~0x0;

  //In a loop, iterate though each entry in the set.

  for(int i = 0; i < L1_LINES_PER_SET; i++){

  // if the current entry has a zero v bit, then overwrite
  // the cache line in the entry with the data in write_data,
  // set the v bit of the entry, clear the dirty and reference bits,
  // and write the new tag to the entry. Set the low bit of the status
  // output parameter to 0 to indicate the evicted line does not need
  // to be written back. There is nothing further to do, the procedure
  // can return

    if(!(l1_cache[index].lines[i].v_r_d_tag & L1_VBIT_MASK)){
      for(int j =  0; j < WORDS_PER_CACHE_LINE; j++){
        l1_cache[index].lines[i].cache_line[j] = write_data[j];
      }
      l1_cache[index].lines[i].v_r_d_tag = l1_cache[index].lines[i].v_r_d_tag | L1_VBIT_MASK;
      l1_cache[index].lines[i].v_r_d_tag = l1_cache[index].lines[i].v_r_d_tag & (~(L1_RBIT_MASK | L1_DIRTYBIT_MASK));
      l1_cache[index].lines[i].v_r_d_tag = (l1_cache[index].lines[i].v_r_d_tag & (~L1_ENTRY_TAG_MASK)) | tag;
      *status = *status & 0x0;
      return;
    }

  //  Otherwise, we remember the first entry we encounter which has r=0 and d=0,
  //  the first entry that has r=0 and d=1, etc.
    else if((r0_d0_index == (~0x0)) && !(l1_cache[index].lines[i].v_r_d_tag & (L1_RBIT_MASK | L1_DIRTYBIT_MASK))){
      r0_d0_index = i;
    }
    else if((r0_d1_index == (~0x0)) && !(l1_cache[index].lines[i].v_r_d_tag & L1_RBIT_MASK) && (l1_cache[index].lines[i].v_r_d_tag & L1_DIRTYBIT_MASK)){
      r0_d1_index = i;
    }
    else if((r1_d0_index == (~0x0)) && (l1_cache[index].lines[i].v_r_d_tag & L1_RBIT_MASK) && !(l1_cache[index].lines[i].v_r_d_tag & L1_DIRTYBIT_MASK)){
      r1_d0_index = i;
    }
  }

  //When we're done looping, we choose the entry with the highest preference
  //on the above list to evict.

  int line_index;
  if(r0_d0_index != (~0x0)){
    line_index = r0_d0_index;
  }
  else if(r0_d1_index != (~0x0)){
    line_index = r0_d1_index;
  }
  else if(r1_d0_index != (~0x0)){
    line_index = r1_d0_index;
  }
  else{
    line_index = 0;
  }

  //if the dirty bit of the cache entry to be evicted is set, then the data in the
  //cache line needs to be written back. The address to write the current entry
  //back to is constructed from the entry's tag and the set index by:
  // (evicted_entry_tag << L1_ADDRESS_TAG_SHIFT) | (set_index << L1_SET_INDEX_SHIFT)
  //This address should be written to the evicted_writeback_address output
  //parameter. The cache line data in the evicted entry should be copied to the
  //evicted_writeback_data_array.

  //Also, if the dirty bit of the chosen entry is been set, the low bit of the status byte
  //should be set to 1 to indicate that the write-back is needed. Otherwise,
  //the low bit of the status byte should be set to 0.

  if(l1_cache[index].lines[line_index].v_r_d_tag & L1_DIRTYBIT_MASK){
    int evicted_entry_tag = l1_cache[index].lines[line_index].v_r_d_tag & L1_ENTRY_TAG_MASK;
    *evicted_writeback_address = (evicted_entry_tag << L1_ADDRESS_TAG_SHIFT) | (index << L1_SET_INDEX_SHIFT);
    for(int i = 0; i < WORDS_PER_CACHE_LINE; i++){
      evicted_writeback_data[i] = l1_cache[index].lines[line_index].cache_line[i];
    }
    *status = *status | 0x1;
  }
  else{
    *status = *status & 0x0;
  }

  //Then, copy the data from write_data to the cache line in the entry,
  //set the valid bit of the entry, clear the dirty bit of the
  //entry, and write the tag bits of the address into the tag of
  //the entry.

  for(int i = 0; i < WORDS_PER_CACHE_LINE; i++){
    l1_cache[index].lines[line_index].cache_line[i] = write_data[i];
    l1_cache[index].lines[line_index].v_r_d_tag = l1_cache[index].lines[line_index].v_r_d_tag | L1_VBIT_MASK;
    l1_cache[index].lines[line_index].v_r_d_tag = l1_cache[index].lines[line_index].v_r_d_tag & (~L1_DIRTYBIT_MASK);
    l1_cache[index].lines[line_index].v_r_d_tag = (l1_cache[index].lines[line_index].v_r_d_tag & (~L1_ENTRY_TAG_MASK)) | tag;
  }
}

/************************************************

       l1_clear_r_bits()

This procedure clears the r bit of each entry in each set of the L1
cache. It is called periodically to support the NRU algorithm.

***********************************************/

void l1_clear_r_bits()
{
  for(int i = 0; i < L1_NUM_CACHE_SETS; i++){
    for(int j = 0; j < L1_LINES_PER_SET; j++){
      l1_cache[i].lines[j].v_r_d_tag = l1_cache[i].lines[j].v_r_d_tag & (~L1_RBIT_MASK);
    }
  }
}

//These are defined below.
void memory_handle_l1_miss(uint32_t address);
void memory_handle_l2_miss(uint32_t address, uint8_t control);

//We are going to count how many L1 and L2 cache misses
//have occurred. These are the variables used to keep
//track of the misses.
uint32_t num_l1_misses;
uint32_t num_l2_misses;

/*******************************************************

        memory_subsystem_initialize()

This procedure is used to initialize the memory subsystem.
It takes a single 32-bit parameter, memory_size_in_bytes,
that determines large the main memory will be.

*******************************************************/
void memory_subsystem_initialize(uint32_t memory_size_in_bytes)
{

  //Call the intialization procedures for main memory,
  // L2 cache, and L1 cache.

  main_memory_initialize(memory_size_in_bytes);
  l2_initialize();
  l1_initialize();

  //Also initialize num_l1_misses and num_l2_misses to 0.

  num_l1_misses = 0;
  num_l2_misses = 0;
}



/*****************************************************

              memory_access()

This is the procedure for reading and writing one word
of data from and to the memory subsystem.

It takes the following parameters:

address:  32-bit address of the data being read or written.

write_data: In the case of a memory write, the 32-bit value
          being written.

control:  an unsigned byte (8 bits), of which only the two lowest bits
          are meaningful, as follows:
          -- bit 0:  read enable (1 means read, 0 means don't read)
          -- bit 1:  write enable (1 means write, 0 means don't write)

read_data: a 32-bit ouput parameter (thus, a pointer to it is passed).
         In the case of a read operation, the data being read will
         be written to read_data.

****************************************************/

void memory_access(uint32_t address, uint32_t write_data,
                   uint8_t control, uint32_t *read_data)
{
  printf("\nMA: (%d %d @ %d)", control, write_data, address);
  uint8_t status;

  //call l1_cache_access to try to read or write the
  //data from or to the L1 cache.

  l1_cache_access(address, write_data, control, read_data, &status);

  //If an L1 cache miss occurred, then:
  // -- increment num_l1_misses
  // -- call memory_handle_l1_miss(), below, specifying
  //    the requested address, to bring the needed
  //    cache line into L1.
  // -- call l1_cache_access again to read or
  //      write the data.

  if(!status){
    printf(" -> L1 cache miss (%d)", status);
    num_l1_misses++;
    memory_handle_l1_miss(address);
    l1_cache_access(address, write_data, control, read_data, &status);
    if(!status){
      printf(" SOMETHING WENT WRONG");
    }
  }
}



//This procedure should be called when an L1 cache miss occurs.
//It doesn't matter if the miss occured on a read or a write
//operation. It takes as a parameter the address that resulted
//in the L1 cache miss.

void memory_handle_l1_miss(uint32_t address)
{

  //call l2_cache_access to read the cache line containing
  //the specified address from the L2 cache. This is necessary
  //regardless if the operation that caused the L1 cache miss
  //was a read or a write.

  uint32_t read_data[WORDS_PER_CACHE_LINE];
  uint8_t status;
  l2_cache_access(address, NULL, 1, read_data, &status);

  //if the result was an L2 cache miss, then:
  //   -- increment num_l2_misses
  //   -- call memory_handle_l2_miss (below), specifying the address that
  //      caused the L2 miss (which is the same as the address that
  //      caused the L1 miss), and specifying that the L2 miss
  //      occurred when attempting to read (not write) from L2 the cache.
  //  --  call l2_cache_access again to read the needed cache line
  //      from the l2 cache.

  if(!status){
    printf(" -> L2 cache miss (%d)", status);
    num_l2_misses++;
    memory_handle_l2_miss(address, 1);
    // status = 0;
    l2_cache_access(address, NULL, 1, read_data, &status);
    if(!status){
      printf(" SOMETHING WENT WRONG");
    }
  }


  //Now that the needed cache line has been retrieved from the
  //L2 cache (whether an L2 cache miss occurred or not),
  //insert the cache line into the L1 cache by calling l1_insert_line.

  uint32_t evicted_writeback_address;
  uint32_t evicted_writeback_data[WORDS_PER_CACHE_LINE];
  // status = 0;
  l1_insert_line(address, read_data, &evicted_writeback_address, evicted_writeback_data, &status);

  //if the cache line that was evicted from L1 has to be written back,
  //then l2_cache_access must be called to write the evicted cache line
  //to L2. If a cache miss occurs when writing the evicted cache line
  //to L2, then:
  //   -- memory_handle_l2_miss (below) should be called, specifying the
  //      address (of the evicted line) that caused the L2 cache miss,
  //      and specifying that the operation that caused the L2 miss
  //      was a write (not a read).
  //   -- l2_cache_access should be called again to write the cache line
  //      evicted from L1 into L2.
  //

  if(status){
    printf(" -> Write back to L2 (%d)", status);
    // status = 0;
    l2_cache_access(evicted_writeback_address, evicted_writeback_data, 2, NULL, &status);
    if(!status){
      printf(" -> L2 evicted cache miss (%d)", status);
      memory_handle_l2_miss(evicted_writeback_address, 2);
      l2_cache_access(evicted_writeback_address, evicted_writeback_data, 2, NULL, &status);
      if(!status){
        printf(" SOMETHING WENT WRONG");
      }
    }
  }
}


//This procedure handles an L2 cache miss, whether the miss occurred
//due to a read or a write. It takes the following parameters:
// -- address: the address that caused the L2 cache miss
// -- control:  an unsigned byte (8 bits) that indicates whether the
//          L2 miss occurred on a read or a write operation. Only the
//          two lowest bits are meaningful, as follows:
//          -- bit 0 = 1 means a read operation caused the miss
//          -- bit 1 = 1 means a write operation caused the miss
//           (naturally, the two bits should not both be set to 1)

void memory_handle_l2_miss(uint32_t address, uint8_t control)
{
  uint32_t cache_line[WORDS_PER_CACHE_LINE];

  //if the L2 miss was on a read operation, then main_memory_access
  //must be called to fetch the needed cache line from main_memory.
  //The fetched cache line should be written to cache_line (see above).
  //However, if the L2 miss was on a write operation (with an evicted line from L1),
  //there's no need to read the cache line from main memory, since
  //that line will be overwritten.

  if(control & 0x1){
    main_memory_access(address, NULL, control, cache_line);
  }

  //Now call l2_insert_line to insert the cache line data in cache_line,
  //above, into L2. In the case of a read, this is the cache line data
  //that has been read from main memory. In the case of a write, then
  //it's just meaningless data being written to L2 (i.e. whatever happened
  //to be in cache_line), since that line in L2 will be overwritten subsequently.
  uint8_t status;
  uint32_t evicted_writeback_address;
  uint32_t evicted_writeback_data[WORDS_PER_CACHE_LINE];
  l2_insert_line(address, cache_line, &evicted_writeback_address, evicted_writeback_data, &status);

  //If the call to l2_insert_line resulted in an evicted cache line
  //that has to be written back to main memory, call main_memory_access
  //to write the evicted cache line to main memory.

  if(status){
    printf(" -> Write back Main Memory (%d)", status);
    main_memory_access(evicted_writeback_address, cache_line, 2, NULL);
  }
}


//This procedure will be called periodically (e.g. when a clock
//interrupt occurs) in order to cause the r bits in the
//L1 cache to be cleared in support of the NRU replacement
//algorithm.
void memory_handle_clock_interrupt()
{
  //call the function which clears the r bits in the L1 cache

  l1_clear_r_bits();
}

#define MAIN_MEMORY_SIZE_IN_BYTES (1<<25)

int main(){
  printf("%d", MAIN_MEMORY_SIZE_IN_BYTES);
  memory_subsystem_initialize(MAIN_MEMORY_SIZE_IN_BYTES);
  uint32_t address = 4;
  memory_access(address, 23, WRITE_ENABLE_MASK, NULL);
  memory_access(65540, 930131, WRITE_ENABLE_MASK, NULL);
  uint32_t read_data;
  memory_access(address, 0, READ_ENABLE_MASK, &read_data);
  printf("%d\n", read_data);
  memory_access(65540, 0, READ_ENABLE_MASK, &read_data);
  printf("%d\n", read_data);
}
