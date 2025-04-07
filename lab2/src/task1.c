// Lab2 Task1
#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include <pthread.h>

#include "task1.h" // TASK1_TASKS_NUM

// Mutex for protecting stdout
pthread_mutex_t print_mutex;

// Thread function for producer
void *producer_thread(void *arg)
{
	int i, res;
	for (i = 0; i < TASK1_TASKS_NUM; i++)
	{
		MMData data;
		data.task_id = i;
		res = queue_add(data);
		if (res == -1)
			printf("queue_add error\n"); // this should never happen
	}
	queue_close(); // send queue_close()
	return NULL;
}

// Thread function for consumer
void *consumer_thread(void *arg)
{
	MMData ret;
	while (queue_get(&ret) == 0)
	{
		// Use mutex lock for output to avoid interleaving
		pthread_mutex_lock(&print_mutex);
		printf("%d\n", ret.task_id);
		pthread_mutex_unlock(&print_mutex);
	}
	return NULL;
}

// You have to modify main to allow multi-threading.
int main()
{
	// Number of consumer threads
	const int NUM_CONSUMERS = 2;

	// Initialize the queue
	queue_init();

	// Initialize the print mutex
	pthread_mutex_init(&print_mutex, NULL);

	// Create thread handles
	pthread_t producer;
	pthread_t consumers[NUM_CONSUMERS];

	// Create the producer thread
	if (pthread_create(&producer, NULL, producer_thread, NULL) != 0)
	{
		fprintf(stderr, "Error creating producer thread\n");
		return 1;
	}

	// Create consumer threads
	for (int i = 0; i < NUM_CONSUMERS; i++)
	{
		if (pthread_create(&consumers[i], NULL, consumer_thread, NULL) != 0)
		{
			fprintf(stderr, "Error creating consumer thread %d\n", i);
			return 1;
		}
	}

	// Wait for producer to finish
	pthread_join(producer, NULL);

	// Wait for all consumers to finish
	for (int i = 0; i < NUM_CONSUMERS; i++)
	{
		pthread_join(consumers[i], NULL);
	}

	// Clean up
	pthread_mutex_destroy(&print_mutex);

	return 0;
}