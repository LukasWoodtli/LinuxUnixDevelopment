
main: testlist dynamisch

dynamisch: dynamisch.c libmyequal.so
	gcc -o dynamisch dynamisch.c

testlist: testlist.c libmyequal.so
	gcc -L $(abspath $(dir libmyequal.so)) -o testlist testlist.c -lmyequal -isystem . 

libmyequal.so: myequal.o
	gcc -ggdb3 -shared -o libmyequal.so myequal.o -lc

myequal.o:
	gcc -fPIC -Wall -g -c myequal.c

run-dynamisch: dynamisch
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(abspath $(dir libmyequal.so)) ./dynamisch

test: testlist
	LD_LIBRARY_PATH=$$LD_LIBRARY_PATH:$(abspath $(dir libmyequal.so)) ./testlist

clean:
	rm -rf *.o *.so testlist dynamisch
