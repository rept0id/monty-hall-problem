build:
	gcc -O3 -fopenmp lib/ratatoi/ratatoi.c util/*.c controller/*.c view/*.c test/*.c ./main.c -o monty-hall-problem

run:
	./monty-hall-problem

time:
	time ./monty-hall-problem
