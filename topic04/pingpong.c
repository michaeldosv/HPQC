#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

// declare functions
void check_args(int argc, char **argv, int *num_pings, int *size_in_bytes);
void root_task(int num_pings, int size_in_bytes);
void client_task(int num_pings, int size_in_bytes);

int main(int argc, char **argv) {
        int rank, size;
        int num_pings = 0;
        int size_in_bytes = 0;

        // initialize MPI
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        // ensure exactly two processors are used
        if (size != 2) {
                if (rank == 0) { 
                        printf("ERROR: This program requires exactly 2 processes.\n");
                }
                MPI_Finalize();
                return -1;
        }

        // call check_args to extract our 2 arguments
        check_args(argc, argv, &num_pings, &size_in_bytes);

        // ensure both processes start at the exact same time before timing
        MPI_Barrier(MPI_COMM_WORLD);

        // delegate tasks based on rank
        if (rank == 0) {
                root_task(num_pings, size_in_bytes);
        } else if (rank == 1) {
                client_task(num_pings, size_in_bytes);
        }

        MPI_Finalize();
        return 0;
}


// functions
// check that we have the correct number of arguments 3
void check_args(int argc, char **argv, int *num_pings, int *size_in_bytes) {
        // We expect 3 things: the program name, the pings, and the byte size
        if (argc != 3) {
                if (0 == *num_pings) { // Just to silence it for other ranks
                        fprintf(stderr, "Usage: %s <number_of_pings> <message_size_in_bytes>\n", argv[0]);
                }
                MPI_Finalize();
                exit(-1);
        }
        *num_pings = atoi(argv[1]);
        *size_in_bytes = atoi(argv[2]);

        // Safety check to ensure array is at least 1 integer large
        if (*size_in_bytes < sizeof(int)) {
                *size_in_bytes = sizeof(int);
        }
}

void root_task(int num_pings, int size_in_bytes) {
        int tag = 0;
        MPI_Status status;
        double start_time, end_time;

        int num_elements = size_in_bytes / sizeof(int);
        int *message_array = (int*)malloc(num_elements * sizeof(int));

        for (int i = 0; i < num_elements; i++) {
                message_array[i] = 0;
        }

        start_time = MPI_Wtime();

        // Loop using element 0 of the array as the counter
        while (message_array[0] < num_pings) {
                MPI_Send(message_array, num_elements, MPI_INT, 1, tag, MPI_COMM_WORLD);
                MPI_Recv(message_array, num_elements, MPI_INT, 1, tag, MPI_COMM_WORLD, &status);
        }

        end_time = MPI_Wtime();

        double benchmark = end_time - start_time;
        double average_time = benchmark / (2.0 * num_pings);        
        
        printf("Target Pings : %d\n", num_pings);
        printf("Array Size : %d Bytes (%d integers)\n", size_in_bytes, num_elements);
        printf("Final Counter Value : %d\n", message_array[0]);
        printf("Average Transfer Time : %f seconds\n", average_time);

        free(message_array);
}

void client_task(int num_pings, int size_in_bytes) {
        int tag = 0;
        MPI_Status status;

        int num_elements = size_in_bytes / sizeof(int);
        int *message_array = (int*)malloc(num_elements * sizeof(int));

        int local_counter = 0;

        while (local_counter < num_pings) {
                MPI_Recv(message_array, num_elements, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);

                // increment element 0 to track the number of pings
                message_array[0]++;
                local_counter = message_array[0]; 

                MPI_Send(message_array, num_elements, MPI_INT, 0, tag, MPI_COMM_WORLD);
        }

        free(message_array);
}
