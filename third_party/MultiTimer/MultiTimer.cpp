#include "MultiTimer.h"

#include <stdio.h>

static MultiTimer* timerList = NULL;
static PlatformTicksFunction_t platformTicksFunction = NULL;

void multiTimerInstall(PlatformTicksFunction_t ticksFunc)
{
    platformTicksFunction = ticksFunc;
}

static void removeTimer(MultiTimer* timer)
{
    MultiTimer** current = &timerList;
    while (*current)
    {
        if (*current == timer)
        {
            *current = timer->next;
            break;
        }
        current = &(*current)->next;
    }
}

void multiTimerStart(MultiTimer* timer, uint64_t timing, MultiTimerCallback_t callback, void* userData)
{
    removeTimer(timer);

    timer->deadline = platformTicksFunction() + timing;
    timer->callback = callback;
    timer->userData = userData;

    MultiTimer** current = &timerList;
    while (*current && ((*current)->deadline < timer->deadline))
    {
        current = &(*current)->next;
    }
    timer->next = *current;
    *current = timer;
}

void multiTimerStop(MultiTimer* timer)
{
    removeTimer(timer);
}

void multiTimerYield(void)
{
    uint64_t currentTicks = platformTicksFunction();
    while (timerList && (currentTicks >= timerList->deadline))
    {
        MultiTimer* timer = timerList;
        timerList = timer->next;

        if (timer->callback)
        {
            timer->callback(timer, timer->userData); // Execute callback
        }
    }
}
