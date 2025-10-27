#include "cLoad.h"
#include "UIScaler.h"
#include "cWordle.h"
#include "Theme.h"
#include <wx/dir.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <wx/simplebook.h>
#include <wx/stdpaths.h>

cLoad::cLoad(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
    SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());

    UIScaler& scaler = UIScaler::GetInstance();

    // Title
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "Load Game", wxDefaultPosition, wxDefaultSize);
    title->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    title->SetForegroundColour(ThemeManager::Get().GetTextColor());
    title->SetFont(wxFont(scaler.ScaledFontSize(24), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

    // Saved games list
    wxStaticText* listLabel = new wxStaticText(this, wxID_ANY, "Saved Games:", wxDefaultPosition, wxDefaultSize);
    listLabel->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    listLabel->SetForegroundColour(ThemeManager::Get().GetTextColor());
    listLabel->SetFont(wxFont(scaler.ScaledFontSize(14), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));

    wxSize listSize = scaler.ScaledSize(400, 500);
    saveFilesList = new wxListBox(this, ID_SAVE_FILES_LIST_LOAD, wxDefaultPosition, listSize);
    saveFilesList->SetBackgroundColour(ThemeManager::Get().GetInputColor());
    saveFilesList->SetForegroundColour(ThemeManager::Get().GetTextColor());
    saveFilesList->SetFont(wxFont(scaler.ScaledFontSize(12), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false));

    // Buttons
    wxSize buttonSize = scaler.ScaledSize(120, 40);
    loadButton = new wxButton(this, ID_LOAD_BUTTON, "Load", wxDefaultPosition, buttonSize, wxBORDER_NONE);
    backButton = new wxButton(this, ID_BACK_BUTTON_LOAD, "Back", wxDefaultPosition, buttonSize, wxBORDER_NONE);

    // Style buttons
    wxFont buttonFont(scaler.ScaledFontSize(14), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    for (int buttonId = ID_LOAD_BUTTON; buttonId <= ID_BACK_BUTTON_LOAD; buttonId++)
    {
        wxButton* button = GetButtonById(buttonId);
        if (button)
        {
            button->SetBackgroundColour(ThemeManager::Get().GetInputColor());
            button->SetForegroundColour(ThemeManager::Get().GetTextColor());
            button->SetCursor(wxCursor(wxCURSOR_HAND));
            button->SetFont(buttonFont);
        }
    }

    // Bind events
    saveFilesList->Bind(wxEVT_LISTBOX, &cLoad::OnSaveFileSelected, this);
    saveFilesList->Bind(wxEVT_CHAR_HOOK, &cLoad::OnKeyboardPressed, this);
    loadButton->Bind(wxEVT_BUTTON, &cLoad::OnLoadClicked, this);
    backButton->Bind(wxEVT_BUTTON, &cLoad::OnBackClicked, this);

    // Bind mouse events for buttons
    for (int buttonId = ID_LOAD_BUTTON; buttonId <= ID_BACK_BUTTON_LOAD; buttonId++)
    {
        wxButton* button = GetButtonById(buttonId);
        if (button)
        {
            button->Bind(wxEVT_ENTER_WINDOW, &cLoad::OnButtonEnter, this);
            button->Bind(wxEVT_LEAVE_WINDOW, &cLoad::OnButtonLeave, this);
        }
    }

    // Bind focus events
    for (int buttonId = ID_LOAD_BUTTON; buttonId <= ID_BACK_BUTTON_LOAD; buttonId++)
    {
        wxButton* button = GetButtonById(buttonId);
        if (button)
        {
            button->Bind(wxEVT_SET_FOCUS, &cLoad::OnButtonSetFocus, this);
            button->Bind(wxEVT_KILL_FOCUS, &cLoad::OnButtonKillFocus, this);
        }
    }

    // Set up accelerator table
    wxAcceleratorEntry entries[1];
    entries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, ID_ACCEL_ESCAPE);
    wxAcceleratorTable accelTable(1, entries);
    SetAcceleratorTable(accelTable);

    // Bind accelerator events
    Bind(wxEVT_COMMAND_MENU_SELECTED, &cLoad::OnAcceleratorPressed, this, ID_ACCEL_ESCAPE);

    // Set up layout
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* listSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    int titleBorder = scaler.ScaledValue(20);
    int listlabelBorder = scaler.ScaledValue(10);
    // int savelistBorder = scaler.ScaledValue(10);
    int buttonBorderSmall = scaler.ScaledValue(5);
    int buttonBorderLarge = scaler.ScaledValue(10);
    int loadBorder = scaler.ScaledValue(20);

    mainSizer->AddStretchSpacer();
    mainSizer->Add(title, wxSizerFlags().CenterHorizontal().Border(wxALL, titleBorder));

    listSizer->Add(listLabel, wxSizerFlags().Left().Border(wxLEFT | wxRIGHT | wxTOP, listlabelBorder));
    listSizer->Add(saveFilesList, wxSizerFlags().Expand().Border(wxALL, listlabelBorder));

    buttonSizer->Add(loadButton, wxSizerFlags().Border(wxALL, buttonBorderSmall));
    buttonSizer->Add(backButton, wxSizerFlags().Border(wxALL, buttonBorderSmall));

    mainSizer->Add(listSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, loadBorder));
    mainSizer->Add(buttonSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, buttonBorderLarge));
    mainSizer->AddStretchSpacer();

    SetSizer(mainSizer);

    RefreshSaveFilesList();
    UpdateButtonStates();
}

