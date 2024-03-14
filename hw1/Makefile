CC = gcc

all: FileCopy Time

.PHONY: FileCopy
FileCopy: filecpy.c filecpy_main.c 
	$(CC) filecpy.c filecpy_main.c -o FileCopy

.PHONY: Time
Time: time_shm.c 
	$(CC) time_shm.c -o time_shm