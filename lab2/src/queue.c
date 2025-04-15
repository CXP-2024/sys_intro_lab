// naive implementation
#include "mm.h" // MMData
#include "queue.h"
#include <pthread.h>

struct MMQueue
{
	// Circular Queue
	MMData data[MAX_QUEUE_LEN];
	// head, tail of this queue
	int h, t;
	// closed means no more data can be added
	int closed;
	// Add synchronization primitives
	pthread_mutex_t mutex;
	pthread_cond_t not_empty;
	pthread_cond_t not_full;
} QData;

void queue_init()
{
	// init/reset the queue
	QData.h = 0;
	QData.t = 0;
	QData.closed = 0; // only set to 1 when closed and only to avoid adding data but can still get data until no data left

	// Initialize synchronization primitives
	pthread_mutex_init(&QData.mutex, NULL);
	pthread_cond_init(&QData.not_empty, NULL);
	pthread_cond_init(&QData.not_full, NULL);
}

int queue_get(MMData *ret)
{
	pthread_mutex_lock(&QData.mutex);

	// Wait if queue is empty and not closed
	while (QData.h >= QData.t && !QData.closed)
	{
		pthread_cond_wait(&QData.not_empty, &QData.mutex);
	}

	// If queue is empty and closed, return -1
	// this will not happen, since if closed and called queue_get, which only happens when there is still data in the queue
	// but just in case
	if (QData.h >= QData.t)
	{
		pthread_mutex_unlock(&QData.mutex);
		return -1;
	}

	int h = QData.h;
	QData.h++;
	*ret = QData.data[h % MAX_QUEUE_LEN]; // circular use the QData.data

	// Signal that the queue is not full anymore
	pthread_cond_signal(&QData.not_full);
	pthread_mutex_unlock(&QData.mutex);

	return 0; // 0 means success
}

int queue_add(MMData data)
{
	pthread_mutex_lock(&QData.mutex);

	// Check if queue is closed
	if (QData.closed)
	{
		pthread_mutex_unlock(&QData.mutex);
		return -1;
	}

	// Wait if queue is full
	while (QData.t - QData.h >= MAX_QUEUE_LEN && !QData.closed)
	{
		pthread_cond_wait(&QData.not_full, &QData.mutex);
		// Check again if queue was closed while waiting
		if (QData.closed)
		{
			pthread_mutex_unlock(&QData.mutex);
			return -1;
		}
	}

	int t = QData.t;
	QData.t++;
	QData.data[t % MAX_QUEUE_LEN] = data;

	// Signal that the queue is not empty anymore
	pthread_cond_signal(&QData.not_empty);
	pthread_mutex_unlock(&QData.mutex);

	return 0; // 0 means success
}

void queue_close()
{
	pthread_mutex_lock(&QData.mutex);
	QData.closed = 1;

	// Signal all waiting threads
	// however, we don't need to signal not_empty and not_full, since when closed, those workers(if still exist) are wake already
	// and will exit after they finish their work
	// but just in case, we signal them
	pthread_cond_broadcast(&QData.not_empty);
	pthread_cond_broadcast(&QData.not_full);

	pthread_mutex_unlock(&QData.mutex);
}
