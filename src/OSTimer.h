#pragma once

#include <wx/timer.h>
#include "zTimerServ/iOSTimer.h"


class iTimerParent;


class OSTimer: public wxTimer, public iOSTimer
{
    iTimerParent* parent;

    int interval_msec;

public:
    OSTimer();
    virtual ~OSTimer();

    void set_parent(iTimerParent* _parent);
    virtual void set_interval(int milliseconds=-1);
    virtual void start();
    virtual void stop();

private:
    virtual void Notify();
};
