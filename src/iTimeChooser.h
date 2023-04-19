#pragma once

#include <wx/datetime.h>

class iTimeChooser
{
public:
    virtual ~iTimeChooser() = default;
    virtual void OnTimeChoose(wxDateTime _time) = 0;
    virtual void OnReminderAdd(wxLongLong msec, const wxString& text, bool exact_time) = 0;
};
