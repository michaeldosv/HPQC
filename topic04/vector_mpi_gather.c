#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int check_args(int argc, char **argv);

// base code same as previously seen
int main(int argc, char **argv) {
        int rank, size;
        MPI_Init(&argc, &argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);

        int total_elements = check_args(argc, argv);
        int local_chunk_size = total_elements / size;

        int* local_vector = malloc(local_chunk_size * sizeof(int));

        int start_index = rank * local_chunk_size;
        for (int i = 0; i < local_chunk_size; i++) {
                local_vector[i] = start_index + i;
        }

        MPI_Barrier(MPI_COMM_WORLD);
        double start_time = MPI_Wtime();

        int local_sum = 0;
        for (int i = 0; i < local_chunk_size; i++) {
                local_sum += local_vector[i];
        }

        int global_sum = 0;
        int* gathered_sums = NULL;

        // root allocates an array to hold the incoming sums from all processes
        if (rank == 0) {
                gathered_sums = malloc(size * sizeof(int));
        }

        // use MPI_Gather() to allow root process to loop over the vector array chunks sent by the client processes
        MPI_Gather(&local_sum, 1, MPI_INT, gathered_sums, 1, MPI_INT, 0, MPI_COMM_WORLD);

        if (rank == 0) {
                for (int i = 0; i < size; i++) {
                        global_sum += gathered_sums[i];
                }
                free(gathered_sums);
        }

        double end_time = MPI_Wtime();

        if (rank == 0) {
                printf("MPI_Gather Sum: %d\n", global_sum);
                printf("Time : %f seconds\n", end_time - start_time);
        }

        free(local_vector);
        MPI_Finalize();
        return 0;
}

int check_args(int argc, char **argv) {
        if (argc == 2) return atoi(argv[1]);
        fprintf(stderr, "ERROR: Provide total elements!\n");
        exit(-1);
}
