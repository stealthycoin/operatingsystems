#!/bin/sh

time nice -n 80 ./cpu_heavy 10 &
time nice -n -19 ./cpu_heavy 10 &
