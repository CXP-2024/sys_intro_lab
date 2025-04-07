.PHONY: all clean multiply task1_queue_check task1_grade task2_grade task3_grade task4_grade

SOURCES := $(wildcard src/*.c)
INCLUDES := $(wildcard src/*.h)

all: task1 task2 task3

TASK1_SOURCES = src/task1.c src/queue.c
TASK2_SOURCES = src/task2.c src/queue.c src/mm.c
TASK3_SOURCES = src/task3.c src/queue.c src/mm.c

CC_OPTIONS = -O0 -Wall -pthread

task1: $(SOURCES) $(INCLUDES)
	gcc $(TASK1_SOURCES) -o $@ $(CC_OPTIONS)

# change this to test different cases, the provided implementation is sequential (first producer, then consumers), therefore the queue length should be 100, or other numbers accoring to gen_data.
CFLAGS=-DMAX_QUEUE_LEN=100
task2: $(SOURCES) $(INCLUDES)
	gcc $(CFLAGS) $(TASK2_SOURCES) -o $@  $(CC_OPTIONS)

task3: $(SOURCES) $(INCLUDES)
	gcc $(TASK3_SOURCES) -o $@  $(CC_OPTIONS)


baseline: $(SOURCES) $(INCLUDES)
	gcc src/baseline.c src/mm.c -o $@ $(CC_OPTIONS)

gen_data: data/gen.py
	python3.7 data/gen.py

# Task 1 grading
task1_queue_check: $(SOURCES) $(INCLUDES)
	gcc src/test_queue.c src/queue.c -o $@ $(CC_OPTIONS)
	./task1_queue_check
	rm task1_queue_check

# you can change the parameters to test different cases, in final grading, we will change the parameters.
TASK1_GRADING_OPTIONS =  -DGRADING -DTASK1_TASKS_NUM=2048 -DMAX_QUEUE_LEN=2

task1_grade_bin: $(SOURCES) $(INCLUDES)
	gcc $(TASK1_SOURCES) -o $@ $(CC_OPTIONS) $(TASK1_GRADING_OPTIONS)

task1_grade: task1_grade_bin test/task1.py
	./task1_grade_bin > task1_grade_output.txt
	python3 test/task1.py
	rm task1_grade_bin task1_grade_output.txt

# Task 2 grading
grade_checker: test/checker.cpp
	g++ -o $@ $<

TASK2_GRADING_OPTIONS =  -DGRADING  -DMAX_QUEUE_LEN=2

data/task2.txt: data/gen.py
	python3.7 $<

task2_grade_bin: $(SOURCES) $(INCLUDES)
	gcc $(TASK2_SOURCES) -o $@ $(CC_OPTIONS) $(TASK2_GRADING_OPTIONS)

task2_grade: task2_grade_bin test/task2.py baseline data/task2.txt grade_checker
	python3.7 test/task2.py 
	rm task2_grade_bin task2_grade_output.txt task2_grade_time.txt task2_grade_baseline_time.txt task2_grade_answer.txt grade_checker

# Task 3 grading
TASK3_GRADING_OPTIONS =  -DGRADING  -DMAX_QUEUE_LEN=2

data/task3.txt: data/gen.py
	python3.7 data/gen.py

task3_grade_bin: $(SOURCES) $(INCLUDES)
	gcc $(TASK3_SOURCES) -o $@ $(CC_OPTIONS) $(TASK3_GRADING_OPTIONS)

task3_grade: task3_grade_bin test/task3.py baseline data/task3.txt grade_checker
	python3.7 test/task3.py 
	rm task3_grade_bin task3_grade_output.txt task3_grade_time.txt task3_grade_baseline_time.txt task3_grade_answer.txt grade_checker

# Task4, leave for you to implement.
# multiply: 
#

task4_grade: data_source.txt
	make multiply
	python3.7 test/task4.py

clean:
	rm data/task2.txt data/task3.txt \
	task1 task2 task3 baseline \
	task1_grade_* task2_grade_* task3_grade_* grade_checker \
	sysintro_task4.tar.gz