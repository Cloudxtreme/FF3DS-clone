#include "runtime.h"

void StTime() {

	time_t unixTime = time(NULL);
	struct tm* timeStruct = gmtime((const time_t *)&unixTime);
	stSec = timeStruct->tm_sec;
	stMin = timeStruct->tm_min;
	stHr = timeStruct->tm_hour;

}

int UpTime() {

	time_t unixTime = time(NULL);
	struct tm* timeStruct = gmtime((const time_t *)&unixTime);
	//This may bug out if the game is started before midnight. 
	//But it shouldn't matter as we're only using it for timing
	//and the seconds will still be accurate.
	int secs = (timeStruct->tm_sec - stSec);
	int mins = (timeStruct->tm_min - stMin);
	int hrs = (timeStruct->tm_hour - stHr);		//probably not needed. We'll see

	int i = 0;
	while(i < hrs) {
		mins += 60;
		i++;
	}
	i=0;
	while(i < mins) {
		secs += 60;
		i++;
	}
	printf("\x1b[36mRunning time: %i\x1b\n", secs);
	return secs;
}