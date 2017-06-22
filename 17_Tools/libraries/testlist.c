#include <stdio.h>
#include <myequal.h>

int main(void) {
	MYBOOL ret;
	ret = my_equal(5, 5);
	if (ret == MYTRUE)
		printf("Beide Werte sind gleich\n");
	else
		printf("Die Werte sind nich gleich\n");
	
	return 0;
}
