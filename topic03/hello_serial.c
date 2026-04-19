#include <stdio.h>

int main(int argc, char **argv) {

	int uni_size = 16; // run process x times, x is hardcoded as opposed to being a user defined argument

	for(int my_rank = 0; my_rank < uni_size; my_rank++) { // use a loop to achieve same output as hello_mpi.c
		printf("Hello, I am %d of %d\n", my_rank, uni_size);
	}

	return 0;
}
