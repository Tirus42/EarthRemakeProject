#include "tf/time.h"

#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
#endif

unsigned int MilliSecs(){
	#ifdef WIN32
        return timeGetTime();
	#else
		struct timeval tv;
		gettimeofday(&tv, 0);
		return (int)(tv.tv_sec*1000+(tv.tv_usec/1000));
	#endif
}

int CreateTimer(int ms) {
	#ifdef WIN32
	LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -10000L * ms;

    HANDLE hTimer = CreateWaitableTimer(NULL, false, NULL);  //Erstellt den Timer
    if (hTimer == NULL)
		return -1;

    if (!SetWaitableTimer(hTimer, &liDueTime, ms, NULL, NULL, 0))
        return -1;

	return (int)hTimer;
	#else
	return -1;	// Noch nicht implementiert
	#endif
}

void WaitTimer(int hTimer) {
	#ifdef WIN32
	WaitForSingleObject((void*)hTimer, INFINITE);
	#endif
}

void FreeTimer(int hTimer) {
	#ifdef WIN32
	CancelWaitableTimer((void*)hTimer);
	#endif
}
