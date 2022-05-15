#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <omp.h>

int n;
double histogram[10000000];
int bin[101];
double start; 
double end; 

int main(int argc, char **argv){
    int b = atoi(argv[1]);
    char filename[100] = "";
    FILE * fp;
    strcpy(filename, argv[2]);
    fp = fopen(filename,"r");
    if(fp == NULL){
        printf("file error\n");
        exit(1);
    }
    fscanf(fp,"%d", &n);
    // printf("n:%d\n",n);
    for(int i = 0; i < n; i++){
        fscanf(fp,"%lf", &histogram[i]);
    }

    double interval = 100/b;
    start = omp_get_wtime(); 
    for (int i = 0; i<n; i++){
        double read_num = histogram[i];
        int pos = read_num/interval;
        // printf("num:%lf\t,pos:%d\n",read_num,pos);
        bin[pos] ++;
    }
    end = omp_get_wtime(); 
    printf("Work took %f seconds\n", end - start);
    for(int i = 0; i<b; i++){
        printf("bin[%d]:%d\n",i,bin[i]);
    }
    return 0;
}