#include "pch.h"

#include "zConfig.h"

#include "zTimerServ/zTimerServ.h"


zConfig::zConfig() :
    m_pConfig(0),
    m_pGlobOptions(0)
{
}


zConfig::~zConfig()
{
    wxConfigBase::Set(0);
    m_pConfig = 0;

    delete m_pGlobOptions;
    m_pGlobOptions = 0;
}


zConfig& zConfig::inst()
{
    static zConfig t;
    return t;
}


void zConfig::Init()
{
    _create_options();
    _load_options();
}

static wxString _get_main_ini_path()
{
    wxString dir;

    wxFileName executablePath = wxFileName(wxStandardPaths::Get().GetExecutablePath());
    dir = executablePath.GetPath();

#ifdef _DEBUG
    if (!wxFileExists(dir + "/ztimer3.ini"))
        dir = wxGetCwd();
#endif


    wxString ret = dir + "/ztimer3.ini";
    //wxMessageBox(ret);
    return ret;
}


void zConfig::_create_options()
{
    wxString appName = wxEmptyString;
    wxString vendorName = wxEmptyString;
    wxString globalFilename = wxEmptyString;
    wxString localIniFullPath = _get_main_ini_path();
    long style = wxCONFIG_USE_LOCAL_FILE;

    m_pGlobOptions = new wxFileConfig(appName, vendorName, localIniFullPath, globalFilename, style);

    //
    bool local_options = false;
    bool bval = false;
    if (m_pGlobOptions->Read("main/local_options", &bval, false) && bval)
        local_options = true;

    //
    if (!local_options)
        m_pConfig = new wxFileConfig("zTimer", vendorName, "ztimer_settings.ini");
    else
    {
        wxString path = wxFileName(localIniFullPath).GetPath() + "/ztimer_settings.ini";
        m_pConfig = new wxFileConfig(appName, vendorName, path, globalFilename, style);
    }
    wxConfigBase::Set(m_pConfig);
}


void zConfig::_load_options()
{
    if (!m_pGlobOptions)
    {
        wxASSERT(0);
        return;
    }

    add_times.clear();
    int ival = 0;
    int cnt = m_pGlobOptions->Read("add_times/times_cnt", &ival, 0) ? ival : 0;
    if (cnt == 0)
    {
        //empty ztimer3.ini
        add_times.push_back(3);
        add_times.push_back(5);
        add_times.push_back(10);
        add_times.push_back(20);
        add_times.push_back(40);
        add_times.push_back(60);
        return;
    }

    for(int i = 0; i < cnt; i++)
    {
        int _ival = 0;

        wxString key = wxString::Format("time_%d", i);
        if (m_pGlobOptions->Read("add_times/" + key, &_ival, 0))
            add_times.push_back(_ival);
    }
}


wxPoint zConfig::get_dialog_pos()
{
    //trying to get position from config
    wxPoint pos = wxDefaultPosition;

    int x = get_int_val(wxT("window/pos_x"), -1);
    int y = get_int_val(wxT("window/pos_y"), -1);
    if (x != -1 && y != -1)
        pos = wxPoint(x, y);

    return pos;
}


void zConfig::set_dialog_pos(wxPoint point)
{
    set_int_val(wxT("window/pos_x"), point.x);
    set_int_val(wxT("window/pos_y"), point.y);
}


const std::vector<int>& zConfig::get_add_times()
{
    return add_times;
}


void zConfig::set_int_val(const wxString& key, int val)
{
    if (!m_pConfig)
    {
        wxASSERT(0);
        return;
    }
    m_pConfig->Write(key, val);
}


int zConfig::get_int_val(const wxString& key, int dflt_val)
{
    if (!m_pConfig)
    {
        wxASSERT(0);
        return dflt_val;
    }

    return m_pConfig->Read(key, dflt_val);
}


void zConfig::set_LL_val(const wxString& key, wxLongLong val)
{
    if (!m_pConfig)
    {
        wxASSERT(0);
        return;
    }
    m_pConfig->Write(key, val.ToLong());
}


wxLongLong zConfig::get_LL_val(const wxString& key, wxLongLong dflt_val)
{
    if (!m_pConfig)
    {
        wxASSERT(0);
        return dflt_val;
    }

    return m_pConfig->Read(key, dflt_val.ToLong());
}


void zConfig::set_string_val(const wxString& key, const wxString& val)
{
    if (!m_pConfig)
    {
        wxASSERT(0);
        return;
    }
    m_pConfig->Write(key, val);
}


wxString zConfig::get_string_val(const wxString& key, const wxString& dflt_val)
{
    if (!m_pConfig)
    {
        wxASSERT(0);
        return dflt_val;
    }

    return m_pConfig->Read(key, dflt_val);
}


int zConfig::get_is_show_curtime()
{
    return get_int_val(wxT("options/is_show_curtime"), 0);
}


void zConfig::set_is_show_curtime(int val)
{
    set_int_val(wxT("options/is_show_curtime"), val);
}


void zConfig::flush()
{
    if (m_pConfig)
        m_pConfig->Flush();
}


void zConfig::save_reminders(zTimerServ& timer_serv, bool on_del)
{
    int cnt = timer_serv.get_reminders_cnt();
    set_int_val(wxT("reminders/count"), cnt);

    for (int i = 0; i < cnt; i++)
    {
        zReminder& r = timer_serv.get_reminder(i);

        wxString t = r.get_text();
        wxLongLong p = r.get_period();
        bool e = r.get_exact_time();

        wxString key = wxString::Format("reminders/%d", i);

        set_LL_val(key + wxT("/period"), p);
        set_string_val(key + wxT("/text"), t);
        set_int_val(key + wxT("/exact"), e ? 1 : 0);
    }

    if (on_del)
    {
        wxString key = wxString::Format("reminders/%d", cnt);
        m_pConfig->DeleteGroup(key);
    }
}


void zConfig::load_reminders(zTimerServ& timer_serv)
{
    if (timer_serv.get_reminders_cnt() != 0)
    {
        wxASSERT(0);
        return;
    }
    //
    int cnt = get_int_val(wxT("reminders/count"), 0);

    for (int i = 0; i < cnt; i++)
    {
        wxString key = wxString::Format("reminders/%d", i);

        wxLongLong p = get_LL_val(key + wxT("/period"), 0);
        wxString t = get_string_val(key + wxT("/text"), "");
        bool e = (get_int_val(key + wxT("/exact"), 0) != 0);

        if (p > 0)
            timer_serv.add_reminder(p, t, e);
    }
}
