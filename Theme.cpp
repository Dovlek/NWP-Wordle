#include "Theme.h"
#include <wx/fileconf.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>

ThemeManager::ThemeManager()
{
    // Load theme preference from config
    wxString configPath = wxStandardPaths::Get().GetUserDataDir();
    wxString configFile = configPath + wxFileName::GetPathSeparator() + "wordle_config.ini";
    wxFileConfig config(wxEmptyString, wxEmptyString, configFile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

    bool lightTheme = config.ReadBool("/Settings/LightTheme", false);
    currentTheme = lightTheme ? ThemeType::LIGHT : ThemeType::DARK;
}

ThemeManager& ThemeManager::Get()
{
    static ThemeManager instance;
    return instance;
}

void ThemeManager::SetTheme(ThemeType theme)
{
    currentTheme = theme;

    // Save to config
    wxString configPath = wxStandardPaths::Get().GetUserDataDir();
    wxString configFile = configPath + wxFileName::GetPathSeparator() + "wordle_config.ini";
    wxFileConfig config(wxEmptyString, wxEmptyString, configFile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

    config.Write("/Settings/LightTheme", theme == ThemeType::LIGHT);
    config.Flush();
}

wxColor ThemeManager::GetBackgroundColor() const
{
    return currentTheme == ThemeType::DARK ? wxColor(20, 20, 20) : wxColor(255, 255, 255);
}

wxColor ThemeManager::GetTextColor() const
{
    return currentTheme == ThemeType::DARK ? wxColor(*wxWHITE) : wxColor(*wxBLACK);
}

wxColor ThemeManager::GetButtonColor() const
{
    return currentTheme == ThemeType::DARK ? wxColor(58, 58, 60) : wxColor(230, 230, 230);
}

wxColor ThemeManager::GetButtonHoverColor() const
{
    return currentTheme == ThemeType::DARK ? wxColor(129, 131, 132) : wxColor(200, 200, 200);
}

wxColor ThemeManager::GetButtonFocusColor() const
{
    return currentTheme == ThemeType::DARK ? wxColor(86, 87, 88) : wxColor(180, 180, 180);
}

wxColor ThemeManager::GetButtonDisabledColor() const
{
    return currentTheme == ThemeType::DARK ? wxColor(40, 40, 40) : wxColor(210, 210, 210);
}

wxColor ThemeManager::GetInputColor() const
{
    return currentTheme == ThemeType::DARK ? wxColor(58, 58, 60) : wxColor(240, 240, 240);
}

wxColor ThemeManager::GetToggleCheckedColor() const
{
    // Green stays the same for both themes
    return wxColor(83, 141, 78);
}

wxColor ThemeManager::GetHelpTextColor() const
{
    return currentTheme == ThemeType::DARK ? wxColor(150, 150, 150) : wxColor(100, 100, 100);
}
