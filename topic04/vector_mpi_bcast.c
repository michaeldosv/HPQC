#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_args(int argc, char **argv);

int main(int argc, char **argv) {
        int rank, size;
        MPI_Init(&argc, &argv);

        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        int total_elements = check_args(argc, argv);
        int local_chunk_size = total_elements / size;

        // all processes allocate memory for the full array
        int* full_vector = malloc(total_elements * sizeof(int));

        // only the root processor initializes the array
        if (rank == 0) {
                for (int i = 0; i < total_elements; i++) {
                        full_vector[i] = i; 
                }
        }

        MPI_Barrier(MPI_COMM_WORLD);
        double start_time = MPI_Wtime();

        // broadcast the entire array from root to the other processors
        MPI_Bcast(full_vector, total_elements, MPI_INT, 0, MPI_COMM_WORLD);

        // each process runs an internal benchmark
        int start_index = rank * local_chunk_size;
        int end_index = start_index + local_chunk_size;
        
        int local_sum = 0;
        for (int i = start_index; i < end_index; i++) {
                local_sum += full_vector[i];
        }

        int global_sum = 0;
        MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        double end_time = MPI_Wtime();

        if (rank == 0) {
                printf("BCAST Sum: %d | Time: %f seconds\n", global_sum, end_time - start_time);
        }

        free(full_vector);
        MPI_Finalize();
        return 0;
}

int check_args(int argc, char **argv) {
        if (argc != 2) {
                fprintf(stderr, "ERROR: Provide total elements!\n");
                exit(-1);
        }
        return atoi(argv[1]);
}
