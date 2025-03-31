
#include <stdio.h>

int	main(void)
{
	char *parent = (char *)malloc(10);
	int pid = fork();
	if (pid == 0)
	{
		char *child = (char *)malloc(10);
	}
	return (1);
}