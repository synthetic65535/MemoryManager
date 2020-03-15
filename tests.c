#include <stdint.h>
#include <stdlib.h>
#include "tests.h"
#include "memory_manager.h"

// Test for block overwritings
uint8_t test_overwritings(void)
{
	uint8_t data[BLOCK_SIZE];
	uint8_t *my_block;
	uint16_t i, j;
	
	// Generating valuable data
	for (i=0; i<BLOCK_SIZE; i++) data[i] = rand();
	
	// Writing valuable data
	my_block = get_mem();
	for (i=0; i<BLOCK_SIZE; i++) my_block[i] = data[i];
	
	// Writing noise
	void *noise_blocks[POOL_SIZE];
	
	for (i=0; i<POOL_SIZE; i++)
	{
		noise_blocks[i] = get_mem();
		if (noise_blocks[i] != 0)
		{
			for (j=0; j<BLOCK_SIZE; j++)
			{
				((uint8_t *)noise_blocks[i])[j] = rand();
			}
		}
	}
	
	// Checking valuable data
	uint8_t error_code = 0;
	for (i=0; i<BLOCK_SIZE; i++)
		if (my_block[i] != data[i])
			error_code = 1;
	
	// Releasing memory
	free_mem(my_block);
	for (i=0; i<POOL_SIZE; i++)
		free_mem(noise_blocks[i]);
	
	return error_code;
}

// Test for memory leakage
uint8_t test_leakage(void)
{
	uint16_t i, j, c;
	
	init();
	
	// Get all memory
	void *noise_blocks[POOL_SIZE];
	
	c = 0;
	for (i=0; i<POOL_SIZE; i++)
	{
		noise_blocks[i] = get_mem();
		if (noise_blocks[i] != 0)
		{
			for (j=0; j<BLOCK_SIZE; j++)
			{
				((uint8_t *)noise_blocks[i])[j] = rand();
			}
			c++;
		}
	}
	
	if (get_mem() != 0) return 1; // Trying get one more
	
	if (c < POOL_SIZE) return 2;
	
	// Releasing all memory
	for (i=0; i<POOL_SIZE; i++)
		free_mem(noise_blocks[i]);
	
	// Get all memory again and counting free blocks
	c = 0;
	for (i=0; i<POOL_SIZE; i++)
	{
		noise_blocks[i] = get_mem();
		if (noise_blocks[i] != 0) c++;
	}
	
	if (get_mem() != 0) return 3; // Trying to get one more
	
	if (c < POOL_SIZE) return 4;
	
	// Releasing all memory
	for (i=0; i<POOL_SIZE; i++)
		free_mem(noise_blocks[i]);
	
	return 0;
}
