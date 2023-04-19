#pragma once

#include <wx/dialog.h>
#include <wx/timectrl.h>
#include <wx/button.h>

class iTimeChooser;

class zChooseTimeDlg: public wxDialog
{
    iTimeChooser* chooser;
    DECLARE_EVENT_TABLE()

    wxTimePickerCtrl timePicker;
    wxButton btnOK;
    wxButton btnCancel;

public:
    zChooseTimeDlg(iTimeChooser* _chooser);
    virtual ~zChooseTimeDlg();

    void Init();

protected:
    void OnButtonClick(wxCommandEvent& evt);
    void _create_Controls();
};

