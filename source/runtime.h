#include <3ds.h>
#include <time.h>
#include <citro3d.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

time_t unixTime;
struct tm* timeStruct;

int stSec;
int stMin;
int stHr;

void StTime();
int UpTime();
