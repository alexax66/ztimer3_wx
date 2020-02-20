#pragma once

#include <wx/longlong.h>
#include <vector>

#include "iTimerParent.h"
#include "iOSTimer.h"

#include "zReminder.h"


class zTimerServ : public iTimerParent
{
	iTimerParent* parent;
	iOSTimer* m_pTimer;

	bool m_bPaused;
	bool m_bWasPausedOnLock;

	bool m_bLoading;

	wxLongLong m_StartTime;
	wxLongLong m_GlobalStartTime;
	wxLongLong m_TimerTime;
	wxLongLong m_TimeSpend;

	std::vector<zReminder> reminders;

public:
	zTimerServ();
	virtual ~zTimerServ();

	virtual void init(iOSTimer* pTimer);

	void set_Parent(iTimerParent* _parent);

	virtual void OnTimer();

	void set_Paused(bool val);
	bool get_Paused();
	void toggle_pause();

	void set_TimeLeft(wxLongLong val);
	wxLongLong get_TimeLeft();

	wxLongLong get_TimeSpend();
	wxLongLong get_GlobalTimeSpend();

	void init_Timer(wxLongLong val);
	void init_TimerMinutes(wxLongLong val);

	void inc_timer(int hour, int min, int sec);
	void inc_timer(wxLongLong msec);

	void dec_timer(int hour, int min, int sec);

	void getCurStr_TimeLeft(wxString& ret_str, bool& ret_neg);
	wxString getCurStr_TimeSpend();
	wxString getCurStr_GlobalTimeSpend();
	wxString getCurStr_CurrentTime();

	void on_lock_screen();
	void on_unlock_screen();

	void add_reminder(wxLongLong period_msec, const wxString& text, bool exact_time);
	int get_reminders_cnt();
	zReminder& get_reminder(int ind);
	void delete_reminder(int ind);
	void skip_reminder(int ind, wxLongLong period_msec);

protected:
	void star_stop_timer();
};

