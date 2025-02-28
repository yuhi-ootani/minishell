#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int	main(void)
{
	pid_t	pid;
	int		status;

	for (int i = 0; i < 3; i++)
	{
		pid = fork();
		if (pid == 0)
		{
			printf("Child %d (PID: %d) started\n", i, getpid());
			sleep(20 - i); // Different exit times
			printf("Child %d (PID: %d) exiting\n", i, getpid());
			exit(0);
		}
	}
	// Parent waits for all children
	while (wait(&status) > 0)
		printf("A child has exited\n");
	printf("All children have finished.\n");
	return (0);
}
