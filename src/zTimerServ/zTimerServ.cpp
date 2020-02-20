#include "pch.h"

#include "zTimerServ.h"
#include "time_utils.h"

#include "zConfig.h"



zTimerServ::zTimerServ() :
    m_pTimer(0)
{
    parent = 0;

    m_bPaused = true;
    m_bWasPausedOnLock = false;

    m_StartTime = LL_EMPTY_VAL;
    m_GlobalStartTime = LL_EMPTY_VAL;
    m_TimerTime = 5 * 60 * 1000;
    m_TimeSpend = 0;

    m_bLoading = false;
}


void zTimerServ::init(iOSTimer* pTimer)
{
    m_bLoading = true;

    zConfig::inst().load_reminders(*this);

    m_pTimer = pTimer;
    if (m_pTimer)
    {
        m_pTimer->set_parent(this);
        m_pTimer->set_interval(80);

        star_stop_timer();
    }

    m_bLoading = false;
}


zTimerServ::~zTimerServ()
{
}


void zTimerServ::set_Parent(iTimerParent* _parent)
{
    parent = _parent;
};


void zTimerServ::OnTimer()
{
    if (parent)
        parent->OnTimer();
}


void zTimerServ::set_Paused(bool val)
{
    if (m_bPaused == val)
        return;

    wxLongLong cur_time = wxGetLocalTimeMillis();
    if (!val)
    {
        //pause mode off
        m_StartTime = cur_time;
        if (m_GlobalStartTime == LL_EMPTY_VAL)
            m_GlobalStartTime = cur_time;
    }
    else
    {
        //pause mode on
        wxASSERT(m_StartTime != LL_EMPTY_VAL && cur_time >= m_StartTime);
        m_TimerTime -= (cur_time - m_StartTime);
        m_TimeSpend += (cur_time - m_StartTime);
    }
    m_bPaused = val;
    star_stop_timer();
}


bool zTimerServ::get_Paused()
{
    return m_bPaused;
}


void zTimerServ::toggle_pause()
{
    set_Paused(!get_Paused());
}


void zTimerServ::set_TimeLeft(wxLongLong val)
{
    if (get_Paused())
        m_TimerTime = val;
    else
        m_TimerTime = val + (wxGetLocalTimeMillis() - m_StartTime);
}


void zTimerServ::init_Timer(wxLongLong val)
{
    m_bPaused = true;
    star_stop_timer();

    m_TimerTime = val;

    m_TimeSpend = 0;
    m_GlobalStartTime = LL_EMPTY_VAL;
}


void zTimerServ::init_TimerMinutes(wxLongLong val)
{
    init_Timer(val * 60 * 1000);
}


void zTimerServ::inc_timer(int hour, int min, int sec)
{
    set_TimeLeft(get_TimeLeft() + tutl::hms2LL(hour, min, sec));
}


void zTimerServ::inc_timer(wxLongLong msec)
{
    set_TimeLeft(get_TimeLeft() + msec);
}


void zTimerServ::dec_timer(int hour, int min, int sec)
{
    set_TimeLeft(get_TimeLeft() - tutl::hms2LL(hour, min, sec));
}


wxLongLong zTimerServ::get_TimeLeft()
{
    if (get_Paused())
        return m_TimerTime != LL_EMPTY_VAL ? m_TimerTime : 0;

    wxLongLong dt = m_TimerTime - (wxGetLocalTimeMillis() - m_StartTime);
    return dt;
}


wxLongLong zTimerServ::get_TimeSpend()
{
    if (get_Paused())
        return m_TimeSpend;

    wxLongLong dt = m_TimeSpend + (wxGetLocalTimeMillis() - m_StartTime);
    return dt;
}


wxLongLong zTimerServ::get_GlobalTimeSpend()
{
    if (m_GlobalStartTime == LL_EMPTY_VAL)
        return 0;

    return wxGetLocalTimeMillis() - m_GlobalStartTime;
}


void zTimerServ::getCurStr_TimeLeft(wxString& ret_str, bool& ret_neg)
{
    wxLongLong t = get_TimeLeft();
    ret_neg = false;

    if (t < 0)
    {
        t = -t;
        ret_neg = true;
    }

    ret_str = tutl::LL_to_string(t, true);
}


wxString zTimerServ::getCurStr_TimeSpend()
{
    return tutl::LL_to_string(get_TimeSpend());
}


wxString zTimerServ::getCurStr_GlobalTimeSpend()
{
    return tutl::LL_to_string(get_GlobalTimeSpend(), true);
}


wxString zTimerServ::getCurStr_CurrentTime()
{
    wxDateTime now = wxDateTime::Now();
    int h = now.GetHour();
    int m = now.GetMinute();
    int s = now.GetSecond();

    return wxString::Format("%02d:%02d:%02d", h, m, s);
}


void zTimerServ::star_stop_timer()
{
    if (!m_pTimer)
    {
        wxASSERT(0);
        return;
    }
    //if (m_bPaused && m_GlobalStartTime == LL_EMPTY_VAL)
    //    m_pTimer->stop();
    //else
        m_pTimer->start();
}


void zTimerServ::on_lock_screen()
{
    m_bWasPausedOnLock = false;
    if (!get_Paused())
    {
        set_Paused(true);
        m_bWasPausedOnLock = true;
    }
}


void zTimerServ::on_unlock_screen()
{
    if (m_bWasPausedOnLock)
    {
        m_bWasPausedOnLock = false;
        set_Paused(false);
    }
}


void zTimerServ::add_reminder(wxLongLong period_msec, const wxString& text, bool exact_time)
{
    zReminder r;
    r.init(period_msec, text, exact_time, m_bLoading);
    r.set_paused(false);

    reminders.push_back(r);

    if (!m_bLoading)
    {
        zConfig::inst().save_reminders(*this, false);
        zConfig::inst().flush();
    }
}


int zTimerServ::get_reminders_cnt()
{
    return (int)reminders.size();
}


zReminder& zTimerServ::get_reminder(int ind)
{
    if (ind < 0 || ind >=get_reminders_cnt())
    {
        wxASSERT(0);
        static zReminder tmp;
        return tmp;
    };
    return reminders[ind];
}

void zTimerServ::delete_reminder(int ind)
{
    if (ind < 0 || ind >=get_reminders_cnt())
    {
        wxASSERT(0);
        return;
    }

    reminders.erase(reminders.begin() + ind);

    zConfig::inst().save_reminders(*this, true);
    zConfig::inst().flush();
}


void zTimerServ::skip_reminder(int ind, wxLongLong period_msec)
{
    if (ind < 0 || ind >=get_reminders_cnt())
    {
        wxASSERT(0);
        return;
    }

    reminders[ind].skip_some_time(period_msec);
}
