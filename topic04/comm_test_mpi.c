#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// declare functions
void check_uni_size(int uni_size);
void delegate_task(int my_rank, int uni_size);
void root_task(int my_rank, int uni_size);
void client_task(int my_rank, int uni_size);

// main loop
int main(int argc, char **argv) 
{
	// declare and initialise error handling variable
	int ierror = 0;
	
	// declare and initialise rank and size varibles
	int my_rank, uni_size;
	my_rank = uni_size = 0;

	// intitalise MPI
	ierror = MPI_Init(&argc, &argv);

	// gets the rank and world size
	ierror = MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
	ierror = MPI_Comm_size(MPI_COMM_WORLD,&uni_size);
	
	// route the process based on the rank
	delegate_task(my_rank, uni_size);

	// finalize MPI
	ierror = MPI_Finalize();
	return 0;
}


// define functions

// check if uni_size is greater than 1
void check_uni_size(int uni_size) {
	if (uni_size <= 1) { // number of processors must be >= 1

	// outputs error message if not enough processors defined
	printf("Unable to communicate with less than 2 processes. MPI communicator size = %d\n", uni_size);

	MPI_Finalize();

	exit(0);

        }
}


// delegate tasks based on their ranks
void delegate_task(int my_rank, int uni_size) {

	if (0 == my_rank) { // send rank 0 to root_task()
		root_task(my_rank, uni_size);
	}

	else { // send other ranks to client_task()
		client_task(my_rank, uni_size);
	}
}


// process for rank 0
void root_task(int my_rank, int uni_size) {

	// initialise transmission variables
	int recv_message = 0;
	int count = 1;
	int source = 0;
	int tag = 0;
	MPI_Status status;

	// define variables for internal benchmark
	double start_time, end_time, benchmark;

	// iterate through all the client ranks
	for (int their_rank = 1; their_rank < uni_size; their_rank++) {

		// set the source argument to the expected sender's rank
		source = their_rank;

		// start internal benchmark
		start_time = MPI_Wtime();

		// receive the message
		MPI_Recv(&recv_message, count, MPI_INT, source, tag, MPI_COMM_WORLD, &status);

		// end internal benchmark
		end_time = MPI_Wtime();

		// calculate time taken
		benchmark = end_time - start_time;

		// print received confirmation and internal benchmark
		printf("Hello, I am %d of %d. Received %d from Rank %d\n",
			my_rank, uni_size, recv_message, source);
		printf("[RANK %d] took %f s to recv from Rank %d\n",
			my_rank, benchmark, source);
	}
}


// process for remaining ranks
void client_task(int my_rank, int uni_size) {

	// initialise transmission variables
	int send_message = 0;
	int count = 1;
	int dest = 0; // destination is always rank 0
	int tag = 0;

	// define variables for internal benchmark
	double start_time, end_time, benchmark;

	// start internal benchmark
	start_time = MPI_Wtime();

	// perform calculation
	send_message = my_rank * 10;

	// send the calculation to rank 0
	MPI_Send(&send_message, count, MPI_INT, dest, tag, MPI_COMM_WORLD);

	// end internal benchmark
	end_time = MPI_Wtime();

	// calculate internal benchmark
	benchmark = end_time - start_time;

	// print the sent confirmation and internal benchmark
	printf("Hello, I am %d of %d. Sent %d to Rank %d\n",
		my_rank, uni_size, send_message, dest);
	printf("[RANK %d] took %f s to send\n", my_rank, benchmark);
}
