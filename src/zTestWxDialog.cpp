#include "pch.h"

#include "res_ids.h"

#include "zTestWxDialog.h"
#include "zConfig.h"

#include "zChooseTimeDlg.h"
#include "zAddReminderDlg.h"
#include "zReminderDlg.h"


zTestWxDialog::zTestWxDialog(wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style) :
	GUIDialog(parent, id, title, pos, size, style)
{
	SetBackgroundStyle(wxBG_STYLE_PAINT);

	ShowTrayIcon();
	timer_serv.set_Parent(this);

	timer_serv.init(&timer);

	lbdown_point_dd.x = -1;
	lbdown_point_screen = lbdown_point_dd;

	dragging_type = DT_NONE;

	gui_btn_under_mouse = -1;

	m_pChooseTimeDlg = 0;
	m_pAddReminderDlg = 0;
	m_pReminderDlg = 0;

	m_prev_key = INT_MAX;
}

zTestWxDialog::~zTestWxDialog()
{
	if (HasCapture())
		ReleaseMouse();

	delete m_pChooseTimeDlg;
	m_pChooseTimeDlg = 0;

	delete m_pAddReminderDlg;
	m_pAddReminderDlg = 0;

	delete m_pReminderDlg;
	m_pReminderDlg = 0;
}


void zTestWxDialog::OnClose(wxCloseEvent &event)
{
	zConfig::inst().set_dialog_pos(GetPosition());
	Destroy();
}

void zTestWxDialog::OnQuit(wxCommandEvent &event)
{
	zConfig::inst().set_dialog_pos(GetPosition());
	Destroy();
}

void zTestWxDialog::OnAbout(wxCommandEvent &event)
{
	wxString msg;
	wxMessageBox(msg, _("Welcome to..."));
}

void zTestWxDialog::OnLeftDown(wxMouseEvent &evt)
{
	wxPoint point = evt.GetPosition();
	lbdown_point_screen = ClientToScreen(point);

	wxPoint cur_pos = GetPosition();
	lbdown_point_dd.x = lbdown_point_screen.x - cur_pos.x;
	lbdown_point_dd.y = lbdown_point_screen.y - cur_pos.y;

	dragging_type = DT_BEGIN;

	if (!HasCapture())
		CaptureMouse();
}


void zTestWxDialog::OnLeftUp(wxMouseEvent &evt)
{
	if (dragging_type == DT_NONE || dragging_type == DT_BEGIN)
	{
		//try press btn
		wxPoint lbdown_point = ScreenToClient(lbdown_point_screen);
		int btn1 = _get_gui_btn(lbdown_point);
		if (btn1 != -1)
		{
			int btn2 = _get_gui_btn(evt.GetPosition());
			if (btn1 == btn2)
			{
				if (btn1 >= 0 && btn1 < (int)gui_btns.size())
				{
					_process_cmd(gui_btns[btn1].id);
					Refresh(false);
				}
				else
					wxASSERT(0);
			}
		}
	}
	else
		end_window_drag();

	if (HasCapture())
		ReleaseMouse();
}


void zTestWxDialog::OnLeftDClick(wxMouseEvent &evt)
{
	if (!gui_btns.empty())
	{
		int left_btn = gui_btns[0].x;
		if (evt.GetPosition().x < left_btn)
			timer_serv.toggle_pause();
	}
}


void zTestWxDialog::OnRightDown(wxMouseEvent &evt)
{
	create_context_menu(evt.GetPosition());
}


void zTestWxDialog::OnMouseMove(wxMouseEvent &evt)
{
	wxPoint cur = evt.GetPosition();

	int prev = gui_btn_under_mouse;
	gui_btn_under_mouse = _get_gui_btn(evt.GetPosition());
	if (prev != gui_btn_under_mouse)
		Refresh(false);

	//process dragging
	if (dragging_type == DT_NONE)
	{
		_set_capture_on_move(evt.GetPosition());
		return;
	}

	if (!evt.LeftIsDown())
	{
		end_window_drag();
		return;
	}

	cur = ClientToScreen(cur);

	wxPoint cur_pos = GetPosition();
	wxPoint cur_dd;
	cur_dd.x = cur.x - cur_pos.x;
	cur_dd.y = cur.y - cur_pos.y;

	//
	int ddx = cur_dd.x - lbdown_point_dd.x;
	int ddy = cur_dd.y - lbdown_point_dd.y;
	if ((dragging_type == DT_DRAG) || ((ddx != 0 || ddy != 0) && (ddx*ddx + ddy*ddy > 25)))
	{
		dragging_type = DT_DRAG;

		cur_pos.x += ddx;
		cur_pos.y += ddy;
		SetPosition(cur_pos);
	}
}

