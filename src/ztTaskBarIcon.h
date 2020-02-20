#pragma once

#include <wx/taskbar.h>

class ztTaskBarIcon : public wxTaskBarIcon
{
public:
	ztTaskBarIcon();
	wxMenu* CreatePopupMenu();

	void OnLeftUp(wxTaskBarIconEvent& evt);
	void OnMenuItemClick(wxCommandEvent& evt);

	DECLARE_EVENT_TABLE()
};
