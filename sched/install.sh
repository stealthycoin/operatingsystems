#!/bin/sh

cp schedproc.h schedule.c /usr/src/servers/sched/
cp config.h /usr/include/minix/
cd /usr/src/tools/ && make servers