void zTestWxDialog::OnMouseCaptureLost(wxMouseCaptureLostEvent& evt)
{
	end_window_drag();
}


void zTestWxDialog::OnMove(wxMoveEvent &evt)
{
	_set_child_dlgs_pos();
}


void zTestWxDialog::create_context_menu(wxPoint point)
{
	context_menu.create(timer_serv);
	context_menu.fill_reminders_submenu(timer_serv);
	PopupMenu(&context_menu, point);
}


void zTestWxDialog::OnTimer()
{
	check_reminders();
	if (m_pReminderDlg && m_pReminderDlg->IsShown())
		m_pReminderDlg->OnTimer();

	context_menu.refresh_reminders_submenu(timer_serv);

	_check_on_top();
	process_keys_workaround();

	Refresh(false);
}


void zTestWxDialog::ShowTrayIcon(bool doShow)
{
	if (doShow == taskBarIcon.IsIconInstalled())
		return;

#ifdef __WINDOWS__
	if (doShow)
	{
        wxIcon trayIcon(_T("main.ico"), wxBITMAP_TYPE_ICO);
		taskBarIcon.SetIcon(trayIcon);
	}
	else
	{
		taskBarIcon.RemoveIcon();
	}
#endif // __WINDOWS__
}


void zTestWxDialog::OnPaint(wxPaintEvent& evt)
{
	wxAutoBufferedPaintDC dc(this);

	dc.SetBackground(*wxBLACK);
	//dc.SetBackground(*wxLIGHT_GREY);

	dc.Clear();

	wxString face_name;
	//face_name = "Helvetica";

	wxFont font1(8, wxFONTFAMILY_DEFAULT , wxNORMAL, wxNORMAL, false, face_name);
	wxFont font2(12, wxFONTFAMILY_DEFAULT , wxNORMAL, wxFONTWEIGHT_BOLD, false, face_name);

	dc.SetTextForeground(*wxLIGHT_GREY);

	int x = 2;
	int y = 0;
	int y2 = y;// + 2;

	//
	wxString s = timer_serv.getCurStr_TimeSpend();
	dc.SetFont(font1);
	dc.DrawText(s, x, y2);

	wxSize sz = dc.GetTextExtent(s);
	x += sz.x + 5;

	//
	wxString ret_str;
	bool ret_neg;
	timer_serv.getCurStr_TimeLeft(ret_str, ret_neg);
	dc.SetFont(font2);
	dc.SetTextForeground(ret_neg ? *wxRED : *wxWHITE);

	bool need_blink = ret_neg;
	bool bShow = true;
	wxLongLong cur_time = wxGetLocalTimeMillis();
	if (need_blink)
		bShow = (cur_time % 500) > 250;
	//
	if(bShow)
		dc.DrawText(ret_str, x, y);

	//
	dc.SetTextForeground(*wxLIGHT_GREY);

	sz = dc.GetTextExtent(ret_str);
	x += sz.x + 5;

	//
	s = timer_serv.getCurStr_GlobalTimeSpend();
	dc.SetFont(font1);
	dc.DrawText(s, x, y2);

	sz = dc.GetTextExtent(s);
	x += sz.x;

	//
	bool is_show_curtime = (zConfig::inst().get_is_show_curtime() == 1);
	if (is_show_curtime)
	{
		x += 2;

		s = timer_serv.getCurStr_CurrentTime();
		dc.SetFont(font2);
		dc.DrawText(s, x, y2);
		sz = dc.GetTextExtent(s);
		x += sz.x;
	}

	//buttons
	x += 4;
	x = _draw_buttons(dc, x);

	x += 2;
	wxSize cur_size = GetClientSize();
	if (x != cur_size.x && x > 100)
		SetClientSize(x, cur_size.y);
}


