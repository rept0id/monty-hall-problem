build:
	gcc -O3 -fopenmp util/*.c test/*.c controller/*.c ./main.c -o monty-hall-problem

run:
	./monty-hall-problem

time:
	time ./monty-hall-problem
