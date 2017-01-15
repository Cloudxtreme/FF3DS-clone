#include "runtime.h"

int UpTime() {
	time_t unixTime = time(NULL);
	struct tm* timeStruct = gmtime((const time_t *)&unixTime);

	int secs = timeStruct->tm_sec;
	int mins = timeStruct->tm_min;
	int hrs = timeStruct->tm_hour;




	printf("\x1b[36mRunning time: %i\x1b\n", secs);

	return secs;
}