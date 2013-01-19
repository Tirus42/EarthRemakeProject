#include "tf/time.h"

#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
	#include <unistd.h>
#endif

uint32_t MilliSecs(){
	#ifdef WIN32
        return timeGetTime();
	#else
		struct timeval tv;
		gettimeofday(&tv, 0);
		return (int)(tv.tv_sec*1000+(tv.tv_usec/1000));
	#endif
}

void* CreateTimer(int ms) {
	#ifdef WIN32
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
	#ifdef WIN32
	WaitForSingleObject(hTimer, INFINITE);
	#endif
}

void FreeTimer(void* hTimer) {
	#ifdef WIN32
	CancelWaitableTimer((void*)hTimer);
	#endif
}

void Delay(uint32_t ms) {
	#ifdef WIN32
		Sleep(ms);
	#else
		usleep(ms * 1000);
	#endif // WIN32
}
