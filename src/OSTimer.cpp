#include "pch.h"

#include "OSTimer.h"
#include "zTimerServ/iTimerParent.h"


OSTimer::OSTimer() :
    parent(0),
    interval_msec(-1)
{
}


OSTimer::~OSTimer()
{
    stop();
}


void OSTimer::set_parent(iTimerParent* _parent)
{
    parent = _parent;
};


void OSTimer::set_interval(int milliseconds)
{
    interval_msec = milliseconds;
}


void OSTimer::start()
{
    Start(interval_msec);
}


void OSTimer::stop()
{
    Stop();
}


void OSTimer::Notify()
{
    if (parent)
        parent->OnTimer();
}
