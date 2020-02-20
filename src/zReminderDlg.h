#pragma once

#include <wx/frame.h>
#include <wx/button.h>
#include <wx/menu.h>

#include "zTimerServ/zTimerServ.h"

class iTimeChooser;

class zReminderDlg: public wxFrame
{
	iTimeChooser* chooser;
	zTimerServ& timer_serv;

    DECLARE_EVENT_TABLE()

	wxButton btnOK;
	wxButton btnDelete;
	wxButton btnSkip;
	//wxTextCtrl label;

	wxMenu skip_menu;

	int reminder_ind;

	wxLongLong m_prev_refresh_time;

public:
	zReminderDlg(iTimeChooser* _chooser, zTimerServ& _timer_serv, wxSize parent_size);
	virtual ~zReminderDlg();

	void Init();
	void set_reminder(int ind);

	void OnTimer();

protected:
	virtual void OnPaint(wxPaintEvent& evt);

    void OnButtonClick(wxCommandEvent& evt);
	void OnMenuRange(wxCommandEvent &evt);

	void _create_Controls(wxSize parent_size);
};

