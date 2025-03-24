
#include <stdio.h>

int	main(void)
{
	int x = 5;
	int y = 8;

	x ^= y;
	y ^= x;
	x ^= y;
	printf("%d\n%d\n", x, y);
	return (1);
}