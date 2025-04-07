// Lab2 Task2
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "queue.h"

// Thread-safe output with mutex
pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;

// Arguments for threads
typedef struct
{
	int thread_id;
} thread_args_t;

// Producer thread function
void *producer_thread(void *arg)
{
	int t, N, res;
	FILE *fp = fopen("./data/task2.txt", "r");
	if (fp == NULL)
	{
		printf("Error opening file\n");
		exit(1);
	}
	fscanf(fp, "%d", &N);
	for (t = 0; t < N; t++)
	{
		MMData data; // load the MMData
		load_MMData(fp, &data);
		// add to queue
		res = queue_add(data);
		if (res == -1)
			printf("queue_add error\n");
	}
	queue_close();
	fclose(fp);
	return NULL;
}

// Consumer thread function
void *consumer_thread(void *arg)
{
	thread_args_t *args = (thread_args_t *)arg;
	int worker_id = args->thread_id;

	MMData ret;
	while (queue_get(&ret) == 0)
	{
		mm_compute(&ret);

		// Thread-safe output using mutex lock
		pthread_mutex_lock(&output_mutex);
		print_MMData(&ret);
		pthread_mutex_unlock(&output_mutex);
	}

	return NULL;
}

int main()
{
	queue_init();

	// Set up threads
	pthread_t producer;
	const int num_workers = 8; // You can adjust based on performance tests
	pthread_t workers[num_workers];
	thread_args_t worker_args[num_workers];

	// Create producer thread
	if (pthread_create(&producer, NULL, producer_thread, NULL) != 0)
	{
		fprintf(stderr, "Error creating producer thread\n");
		return 1;
	}

	// Create consumer/worker threads
	for (int i = 0; i < num_workers; i++)
	{
		worker_args[i].thread_id = i;
		if (pthread_create(&workers[i], NULL, consumer_thread, &worker_args[i]) != 0)
		{
			fprintf(stderr, "Error creating worker thread %d\n", i);
			return 1;
		}
	}

	// Wait for producer to finish
	pthread_join(producer, NULL);

	// Wait for all workers to finish
	for (int i = 0; i < num_workers; i++)
	{
		pthread_join(workers[i], NULL);
	}

	// Clean up mutex
	pthread_mutex_destroy(&output_mutex);

	return 0;
}