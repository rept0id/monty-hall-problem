build:
	gcc -O3 -fopenmp controller/*.c ./main.c -o monty-hall-problem

run:
	./monty-hall-problem

time:
	time ./monty-hall-problem
