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

        if (rank == 0) {
                full_vector = malloc(total_elements * sizeof(int));
                for (int i = 0; i < total_elements; i++) {
                        full_vector[i] = i;
                }
        }

        MPI_Barrier(MPI_COMM_WORLD);
        double start_time = MPI_Wtime();

        if (rank == 0) {
                // root keeps its own vector array
                for(int i = 0; i < local_chunk_size; i++) {
                        local_vector[i] = full_vector[i];
                }

                // root sends the other chunks to specific ranks using pointers
                for (int target_rank = 1; target_rank < size; target_rank++) {
                        int offset = target_rank * local_chunk_size;
                        MPI_Send(full_vector + offset, local_chunk_size, MPI_INT, target_rank, 0, MPI_COMM_WORLD);
                }
        } else {
                // client processors wait ti receive their chunk from root
                MPI_Recv(local_vector, local_chunk_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }

        int local_sum = 0;
        for (int i = 0; i < local_chunk_size; i++) {
                local_sum += local_vector[i];
        }

        int global_sum = 0;
        MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

        double end_time = MPI_Wtime();

        if (rank == 0) {
                printf("DIY Sum: %d | Time: %f seconds\n", global_sum, end_time - start_time);
                free(full_vector);
        }

        free(local_vector);
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