cLoad::~cLoad()
{
}

void cLoad::OnAcceleratorPressed(wxCommandEvent& evt)
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

    SetFocus();
}

void cLoad::OnKeyboardPressed(wxKeyEvent& evt)
{
    int keyCode = evt.GetKeyCode();

    if (keyCode == WXK_RETURN || keyCode == WXK_NUMPAD_ENTER)
    {
        if (loadButton->IsEnabled())
        {
            wxCommandEvent loadEvent(wxEVT_BUTTON, ID_LOAD_BUTTON);
            loadEvent.SetEventObject(loadButton);
            OnLoadClicked(loadEvent);
            return;
        }
    }

    evt.Skip();
}

wxString cLoad::GetSavesDirectory()
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

cWordle* cLoad::GetWordlePanel()
{
    wxSimplebook* simplebook = dynamic_cast<wxSimplebook*>(wxWindow::GetParent());
    if (simplebook && simplebook->GetPageCount() >= 1)
        return dynamic_cast<cWordle*>(simplebook->GetPage(1));

    return nullptr;
}

void cLoad::OnLoadClicked(wxCommandEvent& evt)
{
    int selection = saveFilesList->GetSelection();
    if (selection == wxNOT_FOUND) // Shouldn't trigger since load button would be disabled
    {
        wxMessageBox("Please select a save file to load.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxString fileName = saveFilesList->GetString(selection);
    wxString savesDir = GetSavesDirectory();
    wxString fullPath = savesDir + wxFileName::GetPathSeparator() + fileName + ".sav";

    if (!wxFile::Exists(fullPath))
    {
        wxMessageBox("Save file not found.", "Error", wxOK | wxICON_ERROR);
        RefreshSaveFilesList();
        return;
    }

    // Read save file content
    wxFile saveFile;
    if (!saveFile.Open(fullPath, wxFile::read))
    {
        wxMessageBox("Failed to open save file.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    wxString gameStateData;
    if (!saveFile.ReadAll(&gameStateData))
    {
        wxMessageBox("Failed to read save file data.", "Error", wxOK | wxICON_ERROR);
        saveFile.Close();
        return;
    }

    saveFile.Close();

    // Get Wordle panel and load the game state
    cWordle* wordlePanel = GetWordlePanel();
    if (!wordlePanel)
    {
        wxMessageBox("Cannot access game data.", "Error", wxOK | wxICON_ERROR);
        return;
    }

    if (wordlePanel->SetGameStateData(gameStateData))
    {
        wxMessageBox("Game loaded: " + fileName, "Load Successful", wxOK | wxICON_INFORMATION);

        // Switch to game panel
        wxCommandEvent switchEvent(wxEVT_CONTINUE_GAME);
        switchEvent.SetEventObject(this);

        wxWindow* parent = GetParent();
        while (parent && !parent->IsTopLevel())
            parent = parent->GetParent();

        if (parent)
            wxPostEvent(parent, switchEvent);
    }
    else
        wxMessageBox("Failed to load game. The save file may be corrupted or incompatible.", "Error", wxOK | wxICON_ERROR);
}

void cLoad::OnBackClicked(wxCommandEvent& evt)
{
    wxCommandEvent switchEvent(wxEVT_SWITCH_TO_MENU);
    switchEvent.SetEventObject(this);

    wxWindow* parent = GetParent();
    while (parent && !parent->IsTopLevel())
        parent = parent->GetParent();

    if (parent)
        wxPostEvent(parent, switchEvent);
}

void cLoad::OnSaveFileSelected(wxCommandEvent& evt)
{
    UpdateButtonStates();
}

void cLoad::OnButtonEnter(wxMouseEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());
    button->SetBackgroundColour(ThemeManager::Get().GetButtonHoverColor());
    button->Refresh();
    evt.Skip();
}

void cLoad::OnButtonLeave(wxMouseEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());
    button->SetBackgroundColour(ThemeManager::Get().GetInputColor());
    button->Refresh();
    evt.Skip();
}

void cLoad::OnButtonSetFocus(wxFocusEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());

    // Set to tab highlight color
    button->SetBackgroundColour(ThemeManager::Get().GetButtonFocusColor());
    button->Refresh();
    evt.Skip();
}

void cLoad::OnButtonKillFocus(wxFocusEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());

    // Reset to the original background color
    button->SetBackgroundColour(ThemeManager::Get().GetInputColor());
    button->Refresh();
    evt.Skip();
}

