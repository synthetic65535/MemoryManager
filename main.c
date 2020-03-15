#include "memory_manager.h"
#include "tests.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

clock_t start, end;
double time_used;
uint8_t error_code;

int main(void)
{
	init();
	
	printf("Overwritings test: ");
	start = clock();
	error_code = test_overwritings();
	end = clock();
	time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	if (error_code == 0) printf("pass (%f sec)\n", time_used); else printf("fail\n");
	
	printf("Memory leakage test: ");
	start = clock();
	error_code = test_leakage();
	end = clock();
	time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
	if (test_leakage() == 0) printf("pass (%f sec)\n", time_used); else printf("fail\n");
	
	return 0;
}
