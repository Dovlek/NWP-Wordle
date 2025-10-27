#pragma once
#include "cLoad.h"
#include "cMenu.h"
#include "cOptions.h"
#include "cSave.h"
#include "cWordle.h"
#include "wx/wx.h"
#include <wx/simplebook.h>

wxDECLARE_EVENT(wxEVT_THEME_CHANGED, wxCommandEvent);

class cMain : public wxFrame
{
public:
    cMain();
    ~cMain();

    void SwitchPageToMenu();
    void SwitchPageToWordle();
    void SwitchPageToSave();
    void SwitchPageToLoad();
    void SwitchPageToOptions();

    bool IsGameActive();
    void EndCurrentGame();
    void RefreshAllPanelsTheme();

private:
    wxSimplebook* cSimplebook;
    cWordle* wordlePanel;
    cMenu* menuPanel;
    cSave* savePanel;
    cLoad* loadPanel;
    cOptions* optionsPanel;

    void OnSwitchToMenu(wxCommandEvent& evt);
    void OnStartNewGame(wxCommandEvent& evt);
    void OnContinueGame(wxCommandEvent& evt);
    void OnSwitchToSave(wxCommandEvent& evt);
    void OnSwitchToLoad(wxCommandEvent& evt);
    void OnSwitchToOptions(wxCommandEvent& evt);
    void OnThemeChanged(wxCommandEvent& evt);
    void UpdateMenuState();
};
