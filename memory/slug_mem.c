/*******************
 * Brilliant Squid 
 * May 8, 2014     
 *******************/
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

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
    int tot_alloc;              /* Running count of allocations */ 
    int cur_alloc;              /* Number of active blocks */ 
    size_t cur_mem_alloc;       /* Total size of active blocks */
    struct mem_block *head;     /* Pointer to head of list */
    struct mem_block *tail;     /* Pointer to tail of list */
    float mean;                 /* Running average of block size */
    float pwr_sum_avg;          /* Used to compute stdev of block size */
};

/* Memory management data structure */
static struct mem_list MemoryList = {0, 0, 0, 0, 0, 0.0, 0.0};

/* Function declarations */
void *slug_malloc ( size_t size, char *WHERE );
void slug_free ( void *addr, char *WHERE );
void slug_memstats ( void );

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

    /* Call memstats on exit */
    if (MemoryList.tot_alloc == 0)
        atexit(slug_memstats);

    /* Report if requested size == 0 */
    if (size == 0) {
        fprintf(stderr, "Requesting zero bytes at %s\n", WHERE);
    }

    /* Allocate requested block */
    if ((block->address = malloc(size)) == NULL) {
        fprintf(stderr, "Error allocating requested memory at %s\n", WHERE);
        exit(3);
    }

    /* Initialize block */
    block->length = size;
    block->time_alloc = time(NULL);
    block->location = WHERE;
    block->prev = MemoryList.tail;
    block->next = NULL;

    /* Update stats and list */
    MemoryList.cur_alloc++;
    MemoryList.tot_alloc++;
    MemoryList.cur_mem_alloc += size; 
    MemoryList.mean += (size - MemoryList.mean) / ((float) MemoryList.tot_alloc);
    MemoryList.pwr_sum_avg += (size * size - MemoryList.pwr_sum_avg) \
                              / ((float) MemoryList.tot_alloc);
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
    /* Locate requested block to be freed */
    struct mem_block *block;
    for (block = MemoryList.head; block != NULL && block->address != addr; block = block->next) ;

    /* Ensure block is not null */
    if (block == NULL) {
        fprintf(stderr, "Invalid free called at %s\n", WHERE);
        exit(4);
    }
    /* Remove requested memory */
    free(block->address);

    /* Remove list entry */
    if (block->prev != NULL) {                      /* Not the head of list */
        block->prev->next = block->next;
        if (block->next == NULL)                    /* Tail of non-singleton list */
            MemoryList.tail = block->prev;            
    } else if (block->next != NULL) {               /* Head of non-singleton list */
        MemoryList.head = block->next;            
        MemoryList.head->prev = NULL;            
    } else {                                        /* List was singleton */
        MemoryList.head = MemoryList.tail = NULL; 
    }

    /* Update stats */
    MemoryList.cur_alloc--;
    MemoryList.cur_mem_alloc -= block->length;
    free(block);
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
    int i = 1;
    struct mem_block *block;
    printf("Currrent Allocations: %d \n", MemoryList.cur_alloc);
    for (block = MemoryList.head; block != NULL; block = block->next) {
        printf("Block %d:\n "
                "\t-> Address: %p\n"
                "\t-> Size: %lu\n"
                "\t-> Timestamp: %ld\n"
                "\t-> Location called: %s\n",\
                i, block->address, block->length, block->time_alloc, block->location);
        i++;
    }
    printf("\n");
    printf("Size of active allocations:         %lu\n", MemoryList.cur_mem_alloc);
    printf("Total allocations made:             %d\n", MemoryList.tot_alloc);
    printf("\n");
    printf("Average block size:                 %f\n", MemoryList.mean);
    printf("Standard deviation:                 %f\n", \
            MemoryList.tot_alloc > 1 ? \
                sqrt( ( MemoryList.pwr_sum_avg * MemoryList.tot_alloc \
                  - MemoryList.tot_alloc * MemoryList.mean * MemoryList.mean) \
                / ( (float) (MemoryList.tot_alloc - 1) ) ) \
                : 0);
}
