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