int zTestWxDialog::_draw_buttons(wxAutoBufferedPaintDC& dc, int x)
{
	gui_btns.clear();
	int _gui_btn_under_mouse = gui_btn_under_mouse;

	int y = 3;
	int ww = 10;
	int hh = 10;
	int ww2 = ww / 2;
	int hh2 = hh / 2;
	std::vector<wxPoint> points;

	wxPen pen(*wxWHITE, 1);
	wxPen pen2(*wxGREEN, 1);
	//dc.SetPen(*wxWHITE_PEN);

	//play/pause
	bool bPause = timer_serv.get_Paused();
	dc.SetPen(_gui_btn_under_mouse == 0 ? pen2 : pen);
	gui_btns.push_back(zGuiButton(x, x+ww, ID_TOGGLE_PAUSE));
	if (bPause)
	{
		points.push_back(wxPoint(x, y));
		points.push_back(wxPoint(x + ww, y + hh/2));
		points.push_back(wxPoint(x, y + hh));
		points.push_back(wxPoint(x, y));
		dc.DrawLines((int)points.size(), &(points[0]));
	}
	else
	{
		int ww3 = ww /3;
		dc.DrawLine(wxPoint(x+ww3, y), wxPoint(x+ww3, y+hh));
		dc.DrawLine(wxPoint(x+ww3-1, y), wxPoint(x+ww3-1, y+hh));
		dc.DrawLine(wxPoint(x+hh-ww3, y), wxPoint(x+hh-ww3, y+hh));
		dc.DrawLine(wxPoint(x+hh-ww3+1, y), wxPoint(x+hh-ww3+1, y+hh));
	}
	x += ww+2;
	points.clear();

	//minus
	dc.SetPen(_gui_btn_under_mouse == 1 ? pen2 : pen);
	gui_btns.push_back(zGuiButton(x, x+ww, ID_MINUS));
	dc.DrawLine(wxPoint(x+1, y + hh2), wxPoint(x-1 + ww, y + hh2));
	dc.DrawLine(wxPoint(x+1, y + hh2 + 1), wxPoint(x-1 + ww, y + hh2 + 1));
	x += ww+2;

	//plus
	dc.SetPen(_gui_btn_under_mouse == 2 ? pen2 : pen);
	gui_btns.push_back(zGuiButton(x, x+ww, ID_PLUS));
	dc.DrawLine(wxPoint(x+ww2, y), wxPoint(x+ww2, y+hh));
	dc.DrawLine(wxPoint(x+ww2-1, y), wxPoint(x+ww2-1, y+hh));
	dc.DrawLine(wxPoint(x, y+hh2), wxPoint(x+ww, y+hh2));
	dc.DrawLine(wxPoint(x, y+hh2-1), wxPoint(x+ww, y+hh2-1));
	x += ww+2;

	//lock
	dc.SetPen(_gui_btn_under_mouse == 3 ? pen2 : pen);
	gui_btns.push_back(zGuiButton(x, x+ww, ID_PAUSE_ON_LOCK));
	int x1 = x + 1;
	dc.DrawLine(wxPoint(x1, y), wxPoint(x1, y+hh));
	dc.DrawLine(wxPoint(x1+1, y), wxPoint(x1+1, y+hh));
	dc.DrawLine(wxPoint(x1, y+hh), wxPoint(x1+ww-2, y+hh));
	dc.DrawLine(wxPoint(x1, y+hh-1), wxPoint(x1+ww-2, y+hh-1));
	x += ww+2;

	//hide
	dc.SetPen(_gui_btn_under_mouse == 4 ? pen2 : pen);
	gui_btns.push_back(zGuiButton(x, x+ww, ID_HIDE));
	dc.DrawLine(wxPoint(x, y), wxPoint(x+ww+1, y+hh+1));
	dc.DrawLine(wxPoint(x, y+1), wxPoint(x+ww+1, y+hh+2));
	dc.DrawLine(wxPoint(x, y+hh), wxPoint(x+ww+1, y-1));
	dc.DrawLine(wxPoint(x, y+hh+1), wxPoint(x+ww+1, y-1+1));
	x += ww+2;

	return x;
}


