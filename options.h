#ifndef OPTIONS_H
#define OPTIONS_H

void parse_options(int argc, char** argv);

extern int interval_ms;
extern int gpio;
extern int offset_ms;
extern int noise;

#endif