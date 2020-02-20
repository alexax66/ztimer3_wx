#include "pch.h"

#include "res_ids.h"

#include "zContextMenu.h"

#include "zConfig.h"
#include "zTimerServ/zTimerServ.h"


zContextMenu::zContextMenu() :
	m_reminders_sub_menu_item(0)
{
}


zContextMenu::~zContextMenu()
{
}


void zContextMenu::create(zTimerServ& timer_serv)
{
	if (GetMenuItemCount())
		return;

	int id = ID_CONTEXT_MENU_MIN;

	//
	const std::vector<int>& add_times = zConfig::inst().get_add_times();
	for(int i = 0; i < (int)add_times.size(); i++)
		this->Append(id + i, wxString::Format("&%d min", add_times[i]));

	//
    this->AppendSeparator();
	{
		wxMenu* subMenu = new wxMenu();
		subMenu->Append(id + 111, wxT("&5 min"));
		subMenu->Append(id + 112, wxT("&10 min"));
		subMenu->Append(id + 113, wxT("&20 min"));
		subMenu->Append(id + 114, wxT("&30 min"));
		subMenu->Append(id + 115, wxT("&40 min"));
		subMenu->Append(id + 116, wxT("&50 min"));
		subMenu->Append(id + 117, wxT("&1 hour"));
		this->AppendSubMenu(subMenu, "Add time");
	}

	//
    this->AppendSeparator();
	{
		wxMenu* subMenu = new wxMenu();
		m_reminders_sub_menu_item = this->AppendSubMenu(subMenu, "Reminders");

		fill_reminders_submenu(timer_serv);
	}

	//
    this->AppendSeparator();
	this->Append(ID_TO_EXACT_TIME, wxT("To exact time"));

	this->AppendSeparator();
	this->Append(ID_TOGGLE_PAUSE, wxT("Toggle Pause"));
	this->Append(ID_MINUS, wxT("Minus"));
	this->Append(ID_PLUS, wxT("Plus"));
	this->Append(ID_PAUSE_ON_LOCK, wxT("Pause on lock"));
	this->Append(ID_HIDE, wxT("Hide"));

	this->AppendSeparator();
	bool is_show_curtime = (zConfig::inst().get_is_show_curtime() == 1);
	this->Append(ID_IS_SHOW_CURTIME, wxT("Show current time"), wxEmptyString, wxITEM_CHECK);
	this->Check(ID_IS_SHOW_CURTIME, is_show_curtime);

	this->AppendSeparator();
	this->Append(0, "Sound");
	this->Append(0, "Window size");

	//
	this->AppendSeparator();
	this->Append(0, "Language");
	this->Append(0, "About");

	//
	this->AppendSeparator();
	this->Append(ID_EXIT, "Exit");
}


void zContextMenu::fill_reminders_submenu(zTimerServ& timer_serv)
{
	if (!m_reminders_sub_menu_item)
	{
		wxASSERT(0);
		return;
	}

	wxMenu* subMenu = m_reminders_sub_menu_item->GetSubMenu();
	if (!subMenu)
	{
		wxASSERT(0);
		return;
	}

	//delete all items
	while (subMenu->GetMenuItemCount())
	{
		wxMenuItem* item = subMenu->FindItemByPosition(0);
		if (item)
			subMenu->Destroy(item);	//Destroy also deletes submenu
	}

	//
	subMenu->Append(ID_ADD_REMINDER, "Add reminder");
	subMenu->AppendSeparator();

	int cnt = timer_serv.get_reminders_cnt();
	for (int i = 0; i < cnt; i++)
	{
		zReminder& r = timer_serv.get_reminder(i);

		wxMenu* reminder_Menu = new wxMenu();
		subMenu->AppendSubMenu(reminder_Menu, r.get_menu_text());

		reminder_Menu->Append(ID_ADD_REMINDER_SUB + REMINDER_ACTIONS_CNT*i + REMINDER_TITLE, r.get_time_text());
		reminder_Menu->AppendSeparator();
		reminder_Menu->Append(ID_ADD_REMINDER_SUB + REMINDER_ACTIONS_CNT*i + REMINDER_ACTION_DELETE, "Delete");
		reminder_Menu->Append(ID_ADD_REMINDER_SUB + REMINDER_ACTIONS_CNT*i + REMINDER_ACTION_PAUSE, "Pause");
		reminder_Menu->Append(ID_ADD_REMINDER_SUB + REMINDER_ACTIONS_CNT*i + REMINDER_ACTION_RESET, "Reset");
	}
}


void zContextMenu::refresh_reminders_submenu(zTimerServ& timer_serv)
{
	if (!m_reminders_sub_menu_item)
		return;

	wxMenu* subMenu = m_reminders_sub_menu_item->GetSubMenu();
	if (!subMenu)
	{
		wxASSERT(0);
		return;
	}

	int cnt = timer_serv.get_reminders_cnt();
	if (cnt != (int)subMenu->GetMenuItemCount() - 2)
	{
		fill_reminders_submenu(timer_serv);
	}

	if (cnt != (int)subMenu->GetMenuItemCount() - 2)
	{
		wxASSERT(0);
		return;
	}

	for (int i = 0; i < cnt; i++)
	{
		wxMenuItem* item = subMenu->FindItemByPosition(i+2);
		wxMenu* subMenu2 = item ? item->GetSubMenu() : 0;
		wxMenuItem* item2 = subMenu2 ? subMenu2->FindItemByPosition(0) : 0;
		if (!item2)
		{
			wxASSERT(0);
			continue;
		}

		zReminder& r = timer_serv.get_reminder(i);
		wxString s = r.get_time_text();

		wxString buf = item->GetItemLabelText();
		//int id = item->GetId();
		//subMenu->SetLabel(id, s);
		item2->SetItemLabel(s);
	}
}

