
enum
{
	MAX_REMINDERS_CNT = 100,
};

enum
{
	REMINDER_ACTION_DELETE = 0,
	REMINDER_ACTION_PAUSE = 1,
	REMINDER_ACTION_RESET = 2,
	REMINDER_TITLE = 3,
	REMINDER_ACTIONS_CNT = 4,
};

enum
{
	ID_CONTEXT_MENU_MIN = 1500,
	ID_CONTEXT_MENU_MAX = ID_CONTEXT_MENU_MIN + 1000,

	ID_SKIP_MENU_MIN = ID_CONTEXT_MENU_MAX + 1,
	ID_SKIP_MENU_MAX = ID_SKIP_MENU_MIN + 1000,

	ID_ADD_REMINDER_SUB = ID_SKIP_MENU_MAX+1,
	ID_ADD_REMINDER_MAX = ID_ADD_REMINDER_SUB + REMINDER_ACTIONS_CNT*MAX_REMINDERS_CNT,
};


//ID_CONTEXT_MENU_MIN .. ID_CONTEXT_MENU_MAX
enum
{
	ID_TOGGLE_PAUSE = ID_CONTEXT_MENU_MIN + 500,
	ID_MINUS,
	ID_PLUS,
	ID_PAUSE_ON_LOCK,
	ID_HIDE,
	ID_IS_SHOW_CURTIME,
	ID_TO_EXACT_TIME,
	ID_ADD_REMINDER,
	//
	ID_EXIT,
};
