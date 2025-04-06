# Lab2

You will extend your code from Lab 1 to run multiple tasks concurrently in this Lab. Running multiple tasks will allow you to utilize the cores on your Raspberry Pi fully. The Lab intends to allow you experience the entire process of changing a single thread program into a multi-thread version. Four tasks in this Lab build on top of each other. The final goal of the Lab is to produce Task 3 code - multiply a large matrix using multiple cores concurrently. Tasks 1 and 2 help you build the final concurrent program step-by-step. We will evaluate the performance boost from your last Lab (single-core) to this one on the running time of Task 3 code.

## Task1: Implement the Task Queue

Recall that we presented a multi-threaded producer-consumer queue data structure in the lecture. You will need to implement a thread-safe queue. Each item in the queue describes a task, i.e., multiply two matrices using your Lab 1 program on a single thread (on a single core).
The queue must be thread-safe in that multiple producers and consumers can access the queue concurrently, and you should avoid busy waiting.

You can simplify the problem as follows: there is a single producer (called the `task scheduler`). It is a scheduler thread that creates empty `MMData` structs (with only the `task_ID` field assigned while leaving other fields undefined.), and add them to the queue. The `task_ID` should start from 0 and monotonically increas by one for each task. The total number of tasks is `TASK1_TASKS_NUM` defined in `task1.h`. When the scheduler has added all `MMData` into the queue, it should call `queue_close()` to close the queue (closing the queue means no more tasks arrive). There are multiple consumer threads (called the `workers`). Each takes out the head of the queue, performs the computation on a single CPU core, and writes out the results. For task 1, you do NOT have to implement the actual multiplications (left for task 2), and just taking out the task and printing out the task ID is ok (one task ID per line, to stdout).

The queue should have a `max_length` defined by `MAX_QUEUE_LEN` in `queue.h`. When the queue is full, the caller thread needs to wait. Likewise, if the queue is empty, a `dequeue()` call should make the caller thread wait. Again, you should not allow busy waiting.

We will check your printout for correctness. Note that the actual order in which the results are printed is NOT important. However, you need to avoid having different tasks interleaving their results in the printing (another synchronization requirement).

We have provided a naive implementation (without using multi-threads) in `task1.c` and `queue.c` to demonstrate the input/output format. You can build up your scheduler, worker, and queue from this starting point.

### Queue API you need to implement

* `MAX_QUEUE_LEN` defines the max length of the queue
* `queue_init();` initiates or resets the queue
* `int queue_add(MMData data);` adds a new task to the queue; `MMData` describes the matrix multiplication task in `mm.h`; return 0 on success, or -1 on failure (queue is closed)
* `int queue_get(MMData *ret);` gets the head task from the queue, and dequeues it; return 0 on success, -1 on failure (buffer is empty and queue is closed)
* `queue_close();` closes the queue, and on closing, this queue should no longer receive incoming tasks

### Notes

