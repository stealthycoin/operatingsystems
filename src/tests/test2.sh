#!/bin/sh

time nice -n 80 ./cpu_heavy 10 &
time nice -n 30 ./cpu_heavy 10 &
