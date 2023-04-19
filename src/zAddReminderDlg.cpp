#include "pch.h"

#include "zAddReminderDlg.h"
#include "iTimeChooser.h"
#include "zTimerServ/time_utils.h"


enum
{
    ID_BTN_OK = 1000,
    ID_BTN_CANCEL,
    ID_EDIT_TEXT,
    ID_CHECKBOX_EXACT_TIME,
};


//---------------------------------------------------------------------------------------
BEGIN_EVENT_TABLE(zAddReminderDlg, wxFrame)
    EVT_BUTTON(ID_BTN_OK, zAddReminderDlg::OnButtonClick)
    EVT_BUTTON(ID_BTN_CANCEL, zAddReminderDlg::OnButtonClick)
END_EVENT_TABLE()


//---------------------------------------------------------------------------------------
zAddReminderDlg::zAddReminderDlg(iTimeChooser* _chooser, wxSize parent_size) :
    chooser(_chooser)
{
    _create_Controls(parent_size);
}


//---------------------------------------------------------------------------------------
zAddReminderDlg::~zAddReminderDlg()
{
}


//---------------------------------------------------------------------------------------
void zAddReminderDlg::Init()
{
    wxDateTime now = wxDateTime::Now();

    now.SetHour(0);
    now.SetMinute(10);
    now.SetSecond(0);

    timePicker.SetValue(now);
}


//---------------------------------------------------------------------------------------
void zAddReminderDlg::_create_Controls(wxSize parent_size)
{
    int id = wxID_ANY;
    wxSize size = wxSize(parent_size.x, 80);//wxDefaultSize;

    //
    int style = wxSTAY_ON_TOP | wxNO_BORDER | wxFRAME_NO_TASKBAR;

    Create(0, id, wxEmptyString, wxDefaultPosition, size, style);

    //
    timePicker.Create(this, 1234);

    btnOK.Create(this, ID_BTN_OK, wxT("OK"));//, wxDefaultPosition, wxSize(80, 20));
    btnCancel.Create(this, ID_BTN_CANCEL, wxT("Cancel"));//, wxDefaultPosition, wxSize(80, 20));

    editText.Create(this, ID_EDIT_TEXT, wxEmptyString);//, wxDefaultPosition, wxSize(80, 20));
    exactTime.Create(this, ID_CHECKBOX_EXACT_TIME, wxT("Exact time"));//, wxDefaultPosition, wxSize(80, 20));

    timePicker.SetPosition(wxPoint(0, 0));
    btnOK.SetPosition(wxPoint(timePicker.GetSize().x, 0));
    btnCancel.SetPosition(wxPoint(btnOK.GetPosition().x + btnOK.GetSize().x, 0));

    editText.SetPosition(wxPoint(0, timePicker.GetSize().y + 2));
    exactTime.SetPosition(wxPoint(editText.GetPosition().x + editText.GetSize().x + 2, editText.GetPosition().y));

    //timePicker.Show();
    //btnOK.Show();
    //btnCancel.Show();
    //editText.Show();

    //SetSize(GetSize().x, editText.GetPosition().y + editText.GetSize().y + 5);
}


//---------------------------------------------------------------------------------------
void zAddReminderDlg::OnButtonClick(wxCommandEvent& evt)
{
    if (evt.GetId() == ID_BTN_OK)
    {
        if (chooser)
        {
            wxDateTime t = timePicker.GetValue();
            bool exact_time = exactTime.GetValue();
            chooser->OnReminderAdd(tutl::hms2LL(t.GetHour(), t.GetMinute(), t.GetSecond()), editText.GetValue(), exact_time);
        }
    }
    else
    if (evt.GetId() == ID_BTN_CANCEL)
    {
    }
    Hide();
}

