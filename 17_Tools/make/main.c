#include <stdio.h>

#include <math.h>

extern void test1(void);
extern void test2(void);

int main(void) {
	test1();
	test2();
	printf("%f", sqrt(3.43));

	return 0;
}

