
mytest: main.o test1.o test2.o
	gcc -o mytest main.o test1.o test2.o -lm

main.o: main.c
	gcc -c main.c

test1.o: test1.c
	gcc -c test1.c

test2.o: test2.c mytest.h
	gcc -c test2.c

clean:
	rm -f *.o mytest mytes.db

testprogram:
	gcc -o mytest main.c test1.c test2.c -lm

debugprogram:
	gcc -DDEBUG -ggdb3 -o mytest.db main.c test1.c test2.c -lm
