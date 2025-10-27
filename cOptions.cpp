#include "cOptions.h"
#include "UIScaler.h"
#include "cMain.h"
#include "cWordle.h"
#include "Theme.h"
#include <wx/filename.h>
#include <wx/stdpaths.h>

cOptions::cOptions(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
    SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());

    UIScaler& scaler = UIScaler::GetInstance();

    // Initialize config
    wxString configPath = wxStandardPaths::Get().GetUserDataDir();
    if (!wxFileName::DirExists(configPath))
        wxFileName::Mkdir(configPath, wxS_DIR_DEFAULT, wxPATH_MKDIR_FULL);

    wxString configFile = configPath + wxFileName::GetPathSeparator() + "wordle_config.ini";
    config = new wxFileConfig(wxEmptyString, wxEmptyString, configFile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

    // Titled
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "Game Options", wxDefaultPosition, wxDefaultSize);
    title->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    title->SetForegroundColour(ThemeManager::Get().GetTextColor());
    title->SetFont(wxFont(scaler.ScaledFontSize(24), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

    wxFont labelFont(scaler.ScaledFontSize(18), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    wxSize toggleSize = scaler.ScaledSize(350, 80);

    // Timed mode checkbox
    timedModeToggle = new wxToggleButton(this, ID_TIMED_MODE_CHECKBOX, "Enable Timed Mode", wxDefaultPosition, toggleSize, wxBORDER_NONE);
    timedModeToggle->SetBackgroundColour(ThemeManager::Get().GetButtonColor());
    timedModeToggle->SetForegroundColour(ThemeManager::Get().GetTextColor());
    timedModeToggle->SetFont(labelFont);
    timedModeToggle->Bind(wxEVT_TOGGLEBUTTON, &cOptions::OnTimedModeToggled, this);

    // Help text for timed mode
    wxStaticText* timedModeHelp = new wxStaticText(this, wxID_ANY, "(5 minutes per round)");
    timedModeHelp->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    timedModeHelp->SetForegroundColour(ThemeManager::Get().GetHelpTextColor());
    timedModeHelp->SetFont(wxFont(scaler.ScaledFontSize(12), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false));

    // Hard mode checkbox
    hardModeToggle = new wxToggleButton(this, ID_HARD_MODE_CHECKBOX, "Enable Hard Mode", wxDefaultPosition, toggleSize, wxBORDER_NONE);
    hardModeToggle->SetBackgroundColour(ThemeManager::Get().GetButtonColor());
    hardModeToggle->SetForegroundColour(ThemeManager::Get().GetTextColor());
    hardModeToggle->SetFont(labelFont);
    hardModeToggle->Bind(wxEVT_TOGGLEBUTTON, &cOptions::OnHardModeToggled, this);

    // Help text for hard mode
    wxStaticText* hardModeHelp = new wxStaticText(this, wxID_ANY, "(Any revealed hints must be used in subsequent guesses)");
    hardModeHelp->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    hardModeHelp->SetForegroundColour(ThemeManager::Get().GetHelpTextColor());
    hardModeHelp->SetFont(wxFont(scaler.ScaledFontSize(12), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false));

    // Theme selection buttons
    wxSize themeButtonSize = scaler.ScaledSize(170, 80);
    darkThemeButton = new wxButton(this, ID_DARK_THEME_BUTTON, "Dark", wxDefaultPosition, themeButtonSize, wxBORDER_NONE);
    darkThemeButton->SetFont(labelFont);
    darkThemeButton->SetCursor(wxCursor(wxCURSOR_HAND));
    darkThemeButton->Bind(wxEVT_BUTTON, &cOptions::OnDarkThemeClicked, this);

    lightThemeButton = new wxButton(this, ID_LIGHT_THEME_BUTTON, "Light", wxDefaultPosition, themeButtonSize, wxBORDER_NONE);
    lightThemeButton->SetFont(labelFont);
    lightThemeButton->SetCursor(wxCursor(wxCURSOR_HAND));
    lightThemeButton->Bind(wxEVT_BUTTON, &cOptions::OnLightThemeClicked, this);

    // Buttons
    wxSize buttonSize = scaler.ScaledSize(120, 40);
    backButton = new wxButton(this, ID_BACK_BUTTON, "Back", wxDefaultPosition, buttonSize, wxBORDER_NONE);

    // Style buttons
    wxFont buttonFont(scaler.ScaledFontSize(14), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

    // Style back button
    backButton->SetBackgroundColour(ThemeManager::Get().GetButtonColor());
    backButton->SetForegroundColour(ThemeManager::Get().GetTextColor());
    backButton->SetCursor(wxCursor(wxCURSOR_HAND));
    backButton->SetFont(buttonFont);

    // Bind events
    backButton->Bind(wxEVT_BUTTON, &cOptions::OnBackClicked, this);

    // Bind mouse events for back button
    backButton->Bind(wxEVT_ENTER_WINDOW, &cOptions::OnButtonEnter, this);
    backButton->Bind(wxEVT_LEAVE_WINDOW, &cOptions::OnButtonLeave, this);

    // Bind focus events for back button
    backButton->Bind(wxEVT_SET_FOCUS, &cOptions::OnButtonSetFocus, this);
    backButton->Bind(wxEVT_KILL_FOCUS, &cOptions::OnButtonKillFocus, this);

    // Bind mouse events for theme buttons
    darkThemeButton->Bind(wxEVT_ENTER_WINDOW, &cOptions::OnButtonEnter, this);
    darkThemeButton->Bind(wxEVT_LEAVE_WINDOW, &cOptions::OnButtonLeave, this);
    lightThemeButton->Bind(wxEVT_ENTER_WINDOW, &cOptions::OnButtonEnter, this);
    lightThemeButton->Bind(wxEVT_LEAVE_WINDOW, &cOptions::OnButtonLeave, this);

    // Bind focus events for theme buttons
    darkThemeButton->Bind(wxEVT_SET_FOCUS, &cOptions::OnButtonSetFocus, this);
    darkThemeButton->Bind(wxEVT_KILL_FOCUS, &cOptions::OnButtonKillFocus, this);
    lightThemeButton->Bind(wxEVT_SET_FOCUS, &cOptions::OnButtonSetFocus, this);
    lightThemeButton->Bind(wxEVT_KILL_FOCUS, &cOptions::OnButtonKillFocus, this);

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

    // Theme selection section
    wxStaticText* themeLabel = new wxStaticText(this, wxID_ANY, "Theme", wxDefaultPosition, wxDefaultSize);
    themeLabel->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    themeLabel->SetForegroundColour(ThemeManager::Get().GetTextColor());
    themeLabel->SetFont(wxFont(scaler.ScaledFontSize(16), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));
    contentSizer->Add(themeLabel, wxSizerFlags().CenterHorizontal().Border(wxALL, smallBorder));

    wxBoxSizer* themeSizer = new wxBoxSizer(wxHORIZONTAL);
    themeSizer->Add(darkThemeButton, wxSizerFlags().Border(wxALL, scaler.ScaledValue(5)));
    themeSizer->Add(lightThemeButton, wxSizerFlags().Border(wxALL, scaler.ScaledValue(5)));
    contentSizer->Add(themeSizer, wxSizerFlags().CenterHorizontal());

    // wxStaticText* themeHelp = new wxStaticText(this, wxID_ANY, "(Changes apply immediately)");
    // themeHelp->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    // themeHelp->SetForegroundColour(ThemeManager::Get().GetHelpTextColor());
    // themeHelp->SetFont(wxFont(scaler.ScaledFontSize(12), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_ITALIC, wxFONTWEIGHT_NORMAL, false));
    // contentSizer->Add(themeHelp, wxSizerFlags().CenterHorizontal().Border(wxTOP | wxBOTTOM, scaler.ScaledValue(5)));

    contentSizer->AddSpacer(sectionBorder);

    // Back button
    contentSizer->Add(backButton, wxSizerFlags().CenterHorizontal().Border(wxTOP, scaler.ScaledValue(60)));

    mainSizer->Add(contentSizer, wxSizerFlags().CenterHorizontal());
    mainSizer->AddStretchSpacer();

    this->SetSizer(mainSizer);

    // Load initial settings
    LoadSettings();

    // Update theme button states to show current selection
    UpdateThemeButtonStates();
}

cOptions::~cOptions()
{
    delete config;
}

void cOptions::LoadSettings()
{
    timedModeEnabled = config->ReadBool("/Settings/TimedMode", false);
    hardModeEnabled = config->ReadBool("/Settings/HardMode", false);

    // Store previous values for reverting if user cancels
    prevTimedModeEnabled = timedModeEnabled;
    prevHardModeEnabled = hardModeEnabled;

    timedModeToggle->SetValue(timedModeEnabled);
    hardModeToggle->SetValue(hardModeEnabled);

    // Update labels to show correct state
    UpdateToggleButtonLabel(timedModeToggle, "Enable Timed Mode");
    UpdateToggleButtonLabel(hardModeToggle, "Enable Hard Mode");
}

void cOptions::UpdateToggleButtonLabel(wxToggleButton* btn, const wxString& baseText)
{
    btn->SetLabel(baseText);
    if (btn->GetValue())
    {
        btn->SetBackgroundColour(ThemeManager::Get().GetToggleCheckedColor()); // Green when checked
    }
    else
    {
        btn->SetBackgroundColour(ThemeManager::Get().GetButtonColor()); // Gray when unchecked
    }
    btn->Refresh();
}

void cOptions::OnTimedModeToggled(wxCommandEvent& evt)
{
    UpdateToggleButtonLabel(timedModeToggle, "Enable Timed Mode");

    // Check if setting actually changed
    bool newValue = timedModeToggle->GetValue();
    if (newValue == timedModeEnabled)
    {
        return; // No change
    }

    // Show warning and ask for confirmation
    if (!CheckAndConfirmGameEnd())
    {
        // User cancelled - revert the toggle
        timedModeToggle->SetValue(timedModeEnabled);
        UpdateToggleButtonLabel(timedModeToggle, "Enable Timed Mode");
        return;
    }

    // User confirmed - save the setting immediately
    SaveTimedModeSetting();
}

void cOptions::OnHardModeToggled(wxCommandEvent& evt)
{
    UpdateToggleButtonLabel(hardModeToggle, "Enable Hard Mode");

    // Check if setting actually changed
    bool newValue = hardModeToggle->GetValue();
    if (newValue == hardModeEnabled)
    {
        return; // No change
    }

    // Show warning and ask for confirmation
    if (!CheckAndConfirmGameEnd())
    {
        // User cancelled - revert the toggle
        hardModeToggle->SetValue(hardModeEnabled);
        UpdateToggleButtonLabel(hardModeToggle, "Enable Hard Mode");
        return;
    }

    // User confirmed - save the setting immediately
    SaveHardModeSetting();
}

void cOptions::SaveTimedModeSetting()
{
    timedModeEnabled = timedModeToggle->GetValue();
    config->Write("/Settings/TimedMode", timedModeEnabled);
    config->Flush();

    // Update previous value
    prevTimedModeEnabled = timedModeEnabled;
}

void cOptions::SaveHardModeSetting()
{
    hardModeEnabled = hardModeToggle->GetValue();
    config->Write("/Settings/HardMode", hardModeEnabled);
    config->Flush();

    // Update previous value
    prevHardModeEnabled = hardModeEnabled;
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

void cOptions::OnDarkThemeClicked(wxCommandEvent& evt)
{
    // Only switch if not already dark theme
    if (ThemeManager::Get().GetCurrentTheme() == ThemeType::DARK)
    {
        return;
    }

    // Switch to dark theme immediately
    ThemeManager::Get().SetTheme(ThemeType::DARK);

    // Save to config
    config->Write("/Settings/LightTheme", false);
    config->Flush();

    // Broadcast theme change event to main window
    wxCommandEvent themeEvent(wxEVT_THEME_CHANGED);
    themeEvent.SetEventObject(this);
    wxWindow* parent = GetParent();
    while (parent && !parent->IsTopLevel())
        parent = parent->GetParent();
    if (parent)
        wxPostEvent(parent, themeEvent);

    // Update button states
    UpdateThemeButtonStates();
}

void cOptions::OnLightThemeClicked(wxCommandEvent& evt)
{
    // Only switch if not already light theme
    if (ThemeManager::Get().GetCurrentTheme() == ThemeType::LIGHT)
    {
        return;
    }

    // Switch to light theme immediately
    ThemeManager::Get().SetTheme(ThemeType::LIGHT);

    // Save to config
    config->Write("/Settings/LightTheme", true);
    config->Flush();

    // Broadcast theme change event to main window
    wxCommandEvent themeEvent(wxEVT_THEME_CHANGED);
    themeEvent.SetEventObject(this);
    wxWindow* parent = GetParent();
    while (parent && !parent->IsTopLevel())
        parent = parent->GetParent();
    if (parent)
        wxPostEvent(parent, themeEvent);

    // Update button states
    UpdateThemeButtonStates();
}

void cOptions::UpdateThemeButtonStates()
{
    bool isLightTheme = (ThemeManager::Get().GetCurrentTheme() == ThemeType::LIGHT);

    // Update button appearance based on current theme
    if (isLightTheme)
    {
        // Light theme is active
        darkThemeButton->SetBackgroundColour(ThemeManager::Get().GetButtonColor());
        lightThemeButton->SetBackgroundColour(ThemeManager::Get().GetToggleCheckedColor());
    }
    else
    {
        // Dark theme is active
        darkThemeButton->SetBackgroundColour(ThemeManager::Get().GetToggleCheckedColor());
        lightThemeButton->SetBackgroundColour(ThemeManager::Get().GetButtonColor());
    }

    darkThemeButton->SetForegroundColour(ThemeManager::Get().GetTextColor());
    lightThemeButton->SetForegroundColour(ThemeManager::Get().GetTextColor());

    darkThemeButton->Refresh();
    lightThemeButton->Refresh();
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
        button->SetBackgroundColour(ThemeManager::Get().GetButtonHoverColor());
        button->Refresh();
    }
    evt.Skip();
}

void cOptions::OnButtonLeave(wxMouseEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());
    if (button->IsEnabled())
    {
        button->SetBackgroundColour(ThemeManager::Get().GetButtonColor());
    }
    else
    {
        button->SetBackgroundColour(ThemeManager::Get().GetButtonDisabledColor());
    }
    button->Refresh();
    evt.Skip();
}

void cOptions::OnButtonSetFocus(wxFocusEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());
    if (button->IsEnabled())
    {
        button->SetBackgroundColour(ThemeManager::Get().GetButtonFocusColor());
        button->Refresh();
    }
    evt.Skip();
}

void cOptions::OnButtonKillFocus(wxFocusEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());
    if (button->IsEnabled())
    {
        button->SetBackgroundColour(ThemeManager::Get().GetButtonColor());
    }
    else
    {
        button->SetBackgroundColour(ThemeManager::Get().GetButtonDisabledColor());
    }
    button->Refresh();
    evt.Skip();
}

