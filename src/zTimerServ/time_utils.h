#pragma once

#include <wx/longlong.h>

enum
{
	LL_EMPTY_VAL = -1
};


namespace tutl
{
	wxLongLong get_cur_time();							//msecs from 1970
	wxLongLong get_day_time(wxLongLong time);			//msecs from the beginning of cur day
	//wxLongLong get_prev_day_time(wxLongLong time);		//msecs from 1970 not including today

	wxString LL_to_string(wxLongLong val, bool no_msec = false);

	inline wxLongLong hms2LL(int hour, int min, int sec)
	{
		return 1000 * (sec + min * 60 + hour * 3600);
	}
};
