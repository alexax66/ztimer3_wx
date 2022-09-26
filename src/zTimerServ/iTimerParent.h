#pragma once


class iTimerParent
{
public:
    virtual ~iTimerParent() = default;
    virtual void OnTimer() = 0;
};
