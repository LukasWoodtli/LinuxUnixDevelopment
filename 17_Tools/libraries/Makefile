
testlist: testlist.c libmyequal.a
	gcc -o testlist testlist.c -lmyequal -isystem . -L .



libmyequal.a: myequal.o
	ar crs libmyequal.a myequal.o
	ranlib libmyequal.a

myequal.o:
	gcc -c myequal.c

test: testlist
	./testlist

clean:
	rm -rf *.o *.a testlist