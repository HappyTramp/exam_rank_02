#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int get_next_line(int fd, char **line);

int main(int argc, char **argv)
{
	int fd;
	char *line;

	fd = open(argv[1], O_RDONLY);
	for (int i = 0; get_next_line(fd, &line) == 1; i++)
	{
		printf("%d [%s]\n", i, line);
		free(line);
	}
	printf("$ [%s]\n", line);
	free(line);

	return 0;
}
