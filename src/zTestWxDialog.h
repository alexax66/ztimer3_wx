#pragma once

#include <wx/dcbuffer.h>

#include "zTestWxApp.h"

#include "GUIDialog.h"
#include "zTimerServ/zTimerServ.h"
#include "OSTimer.h"
#include "iTimeChooser.h"

#include "ztTaskBarIcon.h"
#include "zContextMenu.h"


class zChooseTimeDlg;
class zAddReminderDlg;
class zReminderDlg;

struct zGuiButton
{
	int x;
	int x2;
	int id;

	zGuiButton() : x(0), x2(0), id(-1) {}
	zGuiButton(int _x, int _x2, int _id) : x(_x), x2(_x2), id(_id) {}
};

class zTestWxDialog: public GUIDialog, public iTimerParent, public iTimeChooser
{
    enum DRAG_TYPE
    {
        DT_NONE,  //not drag
        DT_BEGIN, //begin drag
        DT_DRAG   //real drag
    };

	zTimerServ timer_serv;
	OSTimer timer;

	ztTaskBarIcon taskBarIcon;

	wxPoint lbdown_point_dd;
	wxPoint lbdown_point_screen;

	DRAG_TYPE dragging_type;

	std::vector<zGuiButton> gui_btns;
	int gui_btn_under_mouse;

	zChooseTimeDlg* m_pChooseTimeDlg;
	zAddReminderDlg* m_pAddReminderDlg;
	zReminderDlg* m_pReminderDlg;

	zContextMenu context_menu;
	int m_prev_key;

public:
	zTestWxDialog(wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style);
	~zTestWxDialog();

	virtual void OnTimer();

	void ShowTrayIcon(bool doShow = true);

	virtual bool AcceptsFocus() const;

private:
	virtual void OnClose(wxCloseEvent& evt);
	virtual void OnQuit(wxCommandEvent& evt);
	virtual void OnAbout(wxCommandEvent& evt);

	virtual void OnLeftDown(wxMouseEvent &evt);
	virtual void OnLeftUp(wxMouseEvent &evt);
	virtual void OnLeftDClick(wxMouseEvent &evt);
	virtual void OnRightDown(wxMouseEvent &evt);
	virtual void OnPaint(wxPaintEvent& evt);
	virtual void OnMouseMove(wxMouseEvent &evt);
	virtual void OnMouseCaptureLost(wxMouseCaptureLostEvent& evt);
	virtual void OnMove(wxMoveEvent &evt);
	virtual void OnFocus(wxFocusEvent& evt);

	virtual void OnMenuRange(wxCommandEvent &evt);

	virtual void OnTimeChoose(wxDateTime _time);
	virtual void OnReminderAdd(wxLongLong msec, const wxString& text, bool exact_time) override;

	int _draw_buttons(wxAutoBufferedPaintDC& dc, int x);

#ifdef __WINDOWS__
	WXLRESULT MSWWindowProc(WXUINT message, WXWPARAM wParam, WXLPARAM lParam);
#endif // __WINDOWS__
	void create_context_menu(wxPoint point);

	void end_window_drag();

	void choose_exact_time();
	void add_reminder();
	void show_reminder(int reminder_ind);

	void check_reminders();

	int _get_gui_btn(wxPoint pos);
	void _process_cmd(int id);
	void _set_capture_on_move(wxPoint cur);

	void _set_child_dlgs_pos();

	void _check_on_top();
	void process_keys_workaround();
	void on_key_down(int key);
	void on_key_down_dbg(int key);
};

