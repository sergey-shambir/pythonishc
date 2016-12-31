#include "RandomSequence.hpp"

////////////////////////////////////////////////////////////////////////////////
// GetTickCount() WinAPI function
#if defined(_WIN32)

#include <windows.h>

#else

#include <time.h>
#include <sys/time.h>
#include <unistd.h>
static
unsigned long long GetTickCount()
{
    timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    unsigned long long sec = ts.tv_sec;
    unsigned long long nanosec = ts.tv_nsec;

    return sec * 1000 + nanosec / 1000000;
}

#endif
////////////////////////////////////////////////////////////////////////////////

// Copied from POSIX.1-2001 example of libc rand() function
const int RandomSequence::MAX_VALUE = 32768;

// Use only milliseconds from system time
RandomSequence::RandomSequence()
    :_next(GetTickCount() % 1000)
{
}

// Based on POSIX.1-2001 example of libc rand() function
int RandomSequence::operator ()() const
{
    _next = _next * 1103515245 + 12345;
    if (_next < 0)
        _next = -(_next);
    return ((_next / 65537) % MAX_VALUE);
}
