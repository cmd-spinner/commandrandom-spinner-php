#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h> 
#include <fcntl.h>

int main(int argc, char** argv)
{
	const char msg[] = "Hello World!";
	write(1, msg, sizeof(msg) - 1);
	const char err[] = "error";
	write(2, err, sizeof(err) - 1);
	char* buf = (char*)malloc(10);

	int fd = open("aaaaa", 1);
	FILE* fp = fopen("dummy", "w");
	write(fd, err, sizeof(err) - 1);
	fwrite(err, sizeof(char), sizeof(err) - 1, fp);
	printf("aaaaaa\n");
	return 0;
}
