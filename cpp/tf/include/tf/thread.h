#ifndef THREAD_H_INCLUDED
#define THREAD_H_INCLUDED

//Unter Linux muss zu "pthread" gelinkt werden

#ifdef WIN32
	#include <windows.h>

	typedef HANDLE thread_t;
	typedef CRITICAL_SECTION mutex_t;	// Nicht wirklich ein Mutex, kommt aber aufs selbe raus, und ist schneller.
#else
	#include <pthread.h>

	typedef pthread_t thread_t;
	typedef pthread_mutex_t mutex_t;
#endif

namespace thread {

/// Erstellt einen neuen Thread, welcher in der angegebenen Funktion startet.
thread_t CreateThread(void *(*start_routine) (void *), void *arg);

/// Wartet auf die Beendigung des Threads.
void JoinThread(thread_t thread);

/// Gibt Thread wieder frei.
void FreeThread(thread_t thread);

/// Signalisiert dem OS zu einem anderen Thread zu wechseln
void YieldThread();

/// Initiert einen Mutex, nötig bevor er verwendet werden kann.
/// \return 0 bei Erfolg, != 0 im Fehlerfall
int InitMutex(mutex_t * mutex);

/// Gibt einen Mutex wieder frei, nötig bevor der Speicher von dem Prozess freigegeben werden kann.
/// \return 0 bei Erfolg, != 0 im Fehlerfall
int FreeMutex(mutex_t * mutex);

/// Sperrt einen Mutex für exklusieren Zugriff.
/// Wartet, falls nötig, bis er gesperrt werden kann.
void LockMutex(mutex_t * mutex);

/// Versucht den Mutex zu sperren, bricht jedoch ab, wenn dieser bereits gesperrt ist.
/// \return 1 wenn der Mutex gelockt wurde, 0 wenn er bereits belegt ist.
int TryLockMutex(mutex_t * mutex);

/// Gibt den Zugriff auf den Mutex wieder frei.
void UnlockMutex(mutex_t * mutex);

}




#endif // THREAD_H_INCLUDED
