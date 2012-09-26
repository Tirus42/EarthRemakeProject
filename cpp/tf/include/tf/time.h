#ifndef TIME_H
#define TIME_H

// Benötigt "winmm.lib"

#ifdef WIN32
	#include <windows.h>
#else
	#include <sys/time.h>
#endif

/// Gibt die Laufzeit des PCs in Millisekunden zurück
unsigned int MilliSecs();

/**
* Erstellt einen Timer mit einer bestimmten Tickrate in Millisekunden
* @return Den Timer oder -1, falls er nicht erstellt werden konnte
*/
HANDLE CreateTimer(int ms);

/// Wartet so lange bis der Timer einen "Tick" auslöst
void WaitTimer(HANDLE hTimer);

/// Gibt einen Timer wieder frei
void FreeTimer(HANDLE hTimer);

#endif // TIME_H
