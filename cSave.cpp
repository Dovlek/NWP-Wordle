#include "cSave.h"
#include "cWordle.h"
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/tokenzr.h>
#include <wx/simplebook.h>

cSave::cSave(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
    SetBackgroundColour(wxColor(20, 20, 20));

    // Title
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "Save Game", wxDefaultPosition, wxDefaultSize);
    title->SetBackgroundColour(wxColor(20, 20, 20));
    title->SetForegroundColour(wxColor(*wxWHITE));
    title->SetFont(wxFont(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

    // Save name input
    wxStaticText* nameLabel = new wxStaticText(this, wxID_ANY, "Save Name:", wxDefaultPosition, wxDefaultSize);
    nameLabel->SetBackgroundColour(wxColor(20, 20, 20));
    nameLabel->SetForegroundColour(wxColor(*wxWHITE));
    nameLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));

    saveNameInput = new wxTextCtrl(this, ID_SAVE_NAME_INPUT, "", wxDefaultPosition, wxSize(400, 25), wxTE_PROCESS_ENTER);
    saveNameInput->SetBackgroundColour(wxColor(58, 58, 60));
    saveNameInput->SetForegroundColour(wxColor(*wxWHITE));
    saveNameInput->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));

    // Existing saves list
    wxStaticText* listLabel = new wxStaticText(this, wxID_ANY, "Existing Saves:", wxDefaultPosition, wxDefaultSize);
    listLabel->SetBackgroundColour(wxColor(20, 20, 20));
    listLabel->SetForegroundColour(wxColor(*wxWHITE));
    listLabel->SetFont(wxFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));

    saveFilesList = new wxListBox(this, ID_SAVE_FILES_LIST, wxDefaultPosition, wxSize(400, 200));
    saveFilesList->SetBackgroundColour(wxColor(58, 58, 60));
    saveFilesList->SetForegroundColour(wxColor(*wxWHITE));
    saveFilesList->SetFont(wxFont(12, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));

    // Buttons
    saveButton = new wxButton(this, ID_SAVE_BUTTON, "Save", wxDefaultPosition, wxSize(120, 40), wxBORDER_NONE);
    deleteButton = new wxButton(this, ID_DELETE_BUTTON, "Delete", wxDefaultPosition, wxSize(120, 40), wxBORDER_NONE);
    backButton = new wxButton(this, ID_BACK_BUTTON, "Back", wxDefaultPosition, wxSize(120, 40), wxBORDER_NONE);

    // Style buttons
    wxFont buttonFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    for (int buttonId = ID_SAVE_BUTTON; buttonId <= ID_BACK_BUTTON; buttonId++)
    {
        wxButton* button = GetButtonById(buttonId);
        if (button)
        {
            button->SetBackgroundColour(wxColor(58, 58, 60));
            button->SetForegroundColour(wxColor(*wxWHITE));
            button->SetCursor(wxCursor(wxCURSOR_HAND));
            button->SetFont(buttonFont);
        }
    }

    // Bind events
    saveNameInput->Bind(wxEVT_TEXT, &cSave::OnSaveNameChanged, this);
    saveFilesList->Bind(wxEVT_LISTBOX, &cSave::OnSaveFileSelected, this);
    saveButton->Bind(wxEVT_BUTTON, &cSave::OnSaveClicked, this);
    deleteButton->Bind(wxEVT_BUTTON, &cSave::OnDeleteClicked, this);
    backButton->Bind(wxEVT_BUTTON, &cSave::OnBackClicked, this);
    saveNameInput->Bind(wxEVT_TEXT_ENTER, &cSave::OnSaveNameEnter, this);

    // Bind mouse events for buttons
    for (int buttonId = ID_SAVE_BUTTON; buttonId <= ID_BACK_BUTTON; buttonId++)
    {
        wxButton* button = GetButtonById(buttonId);
        if (button)
        {
            button->Bind(wxEVT_ENTER_WINDOW, &cSave::OnButtonEnter, this);
            button->Bind(wxEVT_LEAVE_WINDOW, &cSave::OnButtonLeave, this);
        }
    }

    // Bind focus events
    for (int buttonId = ID_SAVE_BUTTON; buttonId <= ID_BACK_BUTTON; buttonId++)
    {
        wxButton* button = GetButtonById(buttonId);
        if (button)
        {
            button->Bind(wxEVT_SET_FOCUS, &cSave::OnButtonSetFocus, this);
            button->Bind(wxEVT_KILL_FOCUS, &cSave::OnButtonKillFocus, this);
        }
    }

    // Set up layout
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* inputSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    mainSizer->AddStretchSpacer();
    mainSizer->Add(title, wxSizerFlags().CenterHorizontal().Border(wxALL, 20));

    inputSizer->Add(nameLabel, wxSizerFlags().Left().Border(wxLEFT | wxRIGHT | wxTOP, 10));
    inputSizer->Add(saveNameInput, wxSizerFlags().Expand().Border(wxALL, 10));
    inputSizer->Add(listLabel, wxSizerFlags().Left().Border(wxLEFT | wxRIGHT | wxTOP, 10));
    inputSizer->Add(saveFilesList, wxSizerFlags().Expand().Border(wxALL, 10));

    buttonSizer->Add(saveButton, wxSizerFlags().Border(wxALL, 5));
    buttonSizer->Add(deleteButton, wxSizerFlags().Border(wxALL, 5));
    buttonSizer->AddStretchSpacer();
    buttonSizer->Add(backButton, wxSizerFlags().Border(wxALL, 5));

    mainSizer->Add(inputSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, 20));
    mainSizer->Add(buttonSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, 10));
    mainSizer->AddStretchSpacer();

    SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);

    RefreshSaveFilesList();
    UpdateButtonStates();
}

cSave::~cSave()
{
}

wxString cSave::GetSavesDirectory()
{
    wxStandardPaths& stdPaths = wxStandardPaths::Get();
    wxString appDataDir = stdPaths.GetUserDataDir();

    wxString savesDir = appDataDir + wxFileName::GetPathSeparator() + "Saves";

    // Create saves directory if it doesn't exist
    if (!wxDir::Exists(savesDir))
        if (!wxFileName::Mkdir(savesDir, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL))
            wxLogError("Failed to create saves directory: %s", savesDir);

    return savesDir;
}

cWordle* cSave::GetWordlePanel()
{
    wxSimplebook* simplebook = dynamic_cast<wxSimplebook*>(wxWindow::GetParent());
    if (simplebook && simplebook->GetPageCount() >= 1)
        return dynamic_cast<cWordle*>(simplebook->GetPage(1));
    
    return nullptr;
}

void cSave::OnSaveClicked(wxCommandEvent& evt)
{
    wxString saveName = saveNameInput->GetValue().Trim();
    if (saveName.IsEmpty())
    {
        wxMessageBox("Please enter a save name.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Get current game state from Wordle panel
    cWordle* wordlePanel = GetWordlePanel();
    if (!wordlePanel)
    {
        wxMessageBox("Cannot access game data.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    if (!wordlePanel->IsGameInProgress())
    {
        wxMessageBox("No game in progress to save.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Create save file path
    wxString savesDir = GetSavesDirectory();
    wxString fileName = saveName + ".sav";
    wxString fullPath = savesDir + wxFileName::GetPathSeparator() + fileName;

    // Check if file already exists
    if (wxFile::Exists(fullPath))
    {
        int result = wxMessageBox("A save file with this name already exists. Overwrite?",
            "File Exists", wxYES_NO | wxICON_QUESTION);
        if (result != wxYES)
            return;
    }

    // Get game state data
    wxString gameStateData = wordlePanel->GetGameStateData();

    if (gameStateData.IsEmpty())
    {
        wxMessageBox("No game data to save.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Create save file
    wxFile saveFile;
    if (!saveFile.Create(fullPath, true))
    {
        wxMessageBox("Failed to create save file.\nPath: " + fullPath, "Error", wxOK | wxICON_ERROR);
        return;
    }

    // Write the data
    if (!saveFile.Write(gameStateData))
    {
        wxMessageBox("Failed to write game data to file.", "Error", wxOK | wxICON_ERROR);
        saveFile.Close();
        return;
    }

    saveFile.Close();

    // Verify the file was created
    if (wxFile::Exists(fullPath))
    {
        wxMessageBox("Game saved as: " + saveName, "Save Successful", wxOK | wxICON_INFORMATION);
        RefreshSaveFilesList();
        saveNameInput->Clear();
        UpdateButtonStates();
    }
    else
        wxMessageBox("Save file was not created successfully.", "Error", wxOK | wxICON_ERROR);
}

void cSave::OnSaveNameEnter(wxCommandEvent& evt)
{
    if (saveButton->IsEnabled())
    {
        wxCommandEvent saveEvent(wxEVT_BUTTON, ID_SAVE_BUTTON);
        saveEvent.SetEventObject(saveButton);
        OnSaveClicked(saveEvent);
    }
}

void cSave::OnDeleteClicked(wxCommandEvent& evt)
{
    int selection = saveFilesList->GetSelection();
    if (selection == wxNOT_FOUND)
        return;

    wxString fileName = saveFilesList->GetString(selection);
    int result = wxMessageBox("Are you sure you want to delete '" + fileName + "'?",
        "Confirm Delete", wxYES_NO | wxICON_QUESTION);

    if (result == wxYES)
    {
        wxString savesDir = GetSavesDirectory();
        wxString fullPath = savesDir + wxFileName::GetPathSeparator() + fileName + ".sav";

        if (wxFile::Exists(fullPath))
        {
            if (wxRemoveFile(fullPath))
            {
                wxMessageBox("Save file deleted: " + fileName, "Delete Successful", wxOK | wxICON_INFORMATION);
                RefreshSaveFilesList();
                UpdateButtonStates();
            }
            else
                wxMessageBox("Failed to delete save file.", "Error", wxOK | wxICON_ERROR);
        }
        else
        {
            wxMessageBox("Save file not found.", "Error", wxOK | wxICON_ERROR);
            RefreshSaveFilesList();
        }
    }
}

void cSave::OnBackClicked(wxCommandEvent& evt)
{
    wxCommandEvent switchEvent(wxEVT_SWITCH_TO_MENU);
    switchEvent.SetEventObject(this);

    wxWindow* parent = GetParent();
    while (parent && !parent->IsTopLevel())
        parent = parent->GetParent();

    if (parent)
        wxPostEvent(parent, switchEvent);
}

void cSave::OnSaveNameChanged(wxCommandEvent& evt)
{
    UpdateButtonStates();
}

void cSave::OnSaveFileSelected(wxCommandEvent& evt)
{
    int selection = saveFilesList->GetSelection();
    if (selection != wxNOT_FOUND)
    {
        wxString fileName = saveFilesList->GetString(selection);
        saveNameInput->SetValue(fileName);
    }
    UpdateButtonStates();
}

void cSave::OnButtonEnter(wxMouseEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());
    button->SetBackgroundColour(wxColor(129, 131, 132));
    button->Refresh();
    evt.Skip();
}

void cSave::OnButtonLeave(wxMouseEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());
    button->SetBackgroundColour(wxColor(58, 58, 60));
    button->Refresh();
    evt.Skip();
}

void cSave::OnButtonSetFocus(wxFocusEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());

    // Set to tab highlight color
    button->SetBackgroundColour(wxColor(86, 87, 88));
    button->Refresh();
    evt.Skip();
}

void cSave::OnButtonKillFocus(wxFocusEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());

    // Reset to the original background color
    button->SetBackgroundColour(wxColor(58, 58, 60));
    button->Refresh();
    evt.Skip();
}

void cSave::RefreshSaveFilesList()
{
    saveFilesList->Clear();

    wxString savesDir = GetSavesDirectory();

    if (!wxDir::Exists(savesDir))
        return;

    wxDir dir(savesDir);
    if (!dir.IsOpened())
        return;

    wxString filename;
    bool cont = dir.GetFirst(&filename, "*.sav", wxDIR_FILES);

    while (cont)
    {
        wxString displayName = filename;
        if (displayName.EndsWith(".sav"))
            displayName = displayName.SubString(0, displayName.Length() - 5);

        saveFilesList->Append(displayName);
        cont = dir.GetNext(&filename);
    }
}

void cSave::UpdateButtonStates()
{
    bool hasText = !saveNameInput->GetValue().Trim().IsEmpty();
    bool hasSelection = saveFilesList->GetSelection() != wxNOT_FOUND;

    saveButton->Enable(hasText);
    deleteButton->Enable(hasSelection);
}

wxButton* cSave::GetButtonById(int id)
{
    switch (id)
    {
    case ID_SAVE_BUTTON:
        return saveButton;
    case ID_DELETE_BUTTON:
        return deleteButton;
    case ID_BACK_BUTTON:
        return backButton;
    default:
        return nullptr;
    }
}