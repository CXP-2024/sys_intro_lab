// Lab2 Task2
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "queue.h"

void producer(){
    int t, N, res;
    FILE* fp=fopen("./data/task2.txt", "r");
    if(fp==NULL){
        printf("Error opening file\n");
        exit(1);
    }
    fscanf(fp, "%d", &N);
    for(t=0; t<N; t++){
        MMData data;// load the MMData
        load_MMData(fp, &data);
        // add to queue
        res = queue_add(data);
        if(res == -1)
            printf("queue_add error\n");
    }
    queue_close();
    fclose(fp);
}

void consumer(){
    MMData ret;
    while(queue_get(&ret) == 0){
        mm_compute(&ret);
        // you may need a mutex lock output
        print_MMData(&ret);
    }
}

// you have to modify main to allow multi-threading.
int main(){
    queue_init();

    producer(); // Note: Since there is only one thread, when queue is full, this naive implementation will cause infinitely waiting
    consumer();

    return 0;
}