#ifndef TIME_H
#define TIME_H

// Ben�tigt unter Windows "winmm.lib"

#include <stdint.h>

/// Gibt die Laufzeit des PCs in Millisekunden zur�ck
uint32_t MilliSecs();

/**
* Erstellt einen Timer mit einer bestimmten Tickrate in Millisekunden
* @return Den Timer oder -1, falls er nicht erstellt werden konnte
*/
void* CreateTimer(int ms);

/// Wartet so lange bis der Timer einen "Tick" ausl�st
void WaitTimer(void* hTimer);

/// Gibt einen Timer wieder frei
void FreeTimer(void* hTimer);

/// Pausiert die Ausf�hrung um die angegebene Anzahl an Millisekunden
void Delay(uint32_t ms);

#endif // TIME_H