void cLoad::RefreshSaveFilesList()
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

    if (saveFilesList->GetCount() > 0 && saveFilesList->GetSelection() == wxNOT_FOUND)
    {
        saveFilesList->SetSelection(0);
        saveFilesList->SetFocus();
    }
    UpdateButtonStates();
}

void cLoad::UpdateButtonStates()
{
    bool hasSelection = saveFilesList->GetSelection() != wxNOT_FOUND;
    loadButton->Enable(hasSelection);
}

wxButton* cLoad::GetButtonById(int id)
{
    switch (id)
    {
    case ID_LOAD_BUTTON:
        return loadButton;
    case ID_BACK_BUTTON_LOAD:
        return backButton;
    default:
        return nullptr;
    }
}

void cLoad::RefreshTheme()
{
    SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());

    for (wxWindowList::compatibility_iterator node = GetChildren().GetFirst(); node; node = node->GetNext())
    {
        wxWindow* child = node->GetData();
        if (wxStaticText* text = dynamic_cast<wxStaticText*>(child))
        {
            text->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
            text->SetForegroundColour(ThemeManager::Get().GetTextColor());
            text->Refresh();
        }
        else if (wxListBox* listBox = dynamic_cast<wxListBox*>(child))
        {
            listBox->SetBackgroundColour(ThemeManager::Get().GetInputColor());
            listBox->SetForegroundColour(ThemeManager::Get().GetTextColor());
            listBox->Refresh();
        }
        else if (wxButton* button = dynamic_cast<wxButton*>(child))
        {
            button->SetBackgroundColour(ThemeManager::Get().GetInputColor());
            button->SetForegroundColour(ThemeManager::Get().GetTextColor());
            button->Refresh();
        }
    }

    Refresh();
}