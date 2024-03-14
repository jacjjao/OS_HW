CC = gcc

all: FileCopy TimeSHM TimePipe

.PHONY: FileCopy
FileCopy: filecpy.c filecpy_main.c 
	$(CC) filecpy.c filecpy_main.c -o FileCopy -fsanitize=address

.PHONY: TimeSHM
TimeSHM: time_shm.c 
	$(CC) time_shm.c -o time_shm

.PHONY: TimePipe
TimePipe: time_pipe.c
	$(CC) time_pipe.c -o time_pipe