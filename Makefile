CC = gcc
DAY = 1

part1: day$(DAY).c
	$(CC) day$(DAY).c -o day$(DAY).o -lm

part2: day$(DAY)_2.c
	$(CC) day$(DAY)_2.c -o day$(DAY)_2.o -lm

run-part1:
	./day$(DAY).o input/day$(DAY).txt

run-part2:
	./day$(DAY)_2.o input/day$(DAY).txt

clean:
	rm -f *.o
