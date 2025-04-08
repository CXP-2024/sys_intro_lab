// Lab2 Task3
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "queue.h"

// Mutex for thread-safe output
pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;

// Producer-consumer thread functions
void *producer_func(void *arg);
void *consumer_func(void *arg);

// Thread arguments
typedef struct
{
	int id;
} thread_args_t;

// Main compute function (multi-threaded version)
void compute()
{
	// Initialize queue
	queue_init();

	// Create producer thread
	pthread_t producer;
	if (pthread_create(&producer, NULL, producer_func, NULL) != 0)
	{
		fprintf(stderr, "Error creating producer thread\n");
		exit(1);
	}

	// Create consumer threads
	const int num_workers = 4;
	pthread_t consumers[num_workers];
	thread_args_t consumer_args[num_workers];

	for (int i = 0; i < num_workers; i++)
	{
		consumer_args[i].id = i;
		if (pthread_create(&consumers[i], NULL, consumer_func, &consumer_args[i]) != 0)
		{
			fprintf(stderr, "Error creating consumer thread %d\n", i);
			exit(1);
		}
	}

	// Join threads
	pthread_join(producer, NULL);
	for (int i = 0; i < num_workers; i++)
	{
		pthread_join(consumers[i], NULL);
	}

	pthread_mutex_destroy(&output_mutex);
}

// Producer function
void *producer_func(void *arg)
{
	int N, res;
	FILE *fp = fopen("./data/task3.txt", "r");
	if (fp == NULL)
	{
		printf("Error opening file\n");
		exit(1);
	}

	fscanf(fp, "%d", &N);

	for (int i = 0; i < N; i++)
	{
		// Process one matrix at a time
		MMData data;
		load_MMData(fp, &data);

		// Add matrix to queue
		res = queue_add(data);
		if (res == -1)
		{
			printf("queue_add error\n");
		}

		// Wait until all consumers have processed their tasks
		// before reading the next matrix - this is handled by
		// queue_get returning when queue becomes empty
	}

	queue_close();
	fclose(fp);
	return NULL;
}

// Consumer function
void *consumer_func(void *arg)
{
	(void)arg; // Avoid unused parameter warning

	MMData matrix;
	while (queue_get(&matrix) == 0)
	{
		// Process the matrix
		mm_compute(&matrix);

		// Print result with mutex protection
		pthread_mutex_lock(&output_mutex);
		print_MMData(&matrix);
		pthread_mutex_unlock(&output_mutex);
	}

	return NULL;
}

// Main function
int main()
{
	compute();
	return 0;
}