#pragma once
#include "wx/wx.h"
#include <wx/tglbtn.h>
#include <wx/fileconf.h>

class cOptions : public wxPanel
{
public:
    cOptions(wxWindow* parent);
    ~cOptions();

    void LoadSettings();
    void RefreshTheme();

    // Getters for settings
    bool IsTimedModeEnabled() const { return timedModeEnabled; }
    bool IsHardModeEnabled() const { return hardModeEnabled; }

private:
    wxToggleButton* timedModeToggle;
    wxToggleButton* hardModeToggle;
    wxButton* darkThemeButton;
    wxButton* lightThemeButton;
    wxButton* backButton;

    bool timedModeEnabled;
    bool hardModeEnabled;
    bool prevTimedModeEnabled;
    bool prevHardModeEnabled;

    wxFileConfig* config;

    void OnTimedModeToggled(wxCommandEvent& evt);
    void OnHardModeToggled(wxCommandEvent& evt);
    void OnDarkThemeClicked(wxCommandEvent& evt);
    void OnLightThemeClicked(wxCommandEvent& evt);
    void OnBackClicked(wxCommandEvent& evt);
    void OnButtonEnter(wxMouseEvent& evt);
    void OnButtonLeave(wxMouseEvent& evt);
    void OnButtonSetFocus(wxFocusEvent& evt);
    void OnButtonKillFocus(wxFocusEvent& evt);
    void OnAcceleratorPressed(wxCommandEvent& evt);

    void UpdateToggleButtonLabel(wxToggleButton* btn, const wxString& baseText);
    void UpdateThemeButtonStates();
    void SaveTimedModeSetting();
    void SaveHardModeSetting();
    bool CheckAndConfirmGameEnd();
    wxButton* GetButtonById(int id);

    enum
    {
        ID_TIMED_MODE_CHECKBOX = 3000,
        ID_HARD_MODE_CHECKBOX,
        ID_DARK_THEME_BUTTON,
        ID_LIGHT_THEME_BUTTON,
        ID_BACK_BUTTON,
        ID_ACCEL_ESCAPE
    };
};
