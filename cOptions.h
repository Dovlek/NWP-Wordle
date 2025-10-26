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

    // Getters for settings
    bool IsTimedModeEnabled() const { return timedModeEnabled; }
    bool IsHardModeEnabled() const { return hardModeEnabled; }

private:
    wxToggleButton* timedModeToggle;
    wxToggleButton* hardModeToggle;
    wxButton* applyButton;
    wxButton* backButton;

    bool timedModeEnabled;
    bool hardModeEnabled;

    wxFileConfig* config;

    void OnTimedModeToggled(wxCommandEvent& evt);
    void OnHardModeToggled(wxCommandEvent& evt);
    void OnSaveClicked(wxCommandEvent& evt);
    void OnBackClicked(wxCommandEvent& evt);
    void OnButtonEnter(wxMouseEvent& evt);
    void OnButtonLeave(wxMouseEvent& evt);
    void OnButtonSetFocus(wxFocusEvent& evt);
    void OnButtonKillFocus(wxFocusEvent& evt);
    void OnAcceleratorPressed(wxCommandEvent& evt);

    void SaveSettings();
    void UpdateToggleButtonLabel(wxToggleButton* btn, const wxString& baseText);
    bool CheckAndConfirmGameEnd();
    wxButton* GetButtonById(int id);

    enum
    {
        ID_TIMED_MODE_CHECKBOX = 3000,
        ID_HARD_MODE_CHECKBOX,
        ID_SAVE_BUTTON,
        ID_BACK_BUTTON,
        ID_ACCEL_ESCAPE
    };
};
