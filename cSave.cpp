#include "cSave.h"
#include "cWordle.h"
#include "UIScaler.h"
#include <wx/dir.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/tokenzr.h>
#include <wx/simplebook.h>

cSave::cSave(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
    SetBackgroundColour(wxColor(20, 20, 20));

    UIScaler& scaler = UIScaler::GetInstance();

    // Title
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "Save Game", wxDefaultPosition, wxDefaultSize);
    title->SetBackgroundColour(wxColor(20, 20, 20));
    title->SetForegroundColour(wxColor(*wxWHITE));
    title->SetFont(wxFont(scaler.ScaledFontSize(24), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

    // Save name input
    wxStaticText* nameLabel = new wxStaticText(this, wxID_ANY, "Save Name:", wxDefaultPosition, wxDefaultSize);
    nameLabel->SetBackgroundColour(wxColor(20, 20, 20));
    nameLabel->SetForegroundColour(wxColor(*wxWHITE));
    nameLabel->SetFont(wxFont(scaler.ScaledFontSize(14), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));

    wxSize inputSize = scaler.ScaledSize(400, 25);
    saveNameInput = new wxTextCtrl(this, ID_SAVE_NAME_INPUT, "", wxDefaultPosition, inputSize, wxTE_PROCESS_ENTER);
    saveNameInput->SetBackgroundColour(wxColor(58, 58, 60));
    saveNameInput->SetForegroundColour(wxColor(*wxWHITE));
    saveNameInput->SetFont(wxFont(scaler.ScaledFontSize(12), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));
    saveNameInput->SetMaxLength(25);

    // Existing saves list
    wxStaticText* listLabel = new wxStaticText(this, wxID_ANY, "Existing Saves:", wxDefaultPosition, wxDefaultSize);
    listLabel->SetBackgroundColour(wxColor(20, 20, 20));
    listLabel->SetForegroundColour(wxColor(*wxWHITE));
    listLabel->SetFont(wxFont(scaler.ScaledFontSize(14), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));

    wxSize listSize = scaler.ScaledSize(400, 425);
    saveFilesList = new wxListBox(this, ID_SAVE_FILES_LIST, wxDefaultPosition, listSize);
    saveFilesList->SetBackgroundColour(wxColor(58, 58, 60));
    saveFilesList->SetForegroundColour(wxColor(*wxWHITE));
    saveFilesList->SetFont(wxFont(scaler.ScaledFontSize(12), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));

    // Buttons
    wxSize buttonSize = scaler.ScaledSize(120, 40);
    saveButton = new wxButton(this, ID_SAVE_BUTTON, "Save", wxDefaultPosition, buttonSize, wxBORDER_NONE);
    deleteButton = new wxButton(this, ID_DELETE_BUTTON, "Delete", wxDefaultPosition, buttonSize, wxBORDER_NONE);
    backButton = new wxButton(this, ID_BACK_BUTTON, "Back", wxDefaultPosition, buttonSize, wxBORDER_NONE);

    // Style buttons
    wxFont buttonFont(scaler.ScaledFontSize(14), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
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

    // Bind focus events
    saveFilesList->Bind(wxEVT_SET_FOCUS, &cSave::OnSaveFilesListSetFocus, this);

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

    // Set up accelerator table
    wxAcceleratorEntry entries[2];
    entries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, ID_ACCEL_ESCAPE);
    entries[1].Set(wxACCEL_NORMAL, WXK_DELETE, ID_ACCEL_DELETE);
    wxAcceleratorTable accelTable(2, entries);
    SetAcceleratorTable(accelTable);

    // Bind accelerator events
    Bind(wxEVT_COMMAND_MENU_SELECTED, &cSave::OnAcceleratorPressed, this, ID_ACCEL_ESCAPE);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &cSave::OnAcceleratorPressed, this, ID_ACCEL_DELETE);

    // Bind keyboard events
    Bind(wxEVT_CHAR_HOOK, &cSave::OnKeyboardPressed, this);

    // Set up layout
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* inputSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    int titleBorder = scaler.ScaledValue(20);
    int inputBorderSmall = scaler.ScaledValue(10);
    int inputBorderLarge = scaler.ScaledValue(20);
    int buttonBorderSmall = scaler.ScaledValue(5);
    int buttonBorderLarge = scaler.ScaledValue(10);

    mainSizer->AddStretchSpacer();
    mainSizer->Add(title, wxSizerFlags().CenterHorizontal().Border(wxALL, titleBorder));

    inputSizer->Add(nameLabel, wxSizerFlags().Left().Border(wxLEFT | wxRIGHT | wxTOP, inputBorderSmall));
    inputSizer->Add(saveNameInput, wxSizerFlags().Expand().Border(wxALL, inputBorderSmall));
    inputSizer->Add(listLabel, wxSizerFlags().Left().Border(wxLEFT | wxRIGHT | wxTOP, inputBorderSmall));
    inputSizer->Add(saveFilesList, wxSizerFlags().Expand().Border(wxALL, inputBorderSmall));

    buttonSizer->Add(saveButton, wxSizerFlags().Border(wxALL, buttonBorderSmall));
    buttonSizer->Add(deleteButton, wxSizerFlags().Border(wxALL, buttonBorderSmall));
    buttonSizer->Add(backButton, wxSizerFlags().Border(wxALL, buttonBorderSmall));

    mainSizer->Add(inputSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, inputBorderLarge));
    mainSizer->Add(buttonSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, buttonBorderLarge));
    mainSizer->AddStretchSpacer();

    SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);

    RefreshSaveFilesList();
    UpdateButtonStates();
}

