#include <3ds.h>
#include <time.h>
#include <citro3d.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "player.h"

time_t unixTime;
struct tm* timeStruct;

int stSec;
int stMin;
int stHr;
int gSec;
int fps;

void StTime();
int fpsCalc(int secs);
int UpTime();
