#include <sys/timerfd.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <sys/epoll.h>
#include "options.h"
#include "gpio.h"
#include "dcf77.h"

#define handle_error(msg) do { perror(msg); exit(EXIT_FAILURE); } while (0)

int main(int argc, char **argv) {
  parse_options(argc, argv);

  struct itimerspec new_value;
  int timer_fd;

  int gpio_exported;
  gpio_isexported(gpio, &gpio_exported);

  if (gpio_exported == 0) {
    gpio_export(gpio);
  }

  int gpio_fd;
  gpio_open(gpio, &gpio_fd);
  gpio_direction(gpio, 1);

  new_value.it_value.tv_sec = interval_ms/1000;
  new_value.it_value.tv_nsec = (interval_ms%1000)*1E6;
  new_value.it_interval.tv_sec = interval_ms/1000;
  new_value.it_interval.tv_nsec = (interval_ms%1000)*1E6;

  timer_fd = timerfd_create(CLOCK_MONOTONIC, 0);
  if (timer_fd == -1)
    handle_error("timerfd_create");

  if (timerfd_settime(timer_fd, 0, &new_value, NULL) == -1)
    handle_error("timerfd_settime");

  struct epoll_event event;
  int efd = epoll_create(2);
  if (efd == -1) {
    handle_error("epoll_create");
  }

  event.data.fd = timer_fd;
  event.events = EPOLLIN;
  epoll_ctl(efd, EPOLL_CTL_ADD, timer_fd, &event);

  while (1) {
      struct epoll_event revent;
      epoll_wait(efd, &revent, 1, -1);
      /* revent can now be used */

    struct timespec now;
    if (clock_gettime(CLOCK_REALTIME, &now) == -1) {
      handle_error("clock_gettime");
    }


    now.tv_nsec += (offset_ms % 1000l) * 1000000l;
    now.tv_sec += offset_ms / 1000l;

    if (now.tv_nsec / 1000000000l != 0) {
      now.tv_sec -= now.tv_nsec / 1000000000l;
      now.tv_nsec %= 1000000000l;
    }

    if (now.tv_nsec < 0) {
      now.tv_sec -= 1;
      now.tv_nsec += 1000000000l;
    }


    uint64_t exp;
    if (read(timer_fd, &exp, sizeof(uint64_t)) == -1) {
      handle_error("read");
    }

    int msec = now.tv_nsec/1E6;

    int length = dcf77_encode(&now.tv_sec);
    int on = 0;

    if (msec/100 == 0) {
      // first 100 ms
      if (length != 2) {
        on = 1;
      }
    } else if (msec/100 == 1) {
      // second 100 ms
      if (length == 1) {
        on = 1;
      }
    }

    gpio_write(gpio_fd, on);

  }

  close(timer_fd);
  close(efd);
  gpio_close(gpio_fd);

  if (gpio_exported == 0) {
    gpio_unexport(gpio);
  }

	return EXIT_SUCCESS;
}