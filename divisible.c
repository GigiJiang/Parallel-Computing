#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

int *divisible_arr(int A, int x, int n){
    int *my_arr = (int *)calloc(n, sizeof(int));
    for(int i = 0; i < n; i++){
        my_arr[i] = A + i * x;
    }
    return my_arr;
}

int main (int argc, char **argv) {
    FILE *fp; //for creating the output file
    char filename[100] = ""; // the file name
    unsigned int x, A, B;
    double start_p1, end_p1, start_p2, end_p2;
    double time_for_p1, time_max_p1;

    A = (unsigned int) atoi(argv[1]);
    B = (unsigned int) atoi(argv[2]);
    x = (unsigned int) atoi(argv[3]);

    int *res;
    int local_n;
    int *my_arr = NULL;

    if(A % x != 0){
        A = (A / x + 1) * x;
    }
    if(B % x != 0){
        B = (B/x) * x;
    }

    int numbers;
    if(B-A < x){
        numbers = 0;
    }else{
        numbers = (B - A) / x + 1;
    }
//part1 ------------------------------------------------------------
    start_p1 = clock();
    start_p1 = MPI_Wtime();

    MPI_Init(&argc,&argv);
    int rank, pn;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &pn);

    if(numbers % pn == 0){
        local_n = numbers / pn;
    }else {
        local_n = numbers / pn + 1;
    }
    if(local_n == 0) local_n = 1;

    int range = local_n * x;

    if (rank == 0) {
        res = (int *) calloc(local_n * pn + 3, sizeof(int));
    }

    my_arr = divisible_arr(A + rank * range, x, local_n);
    MPI_Gather(my_arr, local_n, MPI_INT, res, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    free(my_arr);
    end_p1 = MPI_Wtime();
    time_for_p1 = end_p1 - start_p1;
    MPI_Reduce(&time_for_p1, &time_max_p1, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
    MPI_Finalize();

//part2 ------------------------------------------------------------

    start_p2 = MPI_Wtime();
    if (rank == 0) {
        strcpy(filename, argv[2]);
        strcat(filename, ".txt");

        if (!(fp = fopen(filename, "w+t"))) {
            printf("Cannot create file %s\n", filename);
            exit(1);
        }

        for (int i = 0; i < numbers; i++) {
            fprintf(fp, "%d\n", res[i]);
        }

        fclose(fp);

        free(res);

        end_p2 = MPI_Wtime();

        printf("time of part1 = %lf s    part2 = %lf s\n",
               (double) (time_max_p1),
               (double) (end_p2 - start_p2));
    }
    return 0;
}


