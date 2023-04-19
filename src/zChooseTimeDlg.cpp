#include "pch.h"

#include "zChooseTimeDlg.h"
#include "iTimeChooser.h"
#include "zTimerServ/time_utils.h"


enum
{
    ID_BTN_OK = 1000,
    ID_BTN_CANCEL
};


//---------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(zChooseTimeDlg, wxDialog)
    EVT_BUTTON(ID_BTN_OK, zChooseTimeDlg::OnButtonClick)
    EVT_BUTTON(ID_BTN_CANCEL, zChooseTimeDlg::OnButtonClick)
END_EVENT_TABLE()


//---------------------------------------------------------------------------------------
zChooseTimeDlg::zChooseTimeDlg(iTimeChooser* _chooser) :
    chooser(_chooser)
{
    _create_Controls();
}


//---------------------------------------------------------------------------------------
zChooseTimeDlg::~zChooseTimeDlg()
{
}


//---------------------------------------------------------------------------------------
void zChooseTimeDlg::Init()
{
    wxDateTime now = wxDateTime::Now();
    timePicker.SetValue(now);
}


//---------------------------------------------------------------------------------------
void zChooseTimeDlg::_create_Controls()
{
    int id = wxID_ANY;
    //wxSize size = wxSize(250, 119);
    wxSize size = wxDefaultSize;
    int style = /*wxSTAY_ON_TOP | */wxNO_BORDER | wxFRAME_NO_TASKBAR;

    Create(0, id, wxEmptyString, wxDefaultPosition, size, style);

    //
    timePicker.Create(this, 1234);

    btnOK.Create(this, ID_BTN_OK, wxT("OK"));//, wxDefaultPosition, wxSize(80, 20));
    btnCancel.Create(this, ID_BTN_CANCEL, wxT("Cancel"));//, wxDefaultPosition, wxSize(80, 20));

    timePicker.SetPosition(wxPoint(0, 0));
    btnOK.SetPosition(wxPoint(timePicker.GetSize().x, 0));
    btnCancel.SetPosition(wxPoint(btnOK.GetPosition().x + btnOK.GetSize().x, 0));

    timePicker.Show();
    btnOK.Show();

    SetSize(GetSize().x, btnOK.GetSize().y);
}


//---------------------------------------------------------------------------------------
void zChooseTimeDlg::OnButtonClick(wxCommandEvent& evt)
{
    if (evt.GetId() == ID_BTN_OK)
    {
        if (chooser)
            chooser->OnTimeChoose(timePicker.GetValue());
    }
    else
    if (evt.GetId() == ID_BTN_CANCEL)
    {
    }
    Close();
}

