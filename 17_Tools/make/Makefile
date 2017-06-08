
mytest: main.o test1.o test2.o
	gcc -o $@ main.o test1.o test2.o -lm

main.o: main.c
	gcc -c $*.c

test1.o: test1.c
	gcc -c $*.c

test2.o: test2.c mytest.h
	gcc -c $*.c

clean:
	rm -f *.o mytest
