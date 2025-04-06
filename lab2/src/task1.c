// Lab2 Task1
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <pthread.h>

#include "task1.h" // TASK1_TASKS_NUM


// naive implementation
void producer(){
    int i, res;
    for(i=0; i<TASK1_TASKS_NUM; i++){
        MMData data;
        data.task_id = i;
        res = queue_add(data);
        if(res == -1)
            printf("queue_add error\n"); // this should never happen
    }
    queue_close(); // send queue_close()
}

// naive implementation
void consumer(){
    MMData ret;
    while(queue_get(&ret) == 0){
        // you may need a mutex lock for output
        printf("%d\n", ret.task_id);
    }
}

// you have to modify main to allow multi-threading.
int main(){
    queue_init();
    
    producer(); // Note: Since there is only one thread, when queue is full, this naive implementation will cause infinitely waiting
    consumer();

    return 0;
}