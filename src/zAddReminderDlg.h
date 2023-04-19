#pragma once

#include <wx/frame.h>
#include <wx/timectrl.h>
#include <wx/button.h>
#include <wx/textctrl.h>
#include <wx/checkbox.h>

class iTimeChooser;

class zAddReminderDlg: public wxFrame
{
    iTimeChooser* chooser;
    DECLARE_EVENT_TABLE()

    wxTimePickerCtrl timePicker;
    wxButton btnOK;
    wxButton btnCancel;
    wxTextCtrl editText;
    wxCheckBox exactTime;

public:
    zAddReminderDlg(iTimeChooser* _chooser, wxSize parent_size);
    virtual ~zAddReminderDlg();

    void Init();

protected:
    void OnButtonClick(wxCommandEvent& evt);
    void _create_Controls(wxSize parent_size);
};

