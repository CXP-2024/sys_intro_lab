// naive implementation
#include "mm.h" // MMData
#include "queue.h"

struct MMQueue{
    // Circular Queue
    MMData data[MAX_QUEUE_LEN];
    // head, tail of this queue
    int h, t; 
    // closed means no more data can be added
    int closed;
}QData;

void queue_init(){
    // init/reset the queue
    QData.h = 0;
    QData.t = 0;
    QData.closed = 0;
}

int queue_get(MMData *ret){
    // this naive implementation did not support block when queue is empty, just return -1
    if(QData.h >= QData.t) return -1;
    int h = QData.h;
    QData.h++;
    *ret = QData.data[h%MAX_QUEUE_LEN]; // circular use the QData.data
    return 0; // 0 means success
}

int queue_add(MMData data){
    printf("MAX LEN = %d", (MAX_QUEUE_LEN));
    // this naive implementation did not support block when queue is full, just return -1
    if(QData.t - QData.h >= MAX_QUEUE_LEN) return -1;
    if(QData.closed) return -1;
    int t = QData.t;
    QData.t++;
    QData.data[t%MAX_QUEUE_LEN] = data;
    return 0; // 0 means success
}

void queue_close(){
    QData.closed = 1;
}
