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

        int* full_vector = NULL;
        int* local_vector = malloc(local_chunk_size * sizeof(int));

        // only root allocates and initializes the full array
        if (rank == 0) {
                full_vector = malloc(total_elements * sizeof(int));
                for (int i = 0; i < total_elements; i++) {
                        full_vector[i] = i;
                }
        }

        MPI_Barrier(MPI_COMM_WORLD);
        double start_time = MPI_Wtime();

        // disperse chunks from root's vector to each processor
        MPI_Scatter(full_vector, local_chunk_size, MPI_INT, 
                    local_vector, local_chunk_size, MPI_INT, 
                    0, MPI_COMM_WORLD);

        int local_sum = 0;
        for (int i = 0; i < local_chunk_size; i++) {
                local_sum += local_vector[i];
        }

        int global_sum = 0;
        MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        double end_time = MPI_Wtime();

        if (rank == 0) {
                printf("SCATTER Sum: %d | Time: %f seconds\n", global_sum, end_time - start_time);
                free(full_vector); // root frees up memory
        }

        free(local_vector); // each processor frees up memory
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
