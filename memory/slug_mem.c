/*******************
 * Brilliant Squid 
 * May 8, 2014     
 *******************/
#include <stdlib.h>
#include <time.h>

/* Stores a single entry for an allocated block.
 */
struct mem_block
{
  void *address;
  size_t length;
  time_t time_alloc;
  char *location;
  struct mem_block *prev;
  struct mem_block *next;
};

/* Linked list for organizing allocated memory. 
 */
struct mem_list
{
  int tot_alloc;
  int cur_alloc;
  size_t cur_mem_alloc;
  struct mem_block *head;
  struct mem_block *tail;
};

static struct mem_list MemoryList = {0, 0, 0, 0, 0};

/*****************************************************************************
 * slug_malloc()
 *
 * This function will allocate memory by calling malloc(). It also records
 * the address, length, current timestamp, and location of the call in an
 * internal data structure. If size is 0, this is not an error but should be
 * reported on stderr as an unusual operation. If the input is > 128 MiB,
 * then an error is reported to stderr and execution terminates. The
 * parameter WHERE is a string constant that records the filename and line
 * # of the caller.
 *
 ****************************************************************************/
void *slug_malloc(size_t size, char *WHERE)
{
  /* Allocate memory for a new block */
  struct mem_block *block;
  if ((block = malloc(sizeof(struct mem_block))) == NULL) {
      perror("Error allocating block.");
      exit(2);
  }

  /* Report if requested size == 0 */
  if (size == 0) {
    perror("Requesting zero bytes.");
  }

  /* Allocate requested block */
  if ((block->address = malloc(size)) == NULL) {
    perror("Error allocating requested memory.");
    exit(3);
  }

  /* Initialize block */
  block->length = size;
  block->time_alloc = time(NULL);
  block->location = WHERE;
  block->prev = MemoryList.tail;
  block->next = NULL;

  /* Update list */
  MemoryList.cur_alloc++;
  MemoryList.tot_alloc++;
  MemoryList.cur_mem_alloc += size; 
  if (MemoryList.cur_alloc == 1) {
    MemoryList.head = block;
  } else {
    MemoryList.tail->next = block;
  }

  MemoryList.tail = block;
  return block->address;
}

/*****************************************************************************
 * slug_free()
 *
 * This function ensures that addr is the start of a valid memory region that
 * is currently allocated by looking at an internal data structure. If not,
 * an error is shown and the program is terminated. If it is valid then
 * free() should be called and the internal data structures updated to
 * indicate that the address is no longer actively allocated.
 ****************************************************************************/

void slug_free ( void *addr, char *WHERE )
{

}

/*****************************************************************************
 * slug_memstats()
 *
 * This function traverses the interal data structures and prints out
 * information about all current allocations. Each allocation report should
 * include the size of the allocation, a timestamp for when the allocation
 * took place, the actual address of the allocation, and file and line
 * number in the test program where the allocation happened. In addition, a
 * summary of all allocations should be reported, including the total # of
 * allocations done, the # of currently active allocations, the total
 * amount of memory currently allocated, and the mean and stdev of sizes
 * that have been allocated.
 ****************************************************************************/
void slug_memstats ( void )
{

}
