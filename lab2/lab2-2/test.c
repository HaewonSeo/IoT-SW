#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#define GPIO_BASE	0x3F200000
#define GPFSEL1		0x04
#define GPSET0		0x1C
#define GPCLR0		0x28

int main()
{
	int fd = open("/dev/mem", O_RDWR|O_SYNC);
	
	// Error Handling
	if (fd < 0)
	{
		printf("Can't open /dev/mem \n");
		exit(1);
	}

	// Map pages of memory
	char *gpio_memory_map = (char*)mmap(0, 4096, PROT_READ|PROT_WRITE,
											MAP_SHARED, fd, GPIO_BASE);
	// Error Handling
	if (gpio_memory_map == MAP_FAILED)
	{
		printf("Error : mmap \n");
		exit(-1);
	}

	// GPIO18

	//volatile unsigned int *gpio = (volatile unsigned int*)gpio_memory_map;
	//gpio[GPFSEL1/4] = (1<<24);	

	volatile char *gpio = (volatile char *)gpio_memory_map;
	
	int i;

	for (i = 0; i < 16; i++)
		printf("gpio[%d](%#x) = %#0x\n", i, &gpio[i], gpio[i]);

	/*
	for (i = 0; i < 5; i++)
	{
		gpio[GPCLR0 / 4] = (1 << 18);
		sleep(1);

		gpio[GPSET0 / 4] = (1 << 18);
		sleep(1);
	}
	*/
	// Unmap pages of memory
	munmap(gpio_memory_map, 4096);

	return 0;
}

