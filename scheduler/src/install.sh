#!/bin/sh

cp ./include/minix/config.h /usr/src/include/minix/config.h
cp ./servers/pm/schedule.c /usr/src/servers/pm/schedule.c
cp ./servers/pm/utility.c /usr/src/servers/pm/utility.c
cp ./servers/sched/sched.h /usr/src/servers/sched/sched.h
cp ./servers/sched/schedproc.h /usr/src/servers/sched/schedproc.h
cp ./servers/sched/schedule.c /usr/src/servers/sched/schedule.c

cd /usr/src/tools && make clean install
