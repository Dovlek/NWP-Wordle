#pragma once
#include "wx/wx.h"

wxDECLARE_EVENT(wxEVT_CONTINUE_GAME, wxCommandEvent);

class cWordle;

class cLoad : public wxPanel
{
public:
    cLoad(wxWindow* parent);
    ~cLoad();

    void RefreshSaveFilesList();

private:
    wxListBox* saveFilesList;
    wxButton* loadButton;
    wxButton* backButton;

    void OnLoadClicked(wxCommandEvent& evt);
    void OnBackClicked(wxCommandEvent& evt);
    void OnSaveFileSelected(wxCommandEvent& evt);
    void OnButtonEnter(wxMouseEvent& evt);
    void OnButtonLeave(wxMouseEvent& evt);
    void OnButtonSetFocus(wxFocusEvent& evt);
    void OnButtonKillFocus(wxFocusEvent& evt);

    void UpdateButtonStates();
    wxButton* GetButtonById(int id);

    wxString GetSavesDirectory();
    cWordle* GetWordlePanel();

    enum
    {
        ID_LOAD_BUTTON = 3000,
        ID_BACK_BUTTON_LOAD,
        ID_SAVE_FILES_LIST_LOAD
    };
};
