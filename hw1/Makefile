CC = gcc

all: FileCopy TimeSHM TimePipe Collatz

.PHONY: FileCopy
FileCopy: file_copy.c 
	$(CC) file_copy.c -o FileCopy

.PHONY: TimeSHM
TimeSHM: time_shm.c 
	$(CC) time_shm.c -o TimeSHM

.PHONY: TimePipe
TimePipe: time_pipe.c
	$(CC) time_pipe.c -o TimePipe

.PHONY: Collatz
Collatz: collatz.c
	$(CC) collatz.c -o Collatz