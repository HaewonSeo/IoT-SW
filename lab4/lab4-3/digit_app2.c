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

	if(argc != 3) {
		printf("Usage: %s [LED color] [LED binary]\n", argv[0]);
		exit(1);
	}

	if( (fd = open(_LED_PATH_, O_RDWR | O_NONBLOCK) ) < 0 ) {
		perror("open()");
		exit(1);
	}

	// set led color (0:RED, 1:GREEN)
	ioctl(fd, 1, atol(argv[1]));

	write(fd, argv[2], strlen(argv[2]));

	close(fd);

	return 0;
}
