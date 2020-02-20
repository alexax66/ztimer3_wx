#include "pch.h"

#include "zReminderDlg.h"
#include "iTimeChooser.h"
#include "zTimerServ/time_utils.h"
#include "zTimerServ/zReminder.h"

#include "res_ids.h"


enum
{
	ID_BTN_OK = 1000,
	ID_BTN_DELETE,
	ID_BTN_SKIP,
};


//---------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(zReminderDlg, wxFrame)
    EVT_BUTTON(ID_BTN_OK, zReminderDlg::OnButtonClick)
	EVT_BUTTON(ID_BTN_DELETE, zReminderDlg::OnButtonClick)
	EVT_BUTTON(ID_BTN_SKIP, zReminderDlg::OnButtonClick)
	EVT_PAINT(zReminderDlg::OnPaint)
	EVT_MENU_RANGE(ID_SKIP_MENU_MIN, ID_SKIP_MENU_MAX, zReminderDlg::OnMenuRange)
END_EVENT_TABLE()


//---------------------------------------------------------------------------------------
zReminderDlg::zReminderDlg(iTimeChooser* _chooser, zTimerServ& _timer_serv, wxSize parent_size) :
	chooser(_chooser),
	timer_serv(_timer_serv),
	reminder_ind(-1)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);
	_create_Controls(parent_size);

	m_prev_refresh_time = LL_EMPTY_VAL;
}


//---------------------------------------------------------------------------------------
zReminderDlg::~zReminderDlg()
{
}


//---------------------------------------------------------------------------------------
void zReminderDlg::Init()
{
}


//---------------------------------------------------------------------------------------
void zReminderDlg::set_reminder(int ind)
{
	reminder_ind = ind;

	//zReminder& r = timer_serv.get_reminder(reminder_ind);
	//label.SetValue(r.get_text());
}


//---------------------------------------------------------------------------------------
void zReminderDlg::_create_Controls(wxSize parent_size)
{
	int id = wxID_ANY;
	wxSize size = wxSize(parent_size.x, 150);
	int style = wxSTAY_ON_TOP | wxNO_BORDER | wxFRAME_NO_TASKBAR;

	Create(0, id, wxEmptyString, wxDefaultPosition, size, style);

	//
	btnOK.Create(this, ID_BTN_OK, wxT("OK"), wxDefaultPosition, wxSize(80, 20));
	btnDelete.Create(this, ID_BTN_DELETE, wxT("Delete"), wxDefaultPosition, wxSize(80, 20));
	btnSkip.Create(this, ID_BTN_SKIP, wxT("Skip"), wxDefaultPosition, wxSize(80, 20));
	//label.Create(this, ID_BTN_CANCEL, wxEmptyString, wxDefaultPosition, wxSize(80, 20));

	btnOK.SetPosition(wxPoint(0, 0));
	btnDelete.SetPosition(wxPoint(btnOK.GetPosition().x + btnOK.GetSize().x, 0));
	btnSkip.SetPosition(wxPoint(btnDelete.GetPosition().x + btnDelete.GetSize().x, 0));
	//label.SetPosition(wxPoint(0, btnOK.GetPosition().y + btnOK.GetSize().y));

	//SetSize(GetSize().x, label.GetPosition().y + label.GetSize().y + 5);
}


//---------------------------------------------------------------------------------------
void zReminderDlg::OnButtonClick(wxCommandEvent& evt)
{
    if (evt.GetId() == ID_BTN_OK)
    {
        zReminder& r = timer_serv.get_reminder(reminder_ind);
        r.reset_reminder_time(false);
		Hide();
    }
	else
    if (evt.GetId() == ID_BTN_DELETE)
    {
        timer_serv.delete_reminder(reminder_ind);
		Hide();
    }
	else
    if (evt.GetId() == ID_BTN_SKIP)
    {
		if (!skip_menu.GetMenuItemCount())
		{
			skip_menu.Append(ID_SKIP_MENU_MIN + 0, wxT("&1 min"));
			skip_menu.Append(ID_SKIP_MENU_MIN + 1, wxT("&3 min"));
			skip_menu.Append(ID_SKIP_MENU_MIN + 2, wxT("&5 min"));
			skip_menu.Append(ID_SKIP_MENU_MIN + 3, wxT("&10 min"));
			skip_menu.Append(ID_SKIP_MENU_MIN + 4, wxT("&15 min"));
			skip_menu.Append(ID_SKIP_MENU_MIN + 5, wxT("&20 min"));
			skip_menu.Append(ID_SKIP_MENU_MIN + 6, wxT("&30 min"));
			skip_menu.Append(ID_SKIP_MENU_MIN + 7, wxT("1 &hour"));
		}
		PopupMenu(&skip_menu, btnSkip.GetPosition());
    }
}


//---------------------------------------------------------------------------------------
void zReminderDlg::OnMenuRange(wxCommandEvent &evt)
{
	int id = evt.GetId();
	int id2 = id - ID_SKIP_MENU_MIN;

	if (id2 >= 0 && id2 <= 7)
	{
		int _min = 0;
		if (id2 == 0) _min = 1;
		if (id2 == 1) _min = 3;
		if (id2 == 2) _min = 5;
		if (id2 == 3) _min = 10;
		if (id2 == 4) _min = 15;
		if (id2 == 5) _min = 20;
		if (id2 == 6) _min = 30;
		if (id2 == 7) _min = 60;
		timer_serv.skip_reminder(reminder_ind, tutl::hms2LL(0, _min, 0));
		Hide();
	}
}


//---------------------------------------------------------------------------------------
void zReminderDlg::OnTimer()
{
	wxLongLong cur_time = wxGetLocalTimeMillis();
	if (m_prev_refresh_time == LL_EMPTY_VAL || cur_time - m_prev_refresh_time > 200)
	{
		Refresh(false);
		m_prev_refresh_time == cur_time;
	}
}

//---------------------------------------------------------------------------------------
void zReminderDlg::OnPaint(wxPaintEvent& evt)
{
	wxAutoBufferedPaintDC dc(this);

	int min_val = 80;
	int _r = min_val + rand()%(256 - min_val);
	int _g = min_val + rand()%(256 - min_val);
	int _b = min_val + rand()%(256 - min_val);
    wxBrush b(wxColour(_r, _g, _b));
	dc.SetBackground(b);

	dc.Clear();

    wxString face_name;
	wxFont font1(20, wxFONTFAMILY_DEFAULT , wxNORMAL, wxFONTWEIGHT_BOLD, false, face_name);

	dc.SetTextForeground(*wxBLACK);

	dc.SetFont(font1);

	zReminder& r = timer_serv.get_reminder(reminder_ind);

	wxString text = r.get_text();
	if (r.get_exact_time())
		text += " (" + tutl::LL_to_string(r.get_period(), true) + ")";

	dc.DrawText(text, 5, 25);
}

