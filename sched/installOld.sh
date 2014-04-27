#!/bin/sh

cp /usr/src/servers/sched/OLDschedproc.h /usr/src/servers/sched/schedproc.h
cp /usr/src/servers/sched/OLDschedule.c /usr/src/servers/sched/schedule.c
cp /usr/src/include/minix/OLDconfig.h /usr/src/include/minix/config.h
cd /usr/src/tools/ && make install
