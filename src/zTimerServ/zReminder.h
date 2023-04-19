#pragma once

#include <wx/longlong.h>

class zReminder
{
    wxLongLong m_period;    //msec. 1) period of reminder on usual mode; 2)exact time to show reminder on m_exact_time mode
    wxString m_text;
    bool m_paused;
    wxLongLong m_ReminderTime;        //time, when 00:00:00 will happen
    wxLongLong m_PausedTimeLeft;

    bool m_exact_time;        //show in exact time
    wxLongLong m_PrevShowTimeOnExact;

public:
    zReminder();
    virtual ~zReminder();

    void init(wxLongLong period_msec, wxString text, bool exact_time, bool loading);
    wxString get_text();
    wxLongLong get_period() { return m_period; }
    wxString get_menu_text();
    wxString get_time_text();
    bool get_exact_time();

    void set_paused(bool paused);
    bool get_paused();
    void toggle_paused() {set_paused(!get_paused()); };

    wxLongLong get_time_left();

    bool need_to_show();

    void reset_reminder_time(bool loading);
    void skip_some_time(wxLongLong skip_time);
};

