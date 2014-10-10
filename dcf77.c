#include "dcf77.h"

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

static uint8_t timecode[60];
static time_t timecode_time;
static struct tm timecode_tm;
static int first = 1;

int dcf77_filltimecode(const time_t* time) {
  timecode_time = *time;
  localtime_r(&timecode_time, &timecode_tm);

  memset(timecode, 0, sizeof(timecode));

  // start of time
  timecode[20] = 1;

  // minute
  uint8_t minute_ones = timecode_tm.tm_min % 10;
  uint8_t minute_tens = timecode_tm.tm_min / 10;
  timecode[21] = !!(minute_ones & 0x01);
  timecode[22] = !!(minute_ones & 0x02);
  timecode[23] = !!(minute_ones & 0x04);
  timecode[24] = !!(minute_ones & 0x08);
  timecode[25] = !!(minute_tens & 0x01);
  timecode[26] = !!(minute_tens & 0x02);
  timecode[27] = !!(minute_tens & 0x04);

  // parity minute
  timecode[28] = (timecode[21] + timecode[22] + timecode[23] + timecode[24] + timecode[25] + timecode[26] + timecode[27]) % 2;

  // hour
  uint8_t hour_ones = timecode_tm.tm_hour % 10;
  uint8_t hour_tens = timecode_tm.tm_hour / 10;
  timecode[29] = !!(hour_ones & 0x01);
  timecode[30] = !!(hour_ones & 0x02);
  timecode[31] = !!(hour_ones & 0x04);
  timecode[32] = !!(hour_ones & 0x08);
  timecode[33] = !!(hour_tens & 0x01);
  timecode[34] = !!(hour_tens & 0x02);

  // parity hour
  timecode[35] = (timecode[29] + timecode[30] + timecode[31] + timecode[32] + timecode[33] + timecode[34]) % 2;

  // day
  uint8_t day_ones = timecode_tm.tm_mday % 10;
  uint8_t day_tens = timecode_tm.tm_mday / 10;
  timecode[36] = !!(day_ones & 0x01);
  timecode[37] = !!(day_ones & 0x02);
  timecode[38] = !!(day_ones & 0x04);
  timecode[39] = !!(day_ones & 0x08);
  timecode[40] = !!(day_tens & 0x01);
  timecode[41] = !!(day_tens & 0x02);

  // week day
  uint8_t weekday_ones = timecode_tm.tm_wday % 10;
  timecode[42] = !!(weekday_ones & 0x01);
  timecode[43] = !!(weekday_ones & 0x02);
  timecode[44] = !!(weekday_ones & 0x04);

  // month
  uint8_t month_ones = (timecode_tm.tm_mon+1) % 10;
  uint8_t month_tens = (timecode_tm.tm_mon+1) / 10;
  timecode[45] = !!(month_ones & 0x01);
  timecode[46] = !!(month_ones & 0x02);
  timecode[47] = !!(month_ones & 0x04);
  timecode[48] = !!(month_ones & 0x08);
  timecode[49] = !!(month_tens & 0x01);

  uint8_t year_ones = (timecode_tm.tm_year % 100) % 10;
  uint8_t year_tens = (timecode_tm.tm_year % 100) / 10;
  timecode[50] = !!(year_ones & 0x01);
  timecode[51] = !!(year_ones & 0x02);
  timecode[52] = !!(year_ones & 0x04);
  timecode[53] = !!(year_ones & 0x08);
  timecode[54] = !!(year_tens & 0x01);
  timecode[55] = !!(year_tens & 0x02);
  timecode[56] = !!(year_tens & 0x04);
  timecode[57] = !!(year_tens & 0x08);

  // date parity
  timecode[58] = 0;
  size_t offset;
  for (offset = 36; offset <=57; ++offset)
    timecode[58] += timecode[offset];
  timecode[58] %= 2;

  timecode[59] = 2;

  return 0;
}

int dcf77_printtimecode() {
  size_t offset;

  for (offset = 0; offset < 59; ++offset) {
    printf("%u", timecode[offset]);
    if ((offset == 0) || (offset == 14) || (offset == 20) || (offset == 28) || (offset == 35) || (offset == 41) || (offset == 44) || (offset == 49)){
      printf("-");
    }
  }

  printf("\n");

  return 0;
}

int dcf77_encode(const time_t* time) {
  struct tm *tmp = localtime(time);

  if ((timecode_tm.tm_min != tmp->tm_min) || (first == 1)) {
    dcf77_filltimecode(time);
    //dcf77_printtimecode();
    first = 0;
  }

  return timecode[tmp->tm_sec];
}