#pragma once
#include "cLoad.h"
#include "cMenu.h"
#include "cSave.h"
#include "cWordle.h"
#include "wx/wx.h"
#include <wx/simplebook.h>

class cMain : public wxFrame
{
public:
    cMain();
    ~cMain();

    void SwitchPageToMenu();
    void SwitchPageToWordle();
    void SwitchPageToSave();
    void SwitchPageToLoad();

private:
    wxSimplebook* cSimplebook;
    cWordle* wordlePanel;
    cMenu* menuPanel;
    cSave* savePanel;
    cLoad* loadPanel;

    void OnSwitchToMenu(wxCommandEvent& evt);
    void OnStartNewGame(wxCommandEvent& evt);
    void OnContinueGame(wxCommandEvent& evt);
    void OnSwitchToSave(wxCommandEvent& evt);
    void OnSwitchToLoad(wxCommandEvent& evt);
    void UpdateMenuState();
};
