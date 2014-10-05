#ifndef GPIO_H
#define GPIO_H

int gpio_export(unsigned int gpio_number);
int gpio_unexport(unsigned int gpio_number);
int gpio_isexported(unsigned int gpio_number, int* exported);
int gpio_open(unsigned int gpio_number, int* fd);
int gpio_close(int fd);
int gpio_write(int fd, int value);
int gpio_direction(unsigned int gpio_number, int dir);

#endif