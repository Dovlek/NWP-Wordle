#include "cOptions.h"
#include "UIScaler.h"
#include "cMain.h"
#include "cWordle.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>

cOptions::cOptions(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
    SetBackgroundColour(wxColor(20, 20, 20));

    UIScaler& scaler = UIScaler::GetInstance();

    // Initialize config
    wxString configPath = wxStandardPaths::Get().GetUserDataDir();
    if (!wxFileName::DirExists(configPath))
        wxFileName::Mkdir(configPath, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);

    wxString configFile = configPath + wxFileName::GetPathSeparator() + "wordle_config.ini";
    config = new wxFileConfig(wxEmptyString, wxEmptyString, configFile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

    // Titled
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "Game Options", wxDefaultPosition, wxDefaultSize);
    title->SetBackgroundColour(wxColor(20, 20, 20));
    title->SetForegroundColour(wxColor(*wxWHITE));
    title->SetFont(wxFont(scaler.ScaledFontSize(24), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

    wxFont labelFont(scaler.ScaledFontSize(18), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    wxSize toggleSize = scaler.ScaledSize(350, 80);

    // Timed mode checkbox
    timedModeToggle = new wxToggleButton(this, ID_TIMED_MODE_CHECKBOX, "Enable Timed Mode", wxDefaultPosition, toggleSize, wxBORDER_NONE);
    timedModeToggle->SetBackgroundColour(wxColor(58, 58, 60));
    timedModeToggle->SetForegroundColour(wxColor(*wxWHITE));
    timedModeToggle->SetFont(labelFont);
    timedModeToggle->Bind(wxEVT_TOGGLEBUTTON, &cOptions::OnTimedModeToggled, this);

    // Help text for timed mode
    wxStaticText* timedModeHelp = new wxStaticText(this, wxID_ANY, "(5 minutes per round)");
    timedModeHelp->SetBackgroundColour(wxColor(20, 20, 20));
    timedModeHelp->SetForegroundColour(wxColor(150, 150, 150));
    timedModeHelp->SetFont(wxFont(scaler.ScaledFontSize(12), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false));

    // Hard mode checkbox
    hardModeToggle = new wxToggleButton(this, ID_HARD_MODE_CHECKBOX, "Enable Hard Mode", wxDefaultPosition, toggleSize, wxBORDER_NONE);
    hardModeToggle->SetBackgroundColour(wxColor(58, 58, 60));
    hardModeToggle->SetForegroundColour(wxColor(*wxWHITE));
    hardModeToggle->SetFont(labelFont);
    hardModeToggle->Bind(wxEVT_TOGGLEBUTTON, &cOptions::OnHardModeToggled, this);

    // Help text for hard mode
    wxStaticText* hardModeHelp = new wxStaticText(this, wxID_ANY, "(Any revealed hints must be used in subsequent guesses)");
    hardModeHelp->SetBackgroundColour(wxColor(20, 20, 20));
    hardModeHelp->SetForegroundColour(wxColor(150, 150, 150));
    hardModeHelp->SetFont(wxFont(scaler.ScaledFontSize(12), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false));

    // Buttons
    wxSize buttonSize = scaler.ScaledSize(120, 40);
    applyButton = new wxButton(this, ID_SAVE_BUTTON, "Apply", wxDefaultPosition, buttonSize, wxBORDER_NONE);
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
    applyButton->Bind(wxEVT_BUTTON, &cOptions::OnSaveClicked, this);
    backButton->Bind(wxEVT_BUTTON, &cOptions::OnBackClicked, this);

    // Bind mouse events for buttons
    for (int buttonId = ID_SAVE_BUTTON; buttonId <= ID_BACK_BUTTON; buttonId++)
    {
        wxButton* button = GetButtonById(buttonId);
        if (button)
        {
            button->Bind(wxEVT_ENTER_WINDOW, &cOptions::OnButtonEnter, this);
            button->Bind(wxEVT_LEAVE_WINDOW, &cOptions::OnButtonLeave, this);
        }
    }

    // Bind focus events
    for (int buttonId = ID_SAVE_BUTTON; buttonId <= ID_BACK_BUTTON; buttonId++)
    {
        wxButton* button = GetButtonById(buttonId);
        if (button)
        {
            button->Bind(wxEVT_SET_FOCUS, &cOptions::OnButtonSetFocus, this);
            button->Bind(wxEVT_KILL_FOCUS, &cOptions::OnButtonKillFocus, this);
        }
    }

    // Set up accelerator table
    wxAcceleratorEntry entries[1];
    entries[0].Set(wxACCEL_NORMAL, WXK_ESCAPE, ID_ACCEL_ESCAPE);
    wxAcceleratorTable accelTable(1, entries);
    SetAcceleratorTable(accelTable);

    Bind(wxEVT_COMMAND_MENU_SELECTED, &cOptions::OnAcceleratorPressed, this, ID_ACCEL_ESCAPE);

    // Layout
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* contentSizer = new wxBoxSizer(wxVERTICAL);

    int titleBorder = scaler.ScaledValue(40);
    int sectionBorder = scaler.ScaledValue(20);
    int smallBorder = scaler.ScaledValue(10);

    mainSizer->AddStretchSpacer();
    mainSizer->Add(title, wxSizerFlags().CenterHorizontal().Border(wxALL, titleBorder));

    // Timed mode section
    contentSizer->Add(timedModeToggle, wxSizerFlags().CenterHorizontal().Border(wxALL, smallBorder));
    contentSizer->Add(timedModeHelp, wxSizerFlags().CenterHorizontal().Border(wxTOP | wxBOTTOM, scaler.ScaledValue(5)));
    contentSizer->AddSpacer(sectionBorder);

    // Hard mode section
    contentSizer->Add(hardModeToggle, wxSizerFlags().CenterHorizontal().Border(wxALL, smallBorder));
    contentSizer->Add(hardModeHelp, wxSizerFlags().CenterHorizontal().Border(wxTOP | wxBOTTOM, scaler.ScaledValue(5)));

    contentSizer->AddSpacer(sectionBorder);

    // Buttons
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(applyButton, wxSizerFlags().Border(wxALL, smallBorder));
    buttonSizer->Add(backButton, wxSizerFlags().Border(wxALL, smallBorder));
    contentSizer->Add(buttonSizer, wxSizerFlags().CenterHorizontal());

    mainSizer->Add(contentSizer, wxSizerFlags().CenterHorizontal());
    mainSizer->AddStretchSpacer();

    this->SetSizer(mainSizer);

    // Load initial settings
    LoadSettings();
}

cOptions::~cOptions()
{
    delete config;
}

void cOptions::LoadSettings()
{
    timedModeEnabled = config->ReadBool("/Settings/TimedMode", false);
    hardModeEnabled = config->ReadBool("/Settings/HardMode", false);

    timedModeToggle->SetValue(timedModeEnabled);
    hardModeToggle->SetValue(hardModeEnabled);

    // Update labels to show correct state
    UpdateToggleButtonLabel(timedModeToggle, "Enable Timed Mode");
    UpdateToggleButtonLabel(hardModeToggle, "Enable Hard Mode");

    // Disable Apply button when entering options panel
    applyButton->Enable(false);
    applyButton->SetBackgroundColour(wxColor(40, 40, 40));
    applyButton->Refresh();
}

void cOptions::UpdateToggleButtonLabel(wxToggleButton* btn, const wxString& baseText)
{
    btn->SetLabel(baseText);
    if (btn->GetValue())
    {
        btn->SetBackgroundColour(wxColor(83, 141, 78)); // Green when checked
    }
    else
    {
        btn->SetBackgroundColour(wxColor(58, 58, 60)); // Gray when unchecked
    }
    btn->Refresh();
}

void cOptions::OnTimedModeToggled(wxCommandEvent& evt)
{
    UpdateToggleButtonLabel(timedModeToggle, "Enable Timed Mode");
    applyButton->Enable(true);
    applyButton->SetBackgroundColour(wxColor(58, 58, 60));
    applyButton->Refresh();
}

void cOptions::OnHardModeToggled(wxCommandEvent& evt)
{
    UpdateToggleButtonLabel(hardModeToggle, "Enable Hard Mode");
    applyButton->Enable(true);
    applyButton->SetBackgroundColour(wxColor(58, 58, 60));
    applyButton->Refresh();
}

void cOptions::SaveSettings()
{
    timedModeEnabled = timedModeToggle->GetValue();
    hardModeEnabled = hardModeToggle->GetValue();

    config->Write("/Settings/TimedMode", timedModeEnabled);
    config->Write("/Settings/HardMode", hardModeEnabled);

    // Disable Apply button after saving
    applyButton->Enable(false);
    applyButton->SetBackgroundColour(wxColor(40, 40, 40));
    applyButton->Refresh();
}

bool cOptions::CheckAndConfirmGameEnd()
{
    // Get the main window by traversing up the parent chain
    wxWindow* window = GetParent();
    cMain* mainWindow = nullptr;

    while (window && !mainWindow)
    {
        mainWindow = dynamic_cast<cMain*>(window);
        if (!mainWindow)
            window = window->GetParent();
    }

    if (!mainWindow)
    {
        wxLogDebug("Could not find main window!");
        return true; // Allow save if we can't find main window
    }

    if (mainWindow->IsGameActive())
    {
        int result = wxMessageBox("Changing game settings will end your current game.\n\nDo you want to continue?", "End Current Game?", wxYES_NO | wxICON_WARNING, this);

        if (result == wxYES)
        {
            mainWindow->EndCurrentGame();
            return true;
        }
        else
        {
            return false;
        }
    }

    return true; // No active game, OK to proceed
}

void cOptions::OnSaveClicked(wxCommandEvent& evt)
{
    if (CheckAndConfirmGameEnd())
    {
        SaveSettings();

        // wxString configPath = wxStandardPaths::Get().GetUserDataDir();
        // wxString configFile = configPath + wxFileName::GetPathSeparator() + "wordle_config.ini";
        // wxString msg = wxString::Format("Settings saved successfully!\n\nConfig file: %s", configFile);
        // wxMessageBox(msg, "Success", wxOK | wxICON_INFORMATION, this);
    }
}

void cOptions::OnBackClicked(wxCommandEvent& evt)
{
    wxCommandEvent switchEvent(wxEVT_SWITCH_TO_MENU);
    switchEvent.SetEventObject(this);

    wxWindow* parent = GetParent();
    while (parent && !parent->IsTopLevel())
        parent = parent->GetParent();

    if (parent)
        wxPostEvent(parent, switchEvent);
}

void cOptions::OnAcceleratorPressed(wxCommandEvent& evt)
{
    if (evt.GetId() == ID_ACCEL_ESCAPE)
    {
        OnBackClicked(evt);
    }
}

void cOptions::OnButtonEnter(wxMouseEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());
    if (button->IsEnabled())
    {
        button->SetBackgroundColour(wxColor(129, 131, 132));
        button->Refresh();
    }
    evt.Skip();
}

void cOptions::OnButtonLeave(wxMouseEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());
    if (button->IsEnabled())
    {
        button->SetBackgroundColour(wxColor(58, 58, 60));
    }
    else
    {
        button->SetBackgroundColour(wxColor(40, 40, 40));
    }
    button->Refresh();
    evt.Skip();
}

void cOptions::OnButtonSetFocus(wxFocusEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());
    if (button->IsEnabled())
    {
        button->SetBackgroundColour(wxColor(86, 87, 88));
        button->Refresh();
    }
    evt.Skip();
}

void cOptions::OnButtonKillFocus(wxFocusEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());
    if (button->IsEnabled())
    {
        button->SetBackgroundColour(wxColor(58, 58, 60));
    }
    else
    {
        button->SetBackgroundColour(wxColor(40, 40, 40));
    }
    button->Refresh();
    evt.Skip();
}

wxButton* cOptions::GetButtonById(int id)
{
    switch (id)
    {
    case ID_SAVE_BUTTON:
        return applyButton;
    case ID_BACK_BUTTON:
        return backButton;
    default:
        return nullptr;
    }
}
