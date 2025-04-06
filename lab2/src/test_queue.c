#include <pthread.h>
#include <stdlib.h>  
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "test_queue.h"
#include "queue.h"

int check_flag(int flag, const char * s){
    if(flag != 0){
        printf("\033[0;31mFAILED:\033[0m %s\n", s);
        return -1;
    }else{
        printf("\033[0;32mPASSED:\033[0m %s\n", s);
        return 0;
    }
}

int main(){
    fputs("[Warining] wrong implementation can lead to infinite loop... \n", stderr);
    
    test_queue_basic();
    test_queue_close();
    test_queue_multithread_basic();
    test_queue_multithread_fifo();
    test_queue_multithread_full();
    test_queue_multithread_get_before_add();
    test_queue_multithread_busywaiting();

    return 0;
}

int test_queue_basic_1(int n);
int test_queue_basic_2(int n);

int test_queue_basic(){
    // puts("Testing test_queue_basic ...");
    // test queue_init, queue_add, queue_get
    // guarantee no empty get
    int flag = 0;
    // 1. get is the same as add, add all, get all
    // flag |= test_queue_basic_1(0);
    flag |= test_queue_basic_1(MAX_QUEUE_LEN);
    // 2. random add and get, no empty get
    // flag |= test_queue_basic_2(0);
    flag |= test_queue_basic_2(MAX_QUEUE_LEN);
    return check_flag(flag, "test_queue_basic");
}

int test_queue_close_1(int n);
int test_queue_close_2(int n);
int test_queue_close_3(int n);

int test_queue_close(){
    // puts("Testing test_queue_close ...");
    // test queue_close
    // has empty get
    int flag = 0;
    // 1. closed, get should return -1
    flag |= test_queue_close_1(MAX_QUEUE_LEN);
    // 2. closed, add should return -1
    flag |= test_queue_close_2(MAX_QUEUE_LEN);
    // 3. if no close, get should waiting & close can stop this waiting
    flag |= test_queue_close_3(MAX_QUEUE_LEN);
    return check_flag(flag, "test_queue_close");
}

int test_queue_multithread_basic_1(int n);

int test_queue_multithread_basic(){
    // puts("Testing test_queue_multithread_basic ...");
    // multi-threading add, get (no empty get)
    // check correction
    int flag = 0;
    
    flag |= test_queue_multithread_basic_1(10);
    flag |= test_queue_multithread_basic_1(100);
    flag |= test_queue_multithread_basic_1(TEST_MAX_ADD_NUM);

    return check_flag(flag, "test_queue_multithread_basic");
}

int test_queue_multithread_fifo_1(int n);

int test_queue_multithread_fifo(){
    // puts("Testing test_queue_multithread_fifo ...");
    
    // two threading, one add, one get, no empty get
    // check fifo
    int flag = 0;

    flag |= test_queue_multithread_fifo_1(10);
    flag |= test_queue_multithread_fifo_1(100);
    flag |= test_queue_multithread_fifo_1(TEST_MAX_ADD_NUM);

    return check_flag(flag, "test_queue_multithread_fifo");
}

int test_queue_multithread_full_1(int n);

int test_queue_multithread_full(){
    int flag = 0;
    flag |= test_queue_multithread_full_1(MAX_QUEUE_LEN);

    return check_flag(flag, "test_queue_multithread_full");
}

int test_queue_multithread_get_before_add_1(int n);

int test_queue_multithread_get_before_add(){
    // puts("Testing test_queue_multithread_get_before_add ...");
    // multi-threading, get before add
    // check correction
    int flag = 0;

    flag |= test_queue_multithread_get_before_add_1(10);
    flag |= test_queue_multithread_get_before_add_1(100);
    flag |= test_queue_multithread_get_before_add_1(TEST_MAX_ADD_NUM);

    return check_flag(flag, "test_queue_multithread_get_before_add");
}

int test_queue_multithread_busywaiting_1(int n);

int test_queue_multithread_busywaiting(){
    // puts("Testing test_queue_multithread_busywaiting ...");
    // check no busy waiting
    // multi-thread get then sleep, check CPU usage
    int flag = 0;

    flag |= test_queue_multithread_busywaiting_1(10);
    // flag |= test_queue_multithread_busywaiting_1(100);
    // flag |= test_queue_multithread_busywaiting_1(TEST_MAX_ADD_NUM);

    return check_flag(flag, "test_queue_multithread_busywaiting");
}

// ----------------------------------------------------------------

int MMData_not_same(MMData *p, MMData *q, int n){
    int i;
    for(i=0;i<n;i++){
        if(p[i].m != q[i].m || p[i].k != q[i].k|| p[i].n != q[i].n ){
            return 1;
        }
        if(p[i].X != q[i].X || p[i].Y != q[i].Y  || p[i].Z != q[i].Z ){
            return 1;
        }
    }
    return 0;
}

void MMData_rand_fill(MMData *p, int n){
    int i;
    for(i=0;i<n;i++){
        p[i].m = rand();
        p[i].k = rand();
        p[i].n = rand();
        p[i].X = NULL;
        p[i].Y = NULL;
        p[i].Z = NULL;
    }
}

