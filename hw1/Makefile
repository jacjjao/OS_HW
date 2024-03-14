CC = gcc

all: FileCopy TimeSHM TimePipe

.PHONY: FileCopy
FileCopy: file_copy.c 
	$(CC) file_copy.c -o FileCopy

.PHONY: TimeSHM
TimeSHM: time_shm.c 
	$(CC) time_shm.c -o time_shm

.PHONY: TimePipe
TimePipe: time_pipe.c
	$(CC) time_pipe.c -o time_pipe