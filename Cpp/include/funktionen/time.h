#ifndef TIME_H
#define TIME_H

#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
#endif

unsigned int MilliSecs();


#endif // TIME_H
