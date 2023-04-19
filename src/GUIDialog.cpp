#include "pch.h"

#include "res_ids.h"

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "GUIDialog.h"

#ifdef __WINDOWS__
    #include "Wtsapi32.h"
    #pragma comment (lib, "Wtsapi32.lib")
#endif // __WINDOWS__

//---------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE( GUIDialog, GUIDialog::BaseClass )
    EVT_CLOSE( GUIDialog::_wxFB_OnClose )
    EVT_LEFT_DOWN(GUIDialog::OnLeftDown)
    EVT_LEFT_UP(GUIDialog::OnLeftUp)
    EVT_PAINT(GUIDialog::OnPaint)
    EVT_LEFT_DCLICK(GUIDialog::OnLeftDClick)
    EVT_RIGHT_DOWN(GUIDialog::OnRightDown)
    EVT_MENU_RANGE(ID_CONTEXT_MENU_MIN, ID_CONTEXT_MENU_MAX, GUIDialog::OnMenuRange)
    EVT_MENU_RANGE(ID_ADD_REMINDER, ID_ADD_REMINDER_MAX, GUIDialog::OnMenuRange)
    EVT_MOTION (GUIDialog::OnMouseMove)
    EVT_MOUSE_CAPTURE_LOST(GUIDialog::OnMouseCaptureLost)
    EVT_MOVE(GUIDialog::OnMove)
    EVT_SET_FOCUS(GUIDialog::OnFocus)
END_EVENT_TABLE()


//---------------------------------------------------------------------------------------
GUIDialog::GUIDialog( wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style ) :
    BaseClass( parent, id, title, pos, size, style )
{
    this->SetSizeHints( wxDefaultSize, wxDefaultSize );

#ifdef __WINDOWS__
    HWND h = GetHandle();
    WTSRegisterSessionNotification(h, NOTIFY_FOR_THIS_SESSION);
#endif // __WINDOWS__
}
