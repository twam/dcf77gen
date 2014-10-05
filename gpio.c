#include "gpio.h"

#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>


#define GPIO_MAX_LENGTH 3

int gpio_export(unsigned int gpio_number)
{
  char buf[GPIO_MAX_LENGTH+1];
  snprintf(buf, GPIO_MAX_LENGTH+1, "%u", gpio_number);

  int fd = open("/sys/class/gpio/export", O_WRONLY);
  if (fd == -1)
    return -1;

  if (write(fd, buf, strlen(buf)) == -1)
    return -1;

  close(fd);

  return 0;
}

int gpio_unexport(unsigned int gpio_number)
{
  char buf[GPIO_MAX_LENGTH+1];
  snprintf(buf, GPIO_MAX_LENGTH+1, "%u", gpio_number);

  int fd = open("/sys/class/gpio/unexport", O_WRONLY);
  if (fd == -1)
    return -1;

  if (write(fd, buf, strlen(buf)) == -1)
    return -1;

  close(fd);

  return 0;
}

int gpio_isexported(unsigned int gpio_number, int* exported)
{
  char buf[GPIO_MAX_LENGTH+21];

  snprintf(buf, GPIO_MAX_LENGTH+21, "/sys/class/gpio/gpio%u", gpio_number);

  struct stat st;
  int retval = stat(buf, &st);
  if (retval == -1) {
    return -1;
  }

  if (S_ISDIR(st.st_mode)) {
    *exported = 1;
  } else {
    *exported = 0;
  }

  return 0;
}

int gpio_open(unsigned int gpio_number, int* fd)
{
  char buf[GPIO_MAX_LENGTH+27];

  snprintf(buf, GPIO_MAX_LENGTH+27, "/sys/class/gpio/gpio%u/value", gpio_number);
  *fd = open(buf, O_WRONLY);

  if (*fd == -1) {
    return -1;
  }

  return 0;
}

int gpio_close(int fd)
{
  close(fd);

  return 0;
}

int gpio_write(int fd, int value)
{
  if (write(fd, value == 0 ? "0" : "1", 1) != 1) {
    return -1;
  }

  return 0;
}

int gpio_direction(unsigned int gpio_number, int dir)
{
  char buf[GPIO_MAX_LENGTH+30];
  snprintf(buf, GPIO_MAX_LENGTH+30, "/sys/class/gpio/gpio%u/direction", gpio_number);
  int fd = open(buf, O_WRONLY);

  if (fd == -1) {
    return -1;
  }

  if (write(fd, dir == 0 ? "in" : "out", dir == 0 ? 2 : 3) != 1) {
    return -1;
  }

  close(fd);

  return 0;
}