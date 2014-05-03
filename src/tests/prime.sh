#!/bin/sh

time nice -n 80 ./primes &
time nice -n 30 ./primes &
