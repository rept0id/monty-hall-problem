build:
	gcc -O3 -fopenmp ./main.c -o main

run:
	./main

time:
	time ./main
