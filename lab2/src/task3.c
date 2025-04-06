// Lab2 Task3
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "queue.h"


void compute(){
    // This is a naive implementation for demo the input and output, you can change anything.
    // !! In task3, You should read the next data AFTER printing the last one
    int t,  N, res;
    FILE* fp=fopen("./data/task3.txt", "r");
    if(fp==NULL){
        printf("Error opening file\n");
        exit(1);
    }
    fscanf(fp, "%d", &N);
    for(t=0; t<N; t++){
        MMData data;// fill the MMData

        // !! In task3, You should read the next data AFTER printing the last one
        load_MMData(fp, &data);

        // add to queue
        res = queue_add(data);
        if(res == -1)
            printf("queue_add error\n"); // this should never happen
        
        // get from queue
        MMData ret;
        queue_get(&ret);
        // compute
        mm_compute(&ret);

        print_MMData(&ret);
    }
    queue_close();
    fclose(fp);
}

// you have to modify main to allow multi-threading.
int main(){
    queue_init();
    
    compute();

    return 0;
}