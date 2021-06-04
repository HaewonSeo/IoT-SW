#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <asm/uaccess.h>

#define DRIVER_AUTHOR	"KHU"
#define DRIVER_DESC		"Led Driver"
#define DEVICE_NAME		"led_dd"
#define MAJOR_NUMBER	222

#define GPIO_BASE		0x3F200000
#define GPIO_SIZE		0xC0

#define INPUT			0
#define OUTPUT			1
#define LOW				0
#define HIGH			1

void __iomem *gpioAddr;
volatile unsigned int gpioToGPFSEL[] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
};
volatile unsigned int gpioToShift[] = {
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
};
volatile unsigned int gpioToGPSET = 0x1C;
volatile unsigned int gpioToGPCLR = 0x28;

const int Led[16] = {
	4, 17, 18, 27, 22, 23, 24, 25,
	6, 12, 13, 16, 19, 20, 26, 21
};

static volatile void initGpioAddr(void) {
	if(gpioAddr == NULL) {
		gpioAddr = ioremap(GPIO_BASE, GPIO_SIZE);
	}
}

static void pinMode(int pin, int mode) {
	volatile unsigned int *gpio = (volatile unsigned int *)gpioAddr;

	unsigned int fsel = gpioToGPFSEL[pin]/sizeof(unsigned int);
	unsigned int shift = gpioToShift[pin];

	if(mode) {
		gpio[fsel] |= (1 << shift);
	}
	else {
		gpio[fsel] |= (0 << shift);
	}
}

static void digitalWrite(int pin, int value) {
	volatile unsigned int *gpio = (volatile unsigned int *)gpioAddr;

	unsigned int set = gpioToGPSET/sizeof(unsigned int);
	unsigned int clr = gpioToGPCLR/sizeof(unsigned int);

	if(value) {
		gpio[set] = (1 << pin);
	}
	else {
		gpio[clr] = (1 << pin);
	}
}

static int led_open(struct inode *inode, struct file *filp) {
	int i;

	initGpioAddr();

	for(i = 0; i < 16; i++) {
		pinMode(Led[i], OUTPUT);
		digitalWrite(Led[i], LOW);
	}

	printk("[led_dd] led_open\n");

	return 0;
}

static int led_release(struct inode *inode, struct file *filp) {
	iounmap(gpioAddr);

	printk("[led_dd] led_realse\n");

	return 0;
}

static int led_write(struct file *filp, const char *buf,
					 size_t len, loff_t *f_pos) {
	int i;
	char state;

	for(i = 0; i < len; i++) {
		copy_from_user(&state, &buf[i], 1);

		if(state == '0') {
			digitalWrite(Led[3-i], LOW);
		}
		else {
			digitalWrite(Led[3-i], HIGH);
		}
	}

	printk("[led_dd] led_write\n");

	return len;
}

static struct file_operations fops = {
	.owner		= THIS_MODULE,
	.open		= led_open,
	.release	= led_release,
	.write		= led_write,
};

static int led_init(void) {
	printk("[led_dd] led_init()\n");
	register_chrdev(MAJOR_NUMBER, DEVICE_NAME, &fops);

	return 0;
}

static void led_exit(void) {
	printk("[led_dd] led_exit()\n");
	unregister_chrdev(MAJOR_NUMBER, DEVICE_NAME);
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("Dual BSD/GPL");
