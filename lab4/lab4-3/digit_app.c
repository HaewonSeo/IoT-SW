#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <linux/kdev_t.h>

#define _LED_PATH_ "/dev/led_dd"

int main(int argc, char **argv) {
	int fd = 0;

	if(argc != 2) {
		printf("Usage: %s [LED binary]\n", argv[0]);
		exit(1);
	}

	if( (fd = open(_LED_PATH_, O_RDWR | O_NONBLOCK) ) < 0 ) {
		perror("open()");
		exit(1);
	}

	write(fd, argv[1], strlen(argv[1]));

	close(fd);

	return 0;
}