cSave::~cSave()
{
}

void cSave::OnAcceleratorPressed(wxCommandEvent& evt)
{
    int id = evt.GetId();

    if (id == ID_ACCEL_ESCAPE)
    {
        wxCommandEvent switchEvent(wxEVT_SWITCH_TO_MENU);
        switchEvent.SetEventObject(this);

        wxWindow* parent = GetParent();
        while (parent && !parent->IsTopLevel())
            parent = parent->GetParent();

        if (parent)
            wxPostEvent(parent, switchEvent);
    }
    else if (id == ID_ACCEL_DELETE)
    {
        if (deleteButton->IsEnabled())
        {
            wxCommandEvent deleteEvent(wxEVT_BUTTON, ID_DELETE_BUTTON);
            deleteEvent.SetEventObject(deleteButton);
            OnDeleteClicked(deleteEvent);
        }
    }

    SetFocus();
}

void cSave::OnKeyboardPressed(wxKeyEvent& evt)
{
    int keyCode = evt.GetKeyCode();
    wxWindow* focusedWindow = FindFocus();

    // Handle Delete key when not in text input
    if (keyCode == WXK_DELETE)
    {
        if (focusedWindow != saveNameInput && deleteButton->IsEnabled())
        {
            wxCommandEvent deleteEvent(wxEVT_BUTTON, ID_DELETE_BUTTON);
            deleteEvent.SetEventObject(deleteButton);
            OnDeleteClicked(deleteEvent);
            SetFocus();
            return;
        }
    }
    // Handle Enter key when saveFilesList has focus
    else if (keyCode == WXK_RETURN || keyCode == WXK_NUMPAD_ENTER)
    {
        if (focusedWindow == saveFilesList && saveButton->IsEnabled())
        {
            wxCommandEvent saveEvent(wxEVT_BUTTON, ID_SAVE_BUTTON);
            saveEvent.SetEventObject(saveButton);
            OnSaveClicked(saveEvent);
            return;
        }
    }
    // Handle Down arrow key when saveNameInput has focus
    else if (keyCode == WXK_DOWN)
    {
        if (focusedWindow == saveNameInput && saveFilesList->GetCount() > 0)
        {
            saveFilesList->SetSelection(0);

            wxCommandEvent selectionEvent(wxEVT_LISTBOX, ID_SAVE_FILES_LIST);
            selectionEvent.SetEventObject(saveFilesList);
            OnSaveFileSelected(selectionEvent);

            saveFilesList->SetFocus();
            return;
        }
    }
    // Handle Up arrow key when saveNameInput has focus
    else if (keyCode == WXK_UP)
    {
        if (focusedWindow == saveFilesList && saveFilesList->GetCount() > 0 && saveFilesList->GetSelection() == 0)
        {
            saveFilesList->SetSelection(wxNOT_FOUND);
            saveNameInput->SetFocus();
            saveNameInput->SelectAll();
            return;
        }
    }

    evt.Skip();
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
        //wxMessageBox("Game saved as: " + saveName, "Save Successful", wxOK | wxICON_INFORMATION);
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
                //wxMessageBox("Save file deleted: " + fileName, "Delete Successful", wxOK | wxICON_INFORMATION);
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

void cSave::OnSaveFilesListSetFocus(wxFocusEvent& evt)
{
    if (saveFilesList->GetCount() > 0 && saveFilesList->GetSelection() == wxNOT_FOUND)
    {
        saveFilesList->SetSelection(0);

        wxCommandEvent selectionEvent(wxEVT_LISTBOX, ID_SAVE_FILES_LIST);
        selectionEvent.SetEventObject(saveFilesList);
        OnSaveFileSelected(selectionEvent);
    }

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

    saveNameInput->SetValue(wxEmptyString);
    saveNameInput->SetFocus();
    UpdateButtonStates();
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