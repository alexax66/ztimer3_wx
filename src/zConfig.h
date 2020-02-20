#pragma once

#include <wx/fileconf.h>
#include <wx/gdicmn.h>

#include <vector>

class zTimerServ;

class zConfig
{
    wxFileConfig *m_pConfig;
    wxFileConfig *m_pGlobOptions;

    std::vector<int> add_times;

public:
    zConfig();
    virtual ~zConfig();

    void Init();

    static zConfig& inst();

    wxPoint get_dialog_pos();
    void set_dialog_pos(wxPoint point);

    const std::vector<int>& get_add_times();

    int get_is_show_curtime();
    void set_is_show_curtime(int val);

    void set_int_val(const wxString& key, int val);
    int get_int_val(const wxString& key, int dflt_val);

    void set_LL_val(const wxString& key, wxLongLong val);
    wxLongLong get_LL_val(const wxString& key, wxLongLong dflt_val);

    void set_string_val(const wxString& key, const wxString& val);
    wxString get_string_val(const wxString& key, const wxString& dflt_val);

    void save_reminders(zTimerServ& timer_serv, bool on_del);
    void load_reminders(zTimerServ& timer_serv);

    void flush();

private:
    void _create_options();
    void _load_options();
};
