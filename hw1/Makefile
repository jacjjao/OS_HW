CC = gcc

all: FileCopy Time

.PHONY: FileCopy
FileCopy: filecpy.c filecpy_main.c 
	$(CC) filecpy.c filecpy_main.c -o FileCopy

.PHONY: Time
Time: time.c 
	$(CC) time.c -o time