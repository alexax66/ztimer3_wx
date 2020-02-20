#include "pch.h"

#include "zTestWxApp.h"
#include "zTestWxDialog.h"
#include "zConfig.h"


IMPLEMENT_APP(zTestWxApp);

BEGIN_EVENT_TABLE(zTestWxApp, wxApp)
  EVT_END_SESSION( zTestWxApp::OnEndSession )
END_EVENT_TABLE()


zTestWxApp::zTestWxApp() :
	main_dlg(0)
{
}


zTestWxApp::~zTestWxApp()
{
}


bool zTestWxApp::OnInit()
{
    //SetVendorName(wxT("zzz"));
    SetAppName(wxT("ztimer3_wx beta"));

	zConfig::inst().Init();

	//
	wxWindow* parent = 0;
	int id = wxID_ANY;
	wxString title = wxT("zTimer3_wx 0.10");

	//
	wxSize size = wxSize(250, 19);//wxDefaultSize;
	int style = /*wxSTAY_ON_TOP | */wxNO_BORDER | wxFRAME_NO_TASKBAR;//wxDEFAULT_DIALOG_STYLE );

#ifdef __WINDOWS__
    style |= wxFRAME_NO_TASKBAR;
#endif // __WINDOWS__

	wxPoint pos = zConfig::inst().get_dialog_pos();
	main_dlg = new zTestWxDialog(parent, id, title,pos , size, style);
	main_dlg->Show();

	return true;
}

void zTestWxApp::OnEndSession(wxCloseEvent& evt)
{
	//wxLogDebug( wxT("zTestWxApp::OnEndSession") );
	evt.Skip();
}

