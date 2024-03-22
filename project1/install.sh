#!/bin/bash

make
insmod jiffies.ko
insmod seconds.ko
insmod list_tasks.ko
insmod list_tasks_dfs.ko
