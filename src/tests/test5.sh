#!/bin/sh

time nice -n 30 ./cpu_bound 10 &
time nice -n 30 ./IO_bound IO_bound.in &