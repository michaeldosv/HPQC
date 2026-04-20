#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

// define function to check user inputs
int check_args(int argc, char **argv);

// main
int main(int argc, char **argv) {

	// initialize MPI rank and vector size variables
	int rank, size;
	int ierror = MPI_Init(&argc, &argv);

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	// get the total size of the vector from the user input
	int total_elements = check_args(argc, argv);

	// calculate number of local process chunks
	int local_chunk_size = total_elements / size;

	// create and populate the local vector chunk
	int* local_vector = malloc(local_chunk_size * sizeof(int));

	// offset the values so they match the serial version exactly
	int start_index = rank * local_chunk_size;
	for (int i = 0; i < local_chunk_size; i++) {
		local_vector[i] = start_index + i;
	}

	// implement internal benchmark
	MPI_Barrier(MPI_COMM_WORLD);

	// start the timer
	double start_time = MPI_Wtime();

	// add all elements of the local chunk together
	int local_sum = 0;
	for (int i = 0; i < local_chunk_size; i++) {
		local_sum += local_vector[i];
	}

	// add all local vector sums together to get final sum
	int global_sum = 0;
	// root adds its local sum first and then receives sum from other processes
	if (rank == 0) {
                global_sum = local_sum;
                int incoming_sum = 0;

                for (int sender = 1; sender < size; sender++) {
                        MPI_Recv(&incoming_sum, 1, MPI_INT, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                        global_sum += incoming_sum;
                }
        } else {
                // client processes send their sums to root process
                MPI_Send(&local_sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }

	// end the timer and compute the total time taken
	double end_time = MPI_Wtime();
	double benchmark = end_time - start_time;

	// print the sum of the vector and internal benchmark time and the sum of MPI_Send() and MPI_Recv()
	if (rank == 0) {
		printf("Sum: %d\n", global_sum);
		printf("Internal Benchmark : %f seconds", benchmark);
		printf("Send and Receive Sum %d\n", global_sum);
	}

	// free up memory and exit MPI
	free(local_vector);
	MPI_Finalize();
	return 0;
}

// check arguments provided by the user
int check_args(int argc, char **argv) {

	int num_arg = 0;

	if (argc == 2) {
		num_arg = atoi(argv[1]);
	}
	else {
		fprintf(stderr, "ERROR: You did not provide a numerical argument!\n"); // error if arguments provided was not 2
 		exit(-1);
	}
	return num_arg;
}