int test_queue_basic_1(int n){
    int tt,i, flag = 0;
    MMData *p = malloc(sizeof(MMData) * n);
    MMData *q = malloc(sizeof(MMData) * n);
    for(tt=0; tt<TEST_NUM; tt++){
        // init data
        MMData_rand_fill(p, n);
        // test
        queue_init();
        for(i=0;i<n;i++){
            queue_add(p[i]);
        }
        for(i=0;i<n;i++){
            queue_get(&q[i]);
        }
        queue_close();
        // end test

        if(MMData_not_same(p, q, n)){
            flag = -1;
        }
    }
    free(p);
    free(q);
    return flag;
}


int test_queue_basic_2(int n){
    int tt,i, flag = 0;
    MMData *p = malloc(sizeof(MMData) * n);
    MMData *q = malloc(sizeof(MMData) * n);
    int pcc=0, qcc=0;
    for(tt=0; tt<TEST_NUM; tt++){
        // init data
        MMData_rand_fill(p, n);
        // test
        pcc=0, qcc=0;
        queue_init();
        for(i=0;i<2*n;i++){
            if(pcc < n &&(pcc == qcc || (rand()&1))){
                queue_add(p[pcc++]);
            }else{
                queue_get(&q[qcc++]);
            }
        }
        queue_close();
        // end test

        if(MMData_not_same(p, q, n)){
            flag = -1;
        }
    }
    free(p);
    free(q);
    return flag;
}


int test_queue_close_1(int n){
    int tt,i, flag = 0;
    MMData tmp;
    for(tt=0; tt<TEST_NUM; tt++){
        // test
        queue_init();
        for(i=0;i<n;i++){
            queue_add(tmp);
        }
        queue_close();
        for(i=0;i<n;i++){
            queue_get(&tmp);
        }
        // get after close
        int res = queue_get(&tmp);
        if(res != -1){
            flag = -1;
        }
        // end test
    }
    return flag;
}

int test_queue_close_2(int n){
    int tt,i, flag = 0;
    MMData tmp={};
    for(tt=0; tt<TEST_NUM; tt++){
        // test
        queue_init();
        for(i=0;i<n;i++){
            queue_add(tmp);
        }
        queue_close();
        // for(i=0;i<n;i++){
        //     queue_get(&tmp);
        // }
        // get after close
        int res = queue_add(tmp);
        if(res != -1){
            flag = -1;
        }
        // end test
    }
    return flag;
}

void* test_queue_close_3_func1(void * arg){
    // get
    MMData tmp;
    int res = queue_get(&tmp);
    if(res != 0){
        return (void *)1;
    }else{
        return 0;
    }
    res = queue_get(&tmp);
    if(res != -1){
        return (void *)1;
    }else{
        return 0;
    }
}
void* test_queue_close_3_func2(void * arg){
    // close
    MMData tmp={};
    queue_add(tmp);
    queue_close();
    return NULL;
}

int test_queue_close_3(int n){
    int tt, flag = 0;
    for(tt=0; tt<TEST_NUM; tt++){
        // test
        queue_init();
        pthread_t t1, t2;
        pthread_create(&t1, NULL, test_queue_close_3_func1, NULL);
        usleep(200);
        pthread_create(&t2, NULL, test_queue_close_3_func2, NULL);
        void *ret1, *ret2;
        pthread_join(t1, &ret1);
        pthread_join(t2, &ret2);
        // end test

        // get after close
        if(ret1 != 0){
            flag = -1;
        }
    }
    return flag;
}

#define TEST_THREAD_NUM 4

typedef struct{
    MMData *p;
    MMData *q;
    int start;
    int end;
}test_queue_multithread_basic_1_args;

void* test_queue_multithread_basic_1_func(void * arg){
    test_queue_multithread_basic_1_args *args = (test_queue_multithread_basic_1_args*)arg;
    int i;
    for(i=args->start;i< args->end;i++){
        queue_add(args->p[i]);
        queue_get(&args->q[i]);
    }
    return NULL;
}

int MMData_not_same_shuffled(MMData *p, MMData *q, int n){
    int i,j; // O(n^2) check
    int vis[TEST_MAX_ADD_NUM]={};
    for(i=0;i<n;i++){
        for(j=0;j<n;j++){
            if(vis[j] == 0 && !MMData_not_same(&p[i], &q[j], 1)){
                vis[j] = 1;
                break;
            }
        }
    }
    for(i=0;i<n;i++){
        if(vis[i] == 0){
            return 1;
        }
    }
    return 0;
}

