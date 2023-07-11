#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

union map
{
	unsigned char map4[4];
	unsigned int ull4;	
};

int main(int argc, char **argv)
{
	union map m4;
	int fd = 0;
	unsigned int x = 0;
	unsigned int m = 0;
	unsigned int h = 0;

	fd = open("/dev/urandom",O_RDONLY);

	read(fd,m4.map4,4);

	close(fd);

	printf("ull4 is %u\n", m4.ull4);

	h = (m4.ull4 / 2);

	for (x = 1; x <h; x++)
	{
		if ((m4.ull4 % x) == 0)
		{
			printf("%u / %u\n",m4.ull4,x);
			m = x;
		}
	}

	printf("Largest was: %u\n",m);

	return 0;
}
