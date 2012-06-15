#include "time.h"

unsigned int MilliSecs(){
	#ifdef WIN32
        return timeGetTime();   ///THX.. wenn so fehler kommen fehlt ne lib.. und lib*.a kannste kürzer schreiben.. wo haste das mingw verz?
	#else
		struct timeval tv;
		gettimeofday(&tv, 0);
		return (int)(tv.tv_sec*1000+(tv.tv_usec/1000));
	#endif
}
