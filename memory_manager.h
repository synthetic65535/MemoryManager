#include <stdint.h>

/* -------- Settings -------- */

// Fixed block size for memory allocation. Max: 65535
#define BLOCK_SIZE 1024

// Memory pool size in blocks. Max: 65535
#define POOL_SIZE 65535

/* -------- Types -------- */

// Element of double linked list of blocks
typedef struct block_s
{
	struct block_s *next; // Next block
	struct block_s *prev; // Previous block
	uint8_t data[BLOCK_SIZE]; // Data
} block_t;

/* -------- Function definitions -------- */

// Initialization
void init(void);

// Get one block of memory
void* get_mem(void);

// Release memory
void free_mem(void* _p);