void zTestWxDialog::OnMenuRange(wxCommandEvent &event)
{
	int id = event.GetId();

	if (id >= ID_CONTEXT_MENU_MIN && id <= ID_CONTEXT_MENU_MAX)
	{
		int id2 = id - ID_CONTEXT_MENU_MIN;

		const std::vector<int>& add_times = zConfig::inst().get_add_times();
		if (id2 >= 0 && id2 < (int)add_times.size())
		{
			timer_serv.init_TimerMinutes(add_times[id2]);
		}
		else
		{
			//ztodo
			if (id2 == 111) timer_serv.inc_timer(0, 5, 0);
			if (id2 == 112) timer_serv.inc_timer(0, 10, 0);
			if (id2 == 113) timer_serv.inc_timer(0, 20, 0);
			if (id2 == 114) timer_serv.inc_timer(0, 30, 0);
			if (id2 == 115) timer_serv.inc_timer(0, 40, 0);
			if (id2 == 116) timer_serv.inc_timer(0, 50, 0);
			if (id2 == 117) timer_serv.inc_timer(0, 60, 0);

			_process_cmd(id);
		}
		Refresh(false);
	}
	else
	if (id >= ID_ADD_REMINDER_SUB && id <= ID_ADD_REMINDER_MAX)
	{
		int id2 = id - ID_ADD_REMINDER_SUB;
		int num = id2 / REMINDER_ACTIONS_CNT;
		switch (id2 % REMINDER_ACTIONS_CNT)
		{
		case REMINDER_ACTION_DELETE:
			timer_serv.delete_reminder(num);
		break;
		case REMINDER_ACTION_PAUSE:
			timer_serv.get_reminder(num).toggle_paused();
		break;
		case REMINDER_ACTION_RESET:
			timer_serv.get_reminder(num).reset_reminder_time(false);
		break;
		default:
			wxASSERT(0);
		}
	}
	else
		wxASSERT(0);
}


void zTestWxDialog::_process_cmd(int id)
{
	switch (id)
	{
		case ID_TOGGLE_PAUSE:
			timer_serv.toggle_pause();
		break;

		case ID_MINUS:
			timer_serv.dec_timer(0, 1, 0);
		break;

		case ID_PLUS:
			timer_serv.inc_timer(0, 1, 0);
		break;

		case ID_PAUSE_ON_LOCK:
			//todo
		break;

		case ID_HIDE:
			Show(false);
		break;

		case ID_IS_SHOW_CURTIME:
			zConfig::inst().set_is_show_curtime( zConfig::inst().get_is_show_curtime() == 1 ? 0 : 1 );
		break;


		case ID_EXIT:
			Close();
		break;

		case ID_TO_EXACT_TIME:
			choose_exact_time();
		break;

		case ID_ADD_REMINDER:
			add_reminder();
		break;
	}
}


void zTestWxDialog::end_window_drag()
{
	dragging_type = DT_NONE;
}


#ifdef __WINDOWS__
WXLRESULT zTestWxDialog::MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam)
{
	if (message ==  WM_WTSSESSION_CHANGE)
	{
		if (wParam == WTS_SESSION_LOCK)
			timer_serv.on_lock_screen();
		else
		if (wParam == WTS_SESSION_UNLOCK)
			timer_serv.on_unlock_screen();
	}

	return GUIDialog::MSWWindowProc(message, wParam, lParam);
}
#endif // __WINDOWS__


void zTestWxDialog::choose_exact_time()
{
	if (!m_pChooseTimeDlg)
		m_pChooseTimeDlg = new zChooseTimeDlg(this);

	m_pChooseTimeDlg->Init();
	m_pChooseTimeDlg->Show();

	_set_child_dlgs_pos();
}


void zTestWxDialog::add_reminder()
{
	if (timer_serv.get_reminders_cnt() >= MAX_REMINDERS_CNT)
	{
		wxMessageBox("Max reminders", _(""));
		return;
	}

	if (!m_pAddReminderDlg)
		m_pAddReminderDlg = new zAddReminderDlg(this, GetSize());

	m_pAddReminderDlg->Init();
	m_pAddReminderDlg->Show();

	_set_child_dlgs_pos();
}


void zTestWxDialog::show_reminder(int reminder_ind)
{
	if (!m_pReminderDlg)
		m_pReminderDlg = new zReminderDlg(this, timer_serv, GetSize());

	m_pReminderDlg->Init();
	m_pReminderDlg->set_reminder(reminder_ind);
	m_pReminderDlg->Show();

	_set_child_dlgs_pos();
}


