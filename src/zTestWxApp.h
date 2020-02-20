#ifndef ZTESTWXAPP_H
#define ZTESTWXAPP_H

#include <wx/app.h>
#include <memory>

class zTestWxDialog;
class wxFileConfig;

class zTestWxApp : public wxApp
{

public:
    zTestWxDialog *main_dlg;

    zTestWxApp();
    virtual ~zTestWxApp();

    virtual bool OnInit();
    virtual void OnEndSession(wxCloseEvent& evt);

    DECLARE_EVENT_TABLE();
private:
};

#endif // ZTESTWXAPP_H
