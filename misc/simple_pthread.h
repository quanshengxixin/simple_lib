#ifndef __SIMPLE_PTHREAD_H__
#define __SIMPLE_PTHREAD_H__

#define __WINDOWS_PLATFORM__		1

#if __WINDOWS_PLATFORM__
#include <Windows.h>


#else

#include <pthread.h>

#endif // __WINDOWS_PLATFORM__

#endif // !__SIMPLE_PTHREAD_H__

