#pragma once
#include "wx/wx.h"
#include <wx/simplebook.h>
#include "cWordle.h"
#include "cMenu.h"

class cMain : public wxFrame
{
public:
    cMain();
    ~cMain();
    
    void SwitchPageToMenu();
    void SwitchPageToWordle();

private:
    wxSimplebook* cSimplebook;
    cWordle* wordlePanel;
    cMenu* menuPanel;
    
    void OnSwitchToMenu(wxCommandEvent& evt);
    void OnSwitchToGame(wxCommandEvent& evt);
    void OnStartNewGame(wxCommandEvent& evt);
};
