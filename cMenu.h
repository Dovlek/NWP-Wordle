#pragma once
#include "wx/wx.h"

class cMenu : public wxPanel
{
public:
    cMenu(wxWindow* parent);
    ~cMenu();

private:
    wxButton* continueButton;
    wxButton* newGameButton;
    wxButton* saveLoadButton;
    wxButton* optionsButton;
    wxButton* exitButton;
    
    void OnContinueClicked(wxCommandEvent& evt);
    void OnNewGameClicked(wxCommandEvent& evt);
    void OnSaveLoadClicked(wxCommandEvent& evt);
    void OnOptionsClicked(wxCommandEvent& evt);
    void OnExitClicked(wxCommandEvent& evt);
    
    enum
    {
        ID_CONTINUE,
        ID_NEW_GAME,
        ID_SAVE_LOAD,
        ID_OPTIONS,
        ID_EXIT
    };
};