#include "time.h"

///Wenn man das Hier drin l�sst, macht das keine solche Probleme :)

#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
#endif

unsigned int MilliSecs(){
	#ifdef WIN32
        return timeGetTime();   ///THX.. wenn so fehler kommen fehlt ne lib.. und lib*.a kannste k�rzer schreiben.. wo haste das mingw verz?
	#else
		struct timeval tv;
		gettimeofday(&tv, 0);
		return (int)(tv.tv_sec*1000+(tv.tv_usec/1000));
	#endif
}
