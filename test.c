
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int	main(int argc, char **argv, char **envp)
{
	char	*parent;
	int		pid;
	char	*child;
	int		fd;

	fd = open("test.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	parent = (char *)malloc(sizeof(char) * 10);
	pid = fork();
	if (pid == 0)
	{
		close(fd);
		child = (char *)malloc(sizeof(char) * 10);
		if (execve("/bin/echosdafasf", (char *[]){"/bin/echo",
				"aaaaaaa\naaaaaaaa\naaaaaaaa\n", NULL}, envp) == -1)
		{
			fprintf(stderr, "execve failed: %s\n", strerror(errno));
			fprintf(stderr, "execve failed: %d\n", errno);
			// free(child);
			// free(parent)
			exit(127);
		}
	}
	close(fd);
	free(parent);
	return (0);
}
