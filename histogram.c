#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <omp.h>

int n;
double histogram[100000100];
int bin[101];

int main(int argc, char **argv){
    int b = atoi(argv[1]);
    int thread = atoi(argv[2]);
    double start; 
    double end; 

    char filename[100] = "";
    FILE * fp;
    strcpy(filename, argv[3]);
    fp = fopen(filename,"r");
    if(fp == NULL){
        exit(1);
    }
    fscanf(fp,"%d", &n);
    for(int i = 0; i < n; i++){
        fscanf(fp,"%lf", &histogram[i]);
    }
    double interval = 100/b;
    int** local_histogram = (int **)calloc(thread,sizeof(int *));


    
    #pragma omp parallel num_threads(thread)
    {
        int tid = omp_get_thread_num();
        #pragma omp for
        for(int i = 0; i<thread;i++){
            local_histogram[i] = (int*) calloc(b,sizeof(int));
        }
        start = omp_get_wtime(); 
         #pragma omp for
        for (int i = 0; i<n; i++){
            int pos = ((int)histogram[i]/interval);
           // printf("i = %d,histogram[i] = %lf,((int)histogram[i] / interval) = %d\n",i,histogram[i],((int)(histogram[i] / interval)) );
            local_histogram[tid][pos]++;
        //      printf("%d\n",tid); 
        }

        #pragma omp for
        for(int i = 0; i<b; i++){
            for(int j = 0; j<thread; j++){
        //printf("j = %d,i = %d,local = %d\n",j,i, local_histogram[j][i]);          
      bin[i] += local_histogram[j][i];
            }
        }
    }

    end = omp_get_wtime(); 
    for(int i = 0; i<b; i++){
        printf("bin[%d]:%d\n",i,bin[i]);
    }
       printf("Work took %f seconds\n", end - start);
    return 0;
}