int zTestWxDialog::_get_gui_btn(wxPoint pos)
{
	wxSize cur_size = GetClientSize();
	if (pos.y < 0 || pos.y >= cur_size.y)
		return -1;

	for(int i = 0; i < (int)gui_btns.size(); i++)
	{
		if (pos.x >= gui_btns[i].x && pos.x <= gui_btns[i].x2)
			return i;
	}

	return -1;
}


void zTestWxDialog::_set_capture_on_move(wxPoint cur)
{
	wxSize cur_size = GetClientSize();

	bool in = (cur.x >= 0) && (cur.y >= 0) && (cur.x <= cur_size.x) && (cur.y <= cur_size.y);

	if (in)
	{
		if (!HasCapture())
			CaptureMouse();
	}
	else
	{
		if (HasCapture())
			ReleaseMouse();
		gui_btn_under_mouse = -1;
	}
}


void zTestWxDialog::OnTimeChoose(wxDateTime _time)
{
	wxDateTime time_now = wxDateTime::Now();

	if (_time < time_now)
		_time += wxTimeSpan(24);

	wxTimeSpan dt = _time - time_now;

	timer_serv.set_TimeLeft(dt.GetMilliseconds());

	Refresh(false);
}


void zTestWxDialog::OnReminderAdd(wxLongLong msec, const wxString& text, bool exact_time)
{
	timer_serv.add_reminder(msec, text, exact_time);
}


void zTestWxDialog::_set_child_dlgs_pos()
{
	wxPoint cur_pos = GetPosition();
	cur_pos.y += GetClientSize().y;

	if (m_pChooseTimeDlg && m_pChooseTimeDlg->IsShown())
	{
		m_pChooseTimeDlg->SetPosition(cur_pos);
		cur_pos.y += m_pChooseTimeDlg->GetClientSize().y;
	}

	if (m_pAddReminderDlg && m_pAddReminderDlg->IsShown())
	{
		m_pAddReminderDlg->SetPosition(cur_pos);
		cur_pos.y += m_pAddReminderDlg->GetClientSize().y;
	}

	if (m_pReminderDlg && m_pReminderDlg->IsShown())
	{
		m_pReminderDlg->SetPosition(cur_pos);
		cur_pos.y += m_pReminderDlg->GetClientSize().y;
	}
}


void zTestWxDialog::check_reminders()
{
	int cnt = timer_serv.get_reminders_cnt();
	for (int i = 0; i < cnt; i++)
	{
		zReminder& r = timer_serv.get_reminder(i);
		if (r.need_to_show())
			show_reminder(i);
	}
}


bool zTestWxDialog::AcceptsFocus() const
{
    return false;
}


void zTestWxDialog::_check_on_top()
{
    //if (IsShown() && !IsShownOnScreen())
    //    Show(true);
}


void zTestWxDialog::OnFocus(wxFocusEvent& evt)
{
	wxWindow* prev = evt.GetWindow();
	if (prev)
		prev->SetFocus();
}


void zTestWxDialog::process_keys_workaround()
{
    if (m_prev_key != INT_MAX && wxGetKeyState((wxKeyCode)m_prev_key))
        return;

    m_prev_key = INT_MAX;

    int keys[] = {WXK_F2, WXK_ESCAPE, WXK_F1, WXK_F4, '1', '2', '3', '4', '0', WXK_DELETE, WXK_LEFT, WXK_UP, WXK_RIGHT, WXK_DOWN};
    for (int i = 0; i < (int)(sizeof(keys)/sizeof(keys[0])); i++)
    {
        int key = keys[i];
        if (wxGetKeyState((wxKeyCode)key))
        {
            m_prev_key = key;
            on_key_down(key);
            return;
        }
    }
}

void zTestWxDialog::on_key_down(int key)
{
	//bool bCtrl = wxGetKeyState(WXK_CONTROL);
	//bool bShift = wxGetKeyState(WXK_SHIFT);

	//wxSize cur_size = GetClientSize();
/*
    switch (key)
    {
        case '0':
		break;
		default:
#ifdef _DEBUG
			on_key_down_dbg(key);
#endif
	}
*/
}

void zTestWxDialog::on_key_down_dbg(int key)
{
/*
	bool bCtrl = wxGetKeyState(WXK_CONTROL);
	bool bShift = wxGetKeyState(WXK_SHIFT);

    switch (key)
    {
        case '1':
			//show_reminder(0);
		break;
	}
*/
}
