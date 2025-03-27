#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <omp.h>

#define SIZE 1000  
#define ROOT 0

int main(int argc, char* argv[]) {
    int proc_id, num_procs, segment_size;
    int array[SIZE], local_array[SIZE];

    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &proc_id);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    segment_size = SIZE / num_procs;  

    if (proc_id == ROOT) {
        printf("Original Data: ");
        for (int i = 0; i < SIZE; i++) {
            array[i] = i + 1;
            printf("%d\n", array[i]);
        }
        printf("\n");
    }

    MPI_Scatter(array, segment_size, MPI_INT, local_array, segment_size, MPI_INT, ROOT, MPI_COMM_WORLD);

    #pragma omp parallel for
    for (int i = 0; i < segment_size; i++) {
        local_array[i] *= 2;  
    }

    MPI_Gather(local_array, segment_size, MPI_INT, array, segment_size, MPI_INT, ROOT, MPI_COMM_WORLD);

    if (proc_id == ROOT) {
        printf("Processed Data: ");
        for (int i = 0; i < SIZE; i++) {
            printf("%d\n", array[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
