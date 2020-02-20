#ifndef __GUIDialog__
#define __GUIDialog__

// Define WX_GCH in order to support precompiled headers with GCC compiler.
// You have to create the header "wx_pch.h" and include all files needed
// for compile your gui inside it.
// Then, compile it and place the file "wx_pch.h.gch" into the same
// directory that "wx_pch.h".
#ifdef WX_GCH
#include <wx_pch.h>
#else
#include <wx/wx.h>
#endif

//#include <wx/button.h>
//#include <wx/statline.h>


//---------------------------------------------------------------------------------------
class GUIDialog : public wxFrame
{
    typedef wxFrame BaseClass;
    //typedef wxDialog BaseClass;

    DECLARE_EVENT_TABLE()
    private:

        // Private event handlers
        void _wxFB_OnClose( wxCloseEvent& event ){ OnClose( event ); }
        void _wxFB_OnAbout( wxCommandEvent& event ){ OnAbout( event ); }
        void _wxFB_OnQuit( wxCommandEvent& event ){ OnQuit( event ); }

    protected:
        virtual void OnClose( wxCloseEvent& event ){ event.Skip(); }
        virtual void OnAbout( wxCommandEvent& event ){ event.Skip(); }
        virtual void OnQuit( wxCommandEvent& event ){ event.Skip(); }
        virtual void OnLeftDown(wxMouseEvent &evt){ evt.Skip(); }
        virtual void OnLeftUp(wxMouseEvent &evt){ evt.Skip(); }
        virtual void OnLeftDClick(wxMouseEvent &evt){ evt.Skip(); }
        virtual void OnRightDown(wxMouseEvent &event){ event.Skip(); }
        virtual void OnPaint(wxPaintEvent& event){ event.Skip(); }
        virtual void OnMouseMove(wxMouseEvent &evt){ evt.Skip(); }
        virtual void OnMove(wxMoveEvent &evt){ evt.Skip(); }
        virtual void OnMouseCaptureLost(wxMouseCaptureLostEvent& evt){ evt.Skip(); }
        virtual void OnFocus(wxFocusEvent& evt){ evt.Skip(); }

        virtual void OnMenuRange(wxCommandEvent &event){ event.Skip(); }

    public:
        GUIDialog(wxWindow* parent, int id, wxString title, wxPoint pos, wxSize size, int style);
};

#endif //__GUIDialog__
