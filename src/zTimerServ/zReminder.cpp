#include "pch.h"

#include "zReminder.h"
#include "time_utils.h"


zReminder::zReminder()
{
    m_period = LL_EMPTY_VAL;
    m_paused = true;
    m_ReminderTime = LL_EMPTY_VAL;
    m_PausedTimeLeft = LL_EMPTY_VAL;
    m_exact_time = false;
}


zReminder::~zReminder()
{
}


void zReminder::init(wxLongLong period_msec, wxString text, bool exact_time, bool loading)
{
    m_period = period_msec;
    m_text = text;
    m_exact_time = exact_time;
    reset_reminder_time(loading);
}


wxString zReminder::get_text()
{
    return m_text;
}


wxString zReminder::get_menu_text()
{
    wxString ret;

    //wxLongLong t = get_time_left();
    //ret += tutl::LL_to_string(t, true);

    ret += " \"" + m_text + "\"";

    if (m_exact_time)
        ret += " (" + tutl::LL_to_string(m_period, true) + ")";

    return ret;
}


wxString zReminder::get_time_text()
{
    //if (m_exact_time)
    //    return tutl::LL_to_string(m_period, true);

    wxLongLong t = get_time_left();
    return tutl::LL_to_string(t, true);
}


bool zReminder::get_exact_time()
{
    return m_exact_time;
}


void zReminder::set_paused(bool paused)
{
    if (m_exact_time)
    {
        m_paused = false;
        return;
    }

    if (m_paused != paused)
    {
        if (paused)
            m_PausedTimeLeft = get_time_left();
        else
        {
            if (m_PausedTimeLeft != LL_EMPTY_VAL)
                m_ReminderTime = tutl::get_cur_time() + m_PausedTimeLeft;
            else
                m_ReminderTime = tutl::get_cur_time() + m_period;
        }
        m_paused = paused;
    }
}


bool zReminder::get_paused()
{
    return m_paused;
}


wxLongLong zReminder::get_time_left()
{
    /*
    if (m_exact_time)
    {
        wxLongLong ret = m_period - tutl::get_cur_time();
        if (ret < 0)
            ret += 24 * 60 * 60;
        return ret;
    }*/

    if (get_paused() && !m_exact_time)
        return m_PausedTimeLeft == LL_EMPTY_VAL ? 0 : m_PausedTimeLeft;
    else
    {
        if (m_ReminderTime == LL_EMPTY_VAL)
            reset_reminder_time(false);

        wxLongLong dt = m_ReminderTime - tutl::get_cur_time();
        if (dt < 0)
            dt = 0;
        return dt;
    }
}


void zReminder::reset_reminder_time(bool loading)
{
    if (!m_exact_time)
    {
        m_ReminderTime = tutl::get_cur_time() + m_period;
        m_PausedTimeLeft = m_period;
    }
    else
    {
        wxLongLong cur_time = tutl::get_cur_time();

        m_ReminderTime = cur_time - tutl::get_day_time(cur_time) + m_period;
        if (cur_time >= m_ReminderTime && !loading)        //(!loading) is needed to show reminders, which occured, when app was closed
            m_ReminderTime += 24 * 3600 * 1000;

        m_PausedTimeLeft = m_ReminderTime - cur_time;
    }
}


void zReminder::skip_some_time(wxLongLong skip_time)
{
    if (get_time_left() > 0)
        m_ReminderTime += skip_time;
    else
        m_ReminderTime = tutl::get_cur_time() + skip_time;
}


bool zReminder::need_to_show()
{
    return !get_paused() && get_time_left() <= 0;
}
