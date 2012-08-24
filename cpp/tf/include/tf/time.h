#ifndef TIME_H
#define TIME_H

// Benötigt "winmm.lib"

/// Gibt die Laufzeit des PCs in Millisekunden zurück
unsigned int MilliSecs();

/**
* Erstellt einen Timer mit einer bestimmten Tickrate in Millisekunden
* @return Den Timer oder -1, falls er nicht erstellt werden konnte
*/
int CreateTimer(int ms);

/// Wartet so lange bis der Timer einen "Tick" auslöst
void WaitTimer(int hTimer);

#endif // TIME_H