int test_queue_multithread_basic_1(int n){
    int tt,i, flag = 0;
    MMData *p = malloc(sizeof(MMData) * n);
    MMData *q = malloc(sizeof(MMData) * n);
    for(tt=0; tt<TEST_NUM*10; tt++){
        // init data
        MMData_rand_fill(p, n);
        // test
        queue_init();
        test_queue_multithread_basic_1_args args[TEST_THREAD_NUM];
        pthread_t t[TEST_THREAD_NUM];
        for(i=0;i<TEST_THREAD_NUM;i++){
            args[i].p = p;
            args[i].q = q;
            args[i].start = i*n/TEST_THREAD_NUM;
            args[i].end = (i+1)*n/TEST_THREAD_NUM;
        }
        args[TEST_THREAD_NUM-1].end = n;
        for(i=0;i<TEST_THREAD_NUM;i++){
            pthread_create(&t[i], NULL, test_queue_multithread_basic_1_func, &args[i]);
        }
        void* ret;
        for(i=0;i<TEST_THREAD_NUM;i++){
            pthread_join(t[i], &ret);
        }

        queue_close();
        // end test
        if(MMData_not_same_shuffled(p, q, n)){
            flag = -1;
        }
    }
    free(p);
    free(q);
    return flag;
}

typedef struct{
    MMData *p;
    int n;
}test_queue_multithread_fifo_args;

void* test_queue_multithread_fifo_func1(void* arg){
    // add
    test_queue_multithread_fifo_args *args = (test_queue_multithread_fifo_args*)arg;
    int i;
    for(i=0;i<args->n;i++){
        queue_add(args->p[i]);
    }
    return NULL;
}

void* test_queue_multithread_fifo_func2(void* arg){
    // get
    test_queue_multithread_fifo_args *args = (test_queue_multithread_fifo_args*)arg;
    int i;
    for(i=0;i<args->n;i++){
        queue_get(&args->p[i]);
    }
    return NULL;
}

int test_queue_multithread_fifo_1(int n){
    int tt, flag = 0;
    MMData *p = malloc(sizeof(MMData) * n);
    MMData *q = malloc(sizeof(MMData) * n);
    for(tt=0; tt<TEST_NUM*10; tt++){
        // init data
        MMData_rand_fill(p, n);
        // test
        queue_init();
        pthread_t t1, t2;
        test_queue_multithread_fifo_args arg1={p, n}, arg2={q, n};
        pthread_create(&t1, NULL, test_queue_multithread_fifo_func1, &arg1);
        pthread_create(&t2, NULL, test_queue_multithread_fifo_func2, &arg2);
        void *ret1, *ret2;
        pthread_join(t1, &ret1);
        pthread_join(t2, &ret2);
        queue_close();
        // end test
        if(MMData_not_same(p, q, n)){
            flag = -1;
        }
    }
    free(p);
    free(q);
    return flag;
}

int test_queue_multithread_full_1(int n){
    int tt, flag = 0;
    n++;
    MMData *p = malloc(sizeof(MMData) * n);
    MMData *q = malloc(sizeof(MMData) * n);
    for(tt=0; tt<TEST_NUM; tt++){
        // init data
        MMData_rand_fill(p, n);
        // test
        queue_init();
        pthread_t t1, t2;
        test_queue_multithread_fifo_args arg1={p, n}, arg2={q, n};
        pthread_create(&t1, NULL, test_queue_multithread_fifo_func1, &arg1);
        usleep(2000);
        pthread_create(&t2, NULL, test_queue_multithread_fifo_func2, &arg2);
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        queue_close();
        // end test
        if(MMData_not_same(p, q, n)){
            flag = -1;
        }
    }
    free(p);
    free(q);
    return flag;
}

int test_queue_multithread_get_before_add_1(int n){
    int tt, flag = 0;
    MMData *p = malloc(sizeof(MMData) * n);
    MMData *q = malloc(sizeof(MMData) * n);
    for(tt=0; tt<TEST_NUM; tt++){
        // init data
        MMData_rand_fill(p, n);
        // test
        queue_init();
        pthread_t t1, t2;
        test_queue_multithread_fifo_args arg1={p, n}, arg2={q, n};
        pthread_create(&t2, NULL, test_queue_multithread_fifo_func2, &arg2);
        usleep(200);
        pthread_create(&t1, NULL, test_queue_multithread_fifo_func1, &arg1);
        pthread_join(t1, NULL);
        pthread_join(t2, NULL);
        queue_close();
        // end test
        if(MMData_not_same(p, q, n)){
            flag = -1;
        }
    }
    free(p);
    free(q);
    return flag;
}

void* test_queue_multithread_busywaiting_1_func(void * arg){
    MMData tmp;
    while(queue_get(&tmp)==0){
        ;
    }
    return NULL;
}
int test_queue_multithread_busywaiting_1(int n){
    int tt,i, flag = 0;
    for(tt=0; tt<TEST_NUM; tt++){// 5 times
        // test
        queue_init();
        pthread_t t[4];
        for(i=0; i < 4; i++){
            pthread_create(&t[i], NULL, test_queue_multithread_busywaiting_1_func, NULL);
        }
        usleep(2000);
        char s[100];
        sprintf(s, "[ $(ps -T -o stat -p %d | grep R |wc -l) -gt 2 ]", getpid());
        if(system(s)==0){
            flag = -1;
        }
        
        queue_close();
        for(i=0;i<TEST_THREAD_NUM; i++){
            pthread_join(t[i], NULL);
        }
        // end test

    }
    return flag;
}

