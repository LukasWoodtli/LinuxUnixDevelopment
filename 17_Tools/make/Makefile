
mytest: main.o test1.o test2.o
	gcc -o $@ main.o test1.o test2.o -lm

.c.o:
	gcc -c $<

test: mytest
	./mytest

clean:
	rm -f *.o mytest

