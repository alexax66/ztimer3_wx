#pragma once

class iTimerParent;


class iOSTimer
{
public:
    virtual ~iOSTimer() = default;
    virtual void set_parent(iTimerParent* _parent) = 0;
    virtual void set_interval(int milliseconds=-1) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
};