* You can use any of the synchronization primitives in [`pthread.h`](https://man7.org/linux/man-pages/man0/pthread.h.0p.html) to build your scheduler, worker, and queue.
* You can use `make task1` to build the program.
* You can use `make task1_queue_check` to check your queue implementation. (This check program only checks some basic queue behavior to make sure you get your basic interface correct).
* You can use `make task1_grade` to check the correctness of the output.

### Grading (10pts + 10pts)

For task1, there is no performance evaluation. 

1. 10 pts: If you passed all the test cases in `./task1_queue_check`, you can get 10 pts, otherwise you get 0 pts.
2. 10 pts: We will run your program using different `TASK1_TASKS_NUM` and `MAX_QUEUE_LEN`, and check the correctness of the output. Your program should start with one producer thread, and multiple (at least two) consumer threads, and output the correct answer, one `task_id` per line with no interleaving, duplication or missing entries.

## Task2: Adding Worker Code

In this task, you need to finish the consumer threads (here, we call them worker threads) and compute the matrix multiplication results. You also need to change your producer (scheduler thread). In addition to the task_ID, you should read the input file `./data/task2.txt` and fill in the matrix information.

We have defined the single-core matrix multiplication function `mm_compute()` in `mm.c`. Optionally, you can use your Lab 1 code for the multiplication (Note that this time the matrix is stored in 1D (row-major) so you may need some modification), or use our reference implemenation of the single thread multiplication (not optimal). You can vary the number of worker threads to achieve good performance (defined as throughput or the number of multiplications per minute) on your Raspberry Pi. You should write the multiplication results, together with its task ID, to stdout.
Hint: optimizing the throughput is more or less equivalent to maximizing the CPU utilization rate (avoid waiting). Recall that you can monitor the CPU utilization using shell tools (`top` or `htop`). Of course, you should not allow any busy waiting.

We will set the `MAX_QUEUE_LEN` to a very small value, say, 2, so you need to consider the full queue case. Note that you do not have to change any queue implementation from Task 1.

Use `make gen_data` you can generate the input files in `./data/`. You can use `df -h` to make sure that you have enough free disk space, as the generated data will use about 200 MB (if it exceeds your free space, please refer to lab0 to expand your disk space). 
The `task2.c` demonstrates the input and output format of this task. You may need to merge it with the code from the task1.

Note that you still need to avoid interleaving outputs from different threads. However, as each output item can be large, you may need to optimize the synchronization on output to avoid blocking other worker threads for extended periods. For example, but not necessary, you can consider reusing your queue again for output (e.g., each worker can also act as a producer and add one output worker thread to format the results into text and output, in this case).
Kindly remind you that you can read-in the `stdout` using shell redirecting `>` for testing.
You can use the [`time` command] (https://man7.org/linux/man-pages/man1/time.1.html) to get the timing statistics of your program, or use `top` to monitor the resource utilization while the program is running.  Ideally, you should get a CPU utilization close to 400% (as you have four cores), without busy waiting in your code.  

### Notes

* You can use `make gen_data` to prepare the data.
* You can use `make task2` to build the program.
* You can use `time ./task2` to run your program and get the time statistics.
* You can use `make task2_grade` to grade your task2 correctness and performance.

### Grading


You should test different numbers of worker threads. Then you need to plot the trend of speedup vs. the number of worker threads in your report. And try to summarize in your report why you get this result.

To test your program, we may change the rand seed in `data/gen.py`, run your program multiple times and get the average running time.

(Optional) If you implement the (optimized) matrix multiplication in and only in `mm_compute()` in `mm.c`, and we will first test the single-core performance baseline using `mm_compute()` for speedup calculation. 

Parallel speedup is defined as the time used (with I/O time) of the single thread program divided by that of the multi-thread program.  We will grade using parallel speedups using the following rules:

1. If your result is not correct, you will get 0 points;
2. If your result is correct, you can get `(min(P, 2) - 1) * 20` pts, where `P` is the parallel speedup (and we assume that you have `P>1`),  
3. In other words, if your parallel speedup is over 2x, you can get the full 20 pts; if your parallel speedup is 1.5x, you can get 10 pts.  If your parallel speedup is less than 1x, you can get 0 pts.


## Task3: Block Matrix Multiplication Worker

Finally, we come back to the main task of this lab, using multithreading to speed up a single matrix multiplication.

Consider the following plain old block matrix multiplication algorithm (or you can refer to your Linear Algebra textbook) https://en.wikipedia.org/wiki/Block_matrix#Block_matrix_multiplication . Alternatively, you can split the first matrix directly (e.g., by row m, but you need to consider the case when `m = 1`) to turn one matrix multiplication task into multiple tasks.

To accelerate the computation of the large matrix, you want to compute all the blocks fully utilizing the four cores on your Raspberry Pi. Thus, you want the scheduler to read in A and B, partition it into blocks, and submit the blocks to the workers.
 The workers need to output the results into a single matrix `C`. Do you need to synchronize on `C`? Can you avoid most of the synchronizations? You need to describe your design in detail in your report.

Here you can assume that you can hold both the entire input matrix (`A` and `B`) and output matrix (`C`) in memory. However, you hold a single set of `A`, `B`, and `C` in memory simultaneously. To compute on different sets of inputs, you need to wait until you finish computing `C`, and then you will write out `C` (into `stdout`), and delete (free or reuse) `A`, `B`, and `C`. Thus, it is essential that the task scheduler knows when to read in the next batch of inputs, depending on when the computation of `C` finishes.

To make your life easier, you can feel free to use additional memory space during the computing process, but your program should make sure to finish computing the previous `C` before reading the next set of `A` and `B`.

The file `task3.c` shows some example code. You may need to merge it with the code of previous tasks. In this task, you should read from the input file `./data/task3.txt`.

### Notes

* You can use `make gen_data` to prepare the data.
* You can use `make task3` to build the program.
* You can use `time ./task3` to run your program and get the time statistics.
* You can use `make task3_grade` to grade your task3 correctness and performance.

### Grading

You need to describe your design of Task 3 in detail in your report.

The test setting is similar to Task 2.

In Task 3, when you try to read next matrix multiplication task, you need to make sure finish computing the last result, and free or reuse the memory space of at least `A`, `B` before continue with the next iteration.  Otherwise, you might use too much memory, causing your program to crash.  In this case, you will receive no points. 

A little bit different from Task 2, here, parallel speedup is defined the time used (**without** I/O time) of baseline divided by the time used (**without** I/O time) of your multi-thread optimized implementation. Since in this task, you can hardly use parallelism to save the (especially input) I/O time. 

1. If your result is not correct, you will get 0 points;
2. If your result is correct, you will get `(min(P, 2.6) -1) * 12.5` pts, where P is the speedup (assuming `P>1`).  
3. In other word, if your parallel speedup is over 2.6x, you can get the full 20 pts.  For all `P<1`, you get 0 points. 

## Task4: Grabbing the Input Matrix from the Web

This task allows you to practice your shell scripting and `Makefile` skills. You need to write a Make target, `multiply`, that does the following:

* Download a `tar.gz` file. The URL for the download is in a file `data_source.txt` in the directory. Note that the file contains many other descriptions, but you will need to extract the URL only for the downloader (we recommend curl or wget).
* untar the file (look at the documentation on how to do it.)
* Iterate through all the files and find only the lines starting with `DATA_A_(line_number):` or `DATA_B_(line_number):`, followed by a sequence of integers, separated by either space or comma (this is one line of the input matrix). You can safely discard all other lines.
* get the matrix size `m, k, n` from `DATA_A` and `DATA_B`.
* Change each line into the format that your Task 3 worker from the previous task can read as input. You can just set the `task_id` to `1`. Note that you should NOT change your task3 workers, but you only massage your data into the format (e.g. you should remove the line header of `DATA_A_*`:, and make the separators consistent).
* Then you should export the data files as input to your task. You should  copy your generated input to `data/task3.txt`, to make sure your Task 3 code can read it without any error. 
* Finally, you run your multiplication code from Task 3, and produce the multiplication result file `task4_output.txt`.

The TAs should finish all the above tasks by running a single `make multiply` command.

When you finish your `make multiply`, you can use `make task4_grade` to check the correctness of your result.

You can either use a combination of grep, sed, awk, or you can use python to produce the input file.


### Grading

You need to describe which commands or script functions you used in your report.

We will change content and the URL in `data_source.txt` when testing your program, but the content has the same format.

If `make multiply` is successful and the result is correct, you can get 20 pts. 


## Grading Summary

Grading: Code correctness and performance 20*4 pts; your written report worth the remaining 20 pts.

## Submission

You should use `git.tsinghua.edu.cn` to manage your project, you may refer to the discussion session or lab1 documentation  to find some useful information. If you don't have `git.tsinghua.edu.cn` permissions, you can also use services like GitHub or Gitee, make sure the repo is `private`, and contact the TA **before** the deadline.

You need to create a **private** repository, and add your teamate, TA `fxy23` to the `Members` (more powerful role than `Reporter`) of your repo. 

You need to keep your final submission code and your report in your **default branch** of your repo, and then submit `student1_id`, `student2_id` and the **repo url address** on `https://learn.tsinghua.edu.cn`.
