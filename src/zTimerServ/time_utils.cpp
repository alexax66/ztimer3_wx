#include "pch.h"

#include "time_utils.h"


wxLongLong tutl::get_cur_time()
{
    return wxGetLocalTimeMillis();
}


wxLongLong tutl::get_day_time(wxLongLong time)
{
    wxLongLong ret = time % (1000 * 24 * 3600);
    return ret;
}

/*
wxLongLong tutl::get_prev_day_time(wxLongLong time)
{
    wxLongLong day_time = time % (1000 * 24 * 3600);
    return time - day_time;
}
*/

wxString tutl::LL_to_string(wxLongLong _val, bool no_msec)
{
    wxLongLong_t val = _val.GetValue();

    int msec = val % 1000;
    val /= 1000;

    int sec = val % 60;
    val /= 60;

    int mm = val % 60;
    val /= 60;

    int hr = val % 60;
    val /= 60;

    wxString s;
    if (!no_msec)
        s = wxString::Format("%02d:%02d:%02d:%03d", hr, mm, sec, msec);
    else
        s = wxString::Format("%02d:%02d:%02d", hr, mm, sec);

    return s;
}
