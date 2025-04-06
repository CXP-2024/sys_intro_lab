#include <stdlib.h>
#include "mm.h"

// You may need to modify the code below to your lab1's code.
void mm_compute(MMData* data){
    int m = data->m;
    int k = data->k;
    int n = data->n;
    int *X = data->X;
    int *Y = data->Y;
    int *Z = data->Z;
    int i, j, l;
    for(i = 0; i < m; i++){
        for(j = 0; j < n; j++){
            Z[i*n+j] = 0;
            for(l = 0; l < k; l++){
                Z[i*n+j] += X[i*k+l] * Y[l*n+j];
            }
        }
    }
}

void load_MMData(FILE* fp, MMData* data){
    int  task_id, m, k, n;
    fscanf(fp, "%d%d%d%d", &task_id, &m, &k, &n);
    data->task_id = task_id;
    data->m = m;
    data->k = k;
    data->n = n;
    
    // use malloc to get the memory space
    data->X = (int*) malloc(sizeof(int)*m*k);
    data->Y = (int*) malloc(sizeof(int)*k*n);
    data->Z = (int*) malloc(sizeof(int)*m*n);
    // remerber to free them after output

    // read X
    for(int i=0; i<m*k; i++){
        fscanf(fp, "%d", &data->X[i]);
    }
    // read Y
    for(int i=0; i<k*n; i++){
        fscanf(fp, "%d", &data->Y[i]);
    }
}

void print_MMData(MMData* data){
    int i,j;
    printf("%d %d %d\n", data->task_id, data->m, data->n);
    // output Z
    for(i=0; i<data->m; i++){
        for(j=0; j<data->n; j++){
            printf("%d ", data->Z[i*data->n+j]);
        }
        printf("\n");
    }
    // free memory
    free(data->X);
    free(data->Y);
    free(data->Z);
}