wxButton* cOptions::GetButtonById(int id)
{
    switch (id)
    {
    case ID_DARK_THEME_BUTTON:
        return darkThemeButton;
    case ID_LIGHT_THEME_BUTTON:
        return lightThemeButton;
    case ID_BACK_BUTTON:
        return backButton;
    default:
        return nullptr;
    }
}

void cOptions::RefreshTheme()
{
    SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());

    for (wxWindowList::compatibility_iterator node = GetChildren().GetFirst(); node; node = node->GetNext())
    {
        wxWindow* child = node->GetData();
        if (wxStaticText* text = dynamic_cast<wxStaticText*>(child))
        {
            // Check if it's help text (gray color)
            if (text->GetLabel().Contains("("))
                text->SetForegroundColour(ThemeManager::Get().GetHelpTextColor());
            else
                text->SetForegroundColour(ThemeManager::Get().GetTextColor());
            text->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
            text->Refresh();
        }
        else if (wxToggleButton* toggle = dynamic_cast<wxToggleButton*>(child))
        {
            toggle->SetBackgroundColour(toggle->GetValue() ? ThemeManager::Get().GetToggleCheckedColor() : ThemeManager::Get().GetButtonColor());
            toggle->SetForegroundColour(ThemeManager::Get().GetTextColor());
            toggle->Refresh();
        }
        else if (wxButton* button = dynamic_cast<wxButton*>(child))
        {
            // Theme buttons are handled separately
            if (button != darkThemeButton && button != lightThemeButton)
            {
                button->SetBackgroundColour(ThemeManager::Get().GetButtonColor());
                button->SetForegroundColour(ThemeManager::Get().GetTextColor());
                button->Refresh();
            }
        }
    }

    // Update theme button states to show current selection
    UpdateThemeButtonStates();

    Refresh();
}
