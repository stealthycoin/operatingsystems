#!/bin/sh

time nice -n 80 ./cpu_bound &
time nice -n 80 ./cpu_bound &
time nice -n 80 ./cpu_bound &
time nice -n -19 ./cpu_bound &
