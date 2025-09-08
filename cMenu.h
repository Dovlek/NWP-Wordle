#pragma once
#include "wx/wx.h"

wxDECLARE_EVENT(wxEVT_START_NEW_GAME, wxCommandEvent);
wxDECLARE_EVENT(wxEVT_CONTINUE_GAME, wxCommandEvent);

class cMenu : public wxPanel
{
public:
    cMenu(wxWindow* parent);
    ~cMenu();
    
    void SetContinueButtonEnabled(bool enabled);

private:
    wxButton* continueButton;
    wxButton* newGameButton;
    wxButton* saveButton;
    wxButton* loadButton;
    wxButton* optionsButton;
    wxButton* exitButton;
    
    void OnContinueClicked(wxCommandEvent& evt);
    void OnNewGameClicked(wxCommandEvent& evt);
    void OnSaveClicked(wxCommandEvent& evt);
    void OnLoadClicked(wxCommandEvent& evt);
    void OnOptionsClicked(wxCommandEvent& evt);
    void OnExitClicked(wxCommandEvent& evt);
    void OnButtonEnter(wxMouseEvent& evt);
    void OnButtonLeave(wxMouseEvent& evt);
    void OnButtonSetFocus(wxFocusEvent& evt);
    void OnButtonKillFocus(wxFocusEvent& evt);
    wxButton* GetButtonById(int id);

    enum
    {
        ID_CONTINUE = 1000,
        ID_NEW_GAME,
        ID_SAVE,
        ID_LOAD,
        ID_OPTIONS,
        ID_EXIT
    };
};