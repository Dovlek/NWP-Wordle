#pragma once
#include "wx/wx.h"
#include <wx/simplebook.h>
#include "cWordle.h"
#include "cMenu.h"
#include "cSave.h"

class cMain : public wxFrame
{
public:
    cMain();
    ~cMain();
    
    void SwitchPageToMenu();
    void SwitchPageToWordle();
    void SwitchPageToSave();

private:
    wxSimplebook* cSimplebook;
    cWordle* wordlePanel;
    cMenu* menuPanel;
    cSave* savePanel;
    
    void OnSwitchToMenu(wxCommandEvent& evt);
    void OnStartNewGame(wxCommandEvent& evt);
    void OnContinueGame(wxCommandEvent& evt);
    void OnSwitchToSave(wxCommandEvent& evt);
    void UpdateMenuState();
};
