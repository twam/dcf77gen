#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include "options.h"
#include "version.h"

int interval_ms = 100;
int gpio = -1;
int offset_ms = 0;
int noise = 0;

static void usage(FILE* fp, int argc, char** argv)
{
  fprintf(fp,
    "Usage: %s [options]\n\n"
    "Options:\n"
    "-h | --help          Print this message\n"
    "-i | --interval      Interval in ms\n"
    "-g | --gpio          GPIO to use\n"
    "-o | --offset        Offset in ms\n"
    "-v | --version       Print version\n"
    "-n | --noise         Add noise in percent\n"
    "",
    argv[0]);
  }

static const char short_options [] = "hvi:g:o:vn:";

static const struct option long_options [] = {
  { "help",     no_argument,       NULL, 'h'},
  { "interval", required_argument, NULL, 'i'},
  { "gpio",     required_argument, NULL, 'g'},
  { "offset",   required_argument, NULL, 'o'},
  { "version",  no_argument,       NULL, 'v' },
  { "noise",    required_argument, NULL, 'n'},
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

      case 'o':
        offset_ms = atoi(optarg);
        break;

      case 'n':
        noise = atoi(optarg);
        if (noise < 0) {
          fprintf(stderr, "Noise has to be >= 0.\n");
          exit(EXIT_FAILURE);
        } else if (noise > 100) {
          fprintf(stderr, "Noise has to be <= 100.\n");
          exit(EXIT_FAILURE);
        }
        break;

      case 'v':
        printf("Version: " GIT_VERSION "\n");
        printf("Compiled on: " __DATE__ " " __TIME__ "\n");
        exit(EXIT_SUCCESS);
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