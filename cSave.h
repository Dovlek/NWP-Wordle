#pragma once
#include "wx/wx.h"

wxDECLARE_EVENT(wxEVT_SWITCH_TO_MENU, wxCommandEvent);

class cSave : public wxPanel
{
public:
    cSave(wxWindow* parent);
    ~cSave();

private:
    wxTextCtrl* saveNameInput;
    wxListBox* saveFilesList;
    wxButton* saveButton;
    wxButton* deleteButton;
    wxButton* backButton;

    void OnSaveClicked(wxCommandEvent& evt);
    void OnDeleteClicked(wxCommandEvent& evt);
    void OnBackClicked(wxCommandEvent& evt);
    void OnSaveNameChanged(wxCommandEvent& evt);
    void OnSaveFileSelected(wxCommandEvent& evt);
    void OnButtonEnter(wxMouseEvent& evt);
    void OnButtonLeave(wxMouseEvent& evt);
    void OnButtonSetFocus(wxFocusEvent& evt);
    void OnButtonKillFocus(wxFocusEvent& evt);

    void RefreshSaveFilesList();
    void UpdateButtonStates();
    wxButton* GetButtonById(int id);

    enum
    {
        ID_SAVE_BUTTON = 2000,
        ID_DELETE_BUTTON,
        ID_BACK_BUTTON,
        ID_SAVE_NAME_INPUT,
        ID_SAVE_FILES_LIST
    };
};