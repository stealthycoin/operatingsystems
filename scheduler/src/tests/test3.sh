#!/bin/sh

time nice -n 80 ./cpu_bound &
time nice -n 30 ./cpu_bound &
time nice -n 5 ./cpu_bound &
