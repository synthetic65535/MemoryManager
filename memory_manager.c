#include "memory_manager.h"
#include <stdint.h>

/* -------- Variables -------- */

// Memory pool
block_t pool[POOL_SIZE];

// A first element of free blocks list
block_t *free_list;

// A first element of used blocks list
block_t *used_list;

/* -------- Functions -------- */

// Initialization
void init(void)
{
	// Making one continous list of blocks
	for (uint16_t i=0; i<POOL_SIZE; i++)
	{
		pool[i].prev = (i == 0) ? 0 : &(pool[i - 1]);
		pool[i].next = (i == (POOL_SIZE - 1)) ? 0 : &(pool[i + 1]);
	}
	
	// Connect all block to free list
	free_list = &(pool[0]);
	used_list = 0;
}

// Get one block of memory
void* get_mem(void)
{
	if (free_list == 0) return 0; // No free memory left
	
	if (used_list == 0) // If used_list is empty
	{
		// Initializing first block in used_list
		used_list = free_list;
		free_list = free_list->next;
		free_list->prev = 0;
		used_list->next = 0;
		used_list->prev = 0;
	} else {
		// Moving one block from free_list to used_list
		used_list->prev = free_list;
		free_list = free_list->next;
		if (free_list != 0) free_list->prev = 0;
		used_list->prev->next = used_list;
		used_list = used_list->prev;
		
	}
	
	return &(used_list->data[0]);
}

// Release memory
void free_mem(void *_p)
{
	if (used_list == 0) return; // Nothing was used
	if (_p == 0) return; // Null pointer
	if ((_p < (void*)pool) || (_p > ((void*)pool + POOL_SIZE * sizeof(block_t)))) return; // Checking boundaries
	if ((_p - (void*)pool - sizeof(block_t*)*2) % sizeof(block_t) != 0) return; // Checking if it is correct pointer to data
	
	block_t *given_block;
	given_block = (block_t*)(_p - sizeof(block_t*)*2); // Getting a pointer to the beginning of block structure
	
	// Removing given_block from used_list and repair pointers
	if (given_block->prev != 0) given_block->prev->next = given_block->next;
	if (given_block->next != 0) given_block->next->prev = given_block->prev;
	if (given_block == used_list) used_list = used_list->next;
	
	if (free_list == 0) // If free_list is empty
	{
		// Initializing first block in free_list
		free_list = given_block;
		free_list->next = 0;
		free_list->prev = 0;
	} else {
		// Moving given_block block to free_list
		free_list->prev = given_block;
		given_block->next = free_list;
		given_block->prev = 0;
		free_list = given_block;
	}
}
