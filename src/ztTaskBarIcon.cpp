#include "pch.h"

#include "ztTaskBarIcon.h"
#include "zTestWxApp.h"
#include "zTestWxDialog.h"


ztTaskBarIcon::ztTaskBarIcon()
{
}


BEGIN_EVENT_TABLE(ztTaskBarIcon, wxTaskBarIcon)
	EVT_MENU(wxID_ANY, ztTaskBarIcon::OnMenuItemClick)
	EVT_TASKBAR_LEFT_UP(ztTaskBarIcon::OnLeftUp)
END_EVENT_TABLE()


wxMenu* ztTaskBarIcon::CreatePopupMenu()
{
	wxDECLARE_APP(zTestWxApp);

	wxMenu* menu = new wxMenu();
	wxMenuItem* menuItem = NULL;

	int id = 1000;

	menuItem = new wxMenuItem(menu, id++, wxGetApp().main_dlg->IsVisible() ? _("Hide") : _("Show"));
#ifdef __WINDOWS__
	wxFont font(menuItem->GetFont());
	font.SetWeight(wxFONTWEIGHT_BOLD);
	menuItem->SetFont(font);
#endif
	menu->Append(menuItem);

	menu->AppendSeparator();

	menuItem = new wxMenuItem(menu, id++, _("Configuration"));
	menu->Append(menuItem);

	menu->AppendSeparator();

	menuItem = new wxMenuItem(menu, id++, _("Close"));
	menu->Append(menuItem);

	return menu;
}


wxMenuItem* GetMenuItemById(wxMenu* menu, int menuItemId)
{
	wxMenu* subMenu = NULL;
	wxMenuItem* menuItem = menu->FindItem(menuItemId, &subMenu);
	if (!menuItem) return NULL;

	wxMenuItem* output = dynamic_cast<wxMenuItem*>(menuItem);

	return output;
}

wxMenuItem* GetClickedMenuItem(wxCommandEvent& evt)
{
	wxMenu* menu = dynamic_cast<wxMenu*>(evt.GetEventObject());
	if (!menu) return NULL;

	return GetMenuItemById(menu, evt.GetId());
}


void ztTaskBarIcon::OnMenuItemClick(wxCommandEvent& evt)
{
	wxDECLARE_APP(zTestWxApp);

	//wxMenu* menu = dynamic_cast<wxMenu*>(evt.GetEventObject());
	wxMenuItem* menuItem = GetClickedMenuItem(evt);

	int id = menuItem->GetId();
	if (id == 1000)
	{
		wxGetApp().main_dlg->Show(!wxGetApp().main_dlg->IsVisible());

	}
	else
	if (id == 1002)
	{
		wxGetApp().main_dlg->Close();
	}
	else
	{
		evt.Skip();
	}
}


void ztTaskBarIcon::OnLeftUp(wxTaskBarIconEvent& evt)
{
	wxDECLARE_APP(zTestWxApp);

	if (!wxGetApp().main_dlg->IsVisible())
	{
		wxGetApp().main_dlg->Show();
		wxGetApp().main_dlg->Raise();
	}
	else
	{
		wxGetApp().main_dlg->Show(false);
	}
}
