#ifndef EXTERNALDEBUGLOG_H_INCLUDED
#define EXTERNALDEBUGLOG_H_INCLUDED


#ifdef NDEBUG
/*
 * If not debugging, assert does nothing.
 */
#define assert(x)	((void)0)

#else /* debugging enabled */

extern SOCKET debugConnection = null;

/*
 * CRTDLL nicely supplies a function which does the actual output and
 * call to abort.
 */
_CRTIMP void __cdecl __MINGW_NOTHROW _assert (const char*, const char*, int) __MINGW_ATTRIB_NORETURN;

/*
 * Definition of the assert macro.
 */
#define assert(e)       ((e) ? (void)0 : _assert(#e, __FILE__, __LINE__))

#endif	/* NDEBUG */


#endif // EXTERNALDEBUGLOG_H_INCLUDED
