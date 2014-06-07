#include "tf/thread.h"

using namespace thread;

thread_t thread::CreateThread(void *(*start_routine) (void*), void *arg) {
	#ifdef WIN32
		DWORD (__attribute__((__stdcall__)) *addr)(void*) = (LPTHREAD_START_ROUTINE)start_routine;

		// Windows-API CreateThread Function
		return ::CreateThread(NULL, 0, addr, arg, 0, NULL);
	#else
		thread_t hnd;

		int result = pthread_create(&hnd, NULL, start_routine, arg);

		if (result != 0)
			return 0;

		return hnd;
	#endif // WIN32
}

void thread::JoinThread(thread_t thread) {
	#ifdef WIN32
		WaitForSingleObject(thread, INFINITE);
	#else
		pthread_join(thread, 0);
	#endif // WIN32
}

void thread::FreeThread(thread_t thread) {
	#ifdef WIN32
		CloseHandle(thread);
	#else
		// Todo
	#endif // WIN32
}

void thread::YieldThread() {
	#ifdef WIN32
		SwitchToThread();
	#else
		pthread_yield();
	#endif // WIN32
}

int thread::InitMutex(mutex_t * mutex) {
	#ifdef WIN32
		InitializeCriticalSection(mutex);
		return 0;
	#else
		return pthread_mutex_init(mutex, NULL);
	#endif // WIN32
}

int thread::FreeMutex(mutex_t * mutex) {
	#ifdef WIN32
		DeleteCriticalSection(mutex);
		return 0;
	#else
		return pthread_mutex_destroy(mutex);
	#endif // WIN32
}

void thread::LockMutex(mutex_t * mutex) {
	#ifdef WIN32
		EnterCriticalSection(mutex);
	#else
		pthread_mutex_lock(mutex);
	#endif // WIN32
}

int thread::TryLockMutex(mutex_t * mutex) {
	#ifdef WIN32
		return TryEnterCriticalSection(mutex);
	#else
		return !pthread_mutex_trylock(mutex);
	#endif // WIN32
}

void thread::UnlockMutex(mutex_t * mutex) {
	#ifdef WIN32
		LeaveCriticalSection(mutex);
	#else
		pthread_mutex_unlock(mutex);
	#endif // WIN32
}
