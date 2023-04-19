#include "pch.h"

#include <wx/apptrait.h>

#include "zTestWxApp.h"
#include "zTestWxDialog.h"
#include "zConfig.h"


IMPLEMENT_APP(zTestWxApp);

BEGIN_EVENT_TABLE(zTestWxApp, wxApp)
  EVT_END_SESSION( zTestWxApp::OnEndSession )
END_EVENT_TABLE()


// static void TempAssertHandler(const wxString& file,
// 		int line, const wxString& func,
// 		const wxString& cond, const wxString& msg)
// {
// 	// fprintf(stderr, "TempAssertHandler: file='%ls' line=%d func='%ls' cond='%ls' msg='%ls'\n",
// 	// 		file.wc_str(), line, func.wc_str(), cond.wc_str(), msg.wc_str());
// }


zTestWxApp::zTestWxApp() :
    main_dlg(0)
{
    // wxSetAssertHandler(TempAssertHandler);
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

#ifdef __WINDOWS__
    wxSize size = wxSize(250, 19);
    int style = wxNO_BORDER | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP;
#elif __LINUX__

    wxSize size = wxSize(250, 19);
    //wxSize size = wxDefaultSize;

    wxString desktop;
    if (wxGetEnv("XDG_CURRENT_DESKTOP", &desktop))
    {
        //problem with window height on xfce
        if (desktop.Upper().Find("XFCE") != wxNOT_FOUND)
            size = wxSize(250, 52);
    }

    int style = wxNO_BORDER | wxSTAY_ON_TOP;//wxDEFAULT_DIALOG_STYLE );
#endif

    if (zConfig::inst().get_int_val("window/style_wxFRAME_NO_TASKBAR", 0))
        style |= wxFRAME_NO_TASKBAR;
    if (zConfig::inst().get_int_val("window/style_wxSTAY_ON_TOP", 0))
        style |= wxSTAY_ON_TOP;

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

