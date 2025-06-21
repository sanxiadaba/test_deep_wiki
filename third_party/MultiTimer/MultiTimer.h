#ifndef _MULTI_TIMER_H_
#define _MULTI_TIMER_H_

#include <stdint.h>

typedef uint64_t (*PlatformTicksFunction_t)(void);

typedef struct MultiTimerHandle MultiTimer;

typedef void (*MultiTimerCallback_t)(MultiTimer* timer, void* userData);

struct MultiTimerHandle
{
    MultiTimer* next;
    uint64_t deadline;
    MultiTimerCallback_t callback;
    void* userData;
};

void multiTimerInstall(PlatformTicksFunction_t ticksFunc);

void multiTimerStart(MultiTimer* timer, uint64_t timing, MultiTimerCallback_t callback, void* userData);

void multiTimerStop(MultiTimer* timer);

void multiTimerYield(void);


#endif
