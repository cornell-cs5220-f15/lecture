# make targets:
#   make all: build basic with default CC
#   make submit: run the timer on the compute nodes
#   make run: run the timer locally (good for a laptop)
#   make glider: demo run (print every generation, small board)
#   make with-icc: build basic with Intel compiler
#   make with-gcc: build with GCC compiler
#   make with-gcc-5: build with GCC compiler (Homebrew gcc-5)
#   make clean: clean up binaries

# Dummy targets

.PHONY: all submit run glider with-icc with-gcc with-gcc5

all: basic

submit: basic
	qsub life.pbs

run: basic
	./basic -n 100 -g 1000 -f glider.txt
	./basic -n 4000 -g 10 -f glider.txt

glider: basic
	./basic -v -n 10 -g 15 -f glider.txt

with-icc:
	make CC=icc CFLAGS="-DWITH_TIMING -openmp"

with-gcc:
	make CC=gcc CFLAGS="-DWITH_TIMING -fopenmp"

with-gcc5:
	make CC=gcc-5 CFLAGS="-DWITH_TIMING -fopenmp"

clean:
	rm -f basic *.o

# Build rules

basic: basic.o life_common.o crc32.o
	$(CC) -std=c99 $(CFLAGS) -o basic basic.o life_common.o crc32.o

basic.o: basic.c life_common.h
life_common.o: life_common.c life_common.h

%.o: %.c
	$(CC) -std=c99 $(CFLAGS) -c $<

