basic: basic.o life_common.o crc32.o
	$(CC) -std=c99 -o basic basic.o life_common.o crc32.o

.PHONY: glider
glider: basic
	./basic -v -n 10 -g 15 -f glider.txt

basic.o: basic.c life_common.h
life_common.o: life_common.c life_common.h

%.o: %.c
	$(CC) -std=c99 -c $<

clean:
	rm -f basic *.o
