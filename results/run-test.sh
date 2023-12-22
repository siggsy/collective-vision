#!/bin/bash

prog="./bin/program"
out="./obj/sim-test.txt"

if [ `basename \`pwd\`` = "results" ]; then
	prog=".$prog"
	out=".$out"
fi

$prog \
	-n "1000" \
	-b "(50, 0, 0.5, 0.5, 1)" \
	-b "(1 , 1, 1, 4, 10)" \
	-c "0(0: 0.5, 0.08, 0.1, 0.08, 0.95)" \
	-c "1(1: 0, 0, 0, 0, 1)" \
	-c "0(1: 0.5, -0.1, 0.1, -0.1, 0.95)" \
	-c "1(0: -0.8, 0, -0.6, 0, 0.98)" \
	-o "$out"
