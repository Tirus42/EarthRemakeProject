#include "tf/time.h"

#ifdef _WIN32
	#include <windows.h>

	// Frequenz Wert von QueryPerformanceFrequency()
	uint64_t COUNTER_FREQ = 0;
#else
	#include <sys/time.h>
	#include <time.h>
	#include <unistd.h>
#endif

uint32_t MilliSecs(){
	#ifdef _WIN32
        return timeGetTime();
	#else
		struct timeval tv;
		gettimeofday(&tv, 0);
		return (int)(tv.tv_sec*1000+(tv.tv_usec/1000));
	#endif
}

void* CreateTimer(int ms) {
	#ifdef _WIN32
	LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -10000L * ms;

    HANDLE hTimer = CreateWaitableTimer(NULL, false, NULL);  //Erstellt den Timer
    if (hTimer == NULL)
		return NULL;

    if (!SetWaitableTimer(hTimer, &liDueTime, ms, NULL, NULL, 0)) {
		FreeTimer(hTimer);
        return NULL;
    }

	return hTimer;
	#else
	return 0;	// Noch nicht implementiert
	#endif
}

void WaitTimer(void* hTimer) {
	#ifdef _WIN32
	WaitForSingleObject(hTimer, INFINITE);
	#endif
}

void FreeTimer(void* hTimer) {
	#ifdef _WIN32
	CancelWaitableTimer((void*)hTimer);
	#endif
}

void Delay(uint32_t ms) {
	#ifdef _WIN32
		Sleep(ms);
	#else
		usleep(ms * 1000);
	#endif // _WIN32
}


bool InitHighResolutionTimer() {
	#ifdef _WIN32
	return QueryPerformanceFrequency((LARGE_INTEGER*)&COUNTER_FREQ);
	#else
	return true;
	#endif // _WIN32
}

void HighResolutionTime(uint64_t* target) {
	#ifdef _WIN32
	QueryPerformanceCounter((LARGE_INTEGER*)target);
	#else
	clock_gettime(CLOCK_MONOTONIC, (struct timespec*)target);
	#endif // _WIN32
}

double HighResolutionDiffSec(uint64_t first, uint64_t second) {
	#ifdef _WIN32
	uint64_t diff = second - first;

	return ((double)diff) / (double)COUNTER_FREQ;
	#else
	struct timespec& tFirst = (struct timespec&)first;
	struct timespec& tSecond = (struct timespec&)second;

	uint32_t sec = tSecond.tv_sec - tFirst.tv_sec;
	int32_t ns = tSecond.tv_nsec - tFirst.tv_nsec;

	return sec + ns * 0.000000001d;
	#endif // _WIN32
}

uint64_t HighResolutionDiffNanoSec(uint64_t first, uint64_t second) {
	#ifdef _WIN32
	uint64_t diff = (second - first);

	double ns = ((double)diff / (double)COUNTER_FREQ) * 1000000000;

	return ns;
	#else
	struct timespec& tFirst = (struct timespec&)first;
	struct timespec& tSecond = (struct timespec&)second;

	uint64_t sec = tSecond.tv_sec - tFirst.tv_sec;
	int64_t ns = tSecond.tv_nsec - tFirst.tv_nsec;

	return sec * 1000000000 + ns;
	#endif // _WIN32
}
