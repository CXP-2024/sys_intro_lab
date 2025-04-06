#ifndef TEST_QUEUE_H
#define TEST_QUEUE_H

#include "mm.h" // MMData
#include <time.h>
#include <inttypes.h>

// task1
int test_queue_basic();
int test_queue_close();
int test_queue_multithread_basic();
int test_queue_multithread_fifo();
int test_queue_multithread_full();
int test_queue_multithread_get_before_add();
int test_queue_multithread_busywaiting(); // linux only

#define TEST_MAX_ADD_NUM 1024
#define TEST_NUM 100

#endif // TEST_QUEUE_H