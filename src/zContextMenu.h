#pragma once

#include <wx/menu.h>

class zTimerServ;

class zContextMenu : public wxMenu
{
	wxMenuItem* m_reminders_sub_menu_item;

public:
	zContextMenu();
	virtual ~zContextMenu();

	void create(zTimerServ& timer_serv);

	void fill_reminders_submenu(zTimerServ& timer_serv);
	void refresh_reminders_submenu(zTimerServ& timer_serv);
};
