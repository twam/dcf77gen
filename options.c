#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include "options.h"

int interval_ms = 100;
int gpio = -1;

static void usage(FILE* fp, int argc, char** argv)
{
  fprintf(fp,
    "Usage: %s [options]\n\n"
    "Options:\n"
    "-h | --help          Print this message\n"
    "-i | --interval      Interval in ms\n"
    "-g | --gpio          GPIO to use\n"
//    "-v | --version       Print version\n"
    "",
    argv[0]);
  }

static const char short_options [] = "hvi:g:";

static const struct option long_options [] = {
  { "help",       no_argument,          NULL,             'h'},
  { "interval",   required_argument,    NULL,             'i'},
  { "gpio",       required_argument,    NULL,             'g'},
//  { "version",  no_argument,    NULL,   'v' },
  { 0, 0, 0, 0 }
};

void parse_options(int argc, char** argv)
{
  for (;;) {
    int index, c = 0;

    c = getopt_long(argc, argv, short_options, long_options, &index);

    if (-1 == c)
      break;

    switch (c) {
      case 'h':
        // print help
        usage(stdout, argc, argv);
        exit(EXIT_SUCCESS);

      case 'i':
        interval_ms = atoi(optarg);
        break;

      case 'g':
        gpio = atoi(optarg);
        break;

      default:
        usage(stderr, argc, argv);
        exit(EXIT_FAILURE);
    }
  }

  // check for needed parameters
  if (gpio == -1) {
    fprintf(stderr, "GPIO not set.\n");
    usage(stderr, argc, argv);
    exit(EXIT_FAILURE);
  }

}