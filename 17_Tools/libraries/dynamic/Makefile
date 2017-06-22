
testlist: testlist.c libmyequal.so
	gcc -L $(abspath $(dir libmyequal.so)) -o testlist testlist.c -lmyequal -isystem . 

libmyequal.so: myequal.o
	gcc -ggdb3 -shared -o libmyequal.so myequal.o -lc

myequal.o:
	gcc -fPIC -Wall -g -c myequal.c


test: testlist
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(abspath $(dir libmyequal.so)) ./testlist
clean:
	rm -rf *.o *.so testlist