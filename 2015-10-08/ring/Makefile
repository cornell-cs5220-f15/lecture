CC=mpicc
CFLAGS=-std=gnu99 -Wall

.PHONY: clean

ring.x: ring.c
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f ping.x ping-*.o*

