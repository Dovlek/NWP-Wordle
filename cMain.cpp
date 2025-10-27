#include "cMain.h"
#include "EmbeddedResources.h"
#include "Theme.h"
#include <wx/mstream.h>
#ifdef __WXMSW__
#include <dwmapi.h>
#include <windows.h>
#pragma comment(lib, "dwmapi.lib")
#endif

// Platform-specific initialization
#ifdef __WXMSW__
static void EnableDarkTitleBar(wxFrame* frame)
{
    HWND hwnd = frame->GetHWND();
    if (hwnd)
    {
        BOOL value = TRUE;
        DwmSetWindowAttribute(hwnd, 20, &value, sizeof(value));
    }
}
#else
static void EnableDarkTitleBar(wxFrame*)
{
}
#endif

// Define custom events
wxDEFINE_EVENT(wxEVT_SWITCH_TO_MENU, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_START_NEW_GAME, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CONTINUE_GAME, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_SWITCH_TO_SAVE, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_SWITCH_TO_LOAD, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_SWITCH_TO_OPTIONS, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_THEME_CHANGED, wxCommandEvent);

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "NWP - Wordle", wxDefaultPosition, wxDefaultSize)
{
    // Load icon from embedded resources (cross-platform)
    wxIcon appIcon;
    wxMemoryInputStream stream(IDI_ICON_data, IDI_ICON_data_size);
    wxImage img(stream, wxBITMAP_TYPE_ICO);
    if (img.IsOk())
    {
        appIcon.CopyFromBitmap(wxBitmap(img));
        if (appIcon.IsOk())
        {
            SetIcon(appIcon);
        }
    }

    // Enable dark title bar on Windows (Windows 10 build 18362+)
    EnableDarkTitleBar(this);

    wxPanel* mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);
    mainPanel->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());

    cSimplebook = new wxSimplebook(mainPanel, wxID_ANY);

    // Create and add the menu panel as the first Simplebook page
    menuPanel = new cMenu(cSimplebook);
    cSimplebook->AddPage(menuPanel, "Menu", true);

    // Create and add the Wordle game panel as the second Simplebook page
    wordlePanel = new cWordle(cSimplebook);
    cSimplebook->AddPage(wordlePanel, "Wordle", false);

    // Create and add the save panel as the third Simplebook page
    savePanel = new cSave(cSimplebook);
    cSimplebook->AddPage(savePanel, "Save", false);

    // Create and add the load panel as the fourth Simplebook page
    loadPanel = new cLoad(cSimplebook);
    cSimplebook->AddPage(loadPanel, "Load", false);

    // Create and add the options panel as the fifth Simplebook page
    optionsPanel = new cOptions(cSimplebook);
    cSimplebook->AddPage(optionsPanel, "Options", false);

    // Bind custom events
    Bind(wxEVT_SWITCH_TO_MENU, &cMain::OnSwitchToMenu, this);
    Bind(wxEVT_START_NEW_GAME, &cMain::OnStartNewGame, this);
    Bind(wxEVT_CONTINUE_GAME, &cMain::OnContinueGame, this);
    Bind(wxEVT_SWITCH_TO_SAVE, &cMain::OnSwitchToSave, this);
    Bind(wxEVT_SWITCH_TO_LOAD, &cMain::OnSwitchToLoad, this);
    Bind(wxEVT_SWITCH_TO_OPTIONS, &cMain::OnSwitchToOptions, this);
    Bind(wxEVT_THEME_CHANGED, &cMain::OnThemeChanged, this);

    // Set up the main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(cSimplebook, 1, wxEXPAND);
    mainPanel->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);

    menuPanel->SetNewGameButtonFocus();
}

cMain::~cMain()
{
}

void cMain::SwitchPageToMenu()
{
    if (cSimplebook)
    {
        cSimplebook->SetSelection(0);
    }

    UpdateMenuState();
}

void cMain::SwitchPageToWordle()
{
    if (cSimplebook)
    {
        cSimplebook->SetSelection(1);
    }
}

void cMain::SwitchPageToSave()
{
    if (cSimplebook)
    {
        cSimplebook->SetSelection(2);
    }
}

void cMain::SwitchPageToLoad()
{
    if (cSimplebook)
    {
        cSimplebook->SetSelection(3);
    }
}

void cMain::SwitchPageToOptions()
{
    if (cSimplebook)
    {
        cSimplebook->SetSelection(4);
    }
}

bool cMain::IsGameActive()
{
    return wordlePanel && wordlePanel->IsGameInProgress();
}

void cMain::EndCurrentGame()
{
    if (wordlePanel)
    {
        wordlePanel->ForceEndCurrentGame();
    }
    UpdateMenuState();
}

void cMain::UpdateMenuState()
{
    if (menuPanel && wordlePanel)
    {
        bool gameInProgress = wordlePanel->IsGameInProgress();
        menuPanel->SetContinueButtonEnabled(gameInProgress);
    }
}

void cMain::OnSwitchToMenu(wxCommandEvent& evt)
{
    if (menuPanel)
    {
        SwitchPageToMenu();
    }
}

void cMain::OnStartNewGame(wxCommandEvent& evt)
{
    if (wordlePanel)
    {
        wordlePanel->ResetStats();
        wordlePanel->StartNewRound();
    }

    SwitchPageToWordle();
}

void cMain::OnContinueGame(wxCommandEvent& evt)
{
    if (wordlePanel)
    {
        wordlePanel->ContinueFromFinishedGame();
        wordlePanel->ResumeTimer();
    }

    SwitchPageToWordle();
}

void cMain::OnSwitchToSave(wxCommandEvent& evt)
{
    if (savePanel)
    {
        savePanel->RefreshSaveFilesList();
        SwitchPageToSave();
    }
}

void cMain::OnSwitchToLoad(wxCommandEvent& evt)
{
    if (loadPanel)
    {
        loadPanel->RefreshSaveFilesList();
        SwitchPageToLoad();
    }
}

void cMain::OnSwitchToOptions(wxCommandEvent& evt)
{
    if (optionsPanel)
    {
        optionsPanel->LoadSettings();
        SwitchPageToOptions();
    }
}

void cMain::OnThemeChanged(wxCommandEvent& evt)
{
    RefreshAllPanelsTheme();
}

void cMain::RefreshAllPanelsTheme()
{
    // Update main panel background
    wxPanel* mainPanel = dynamic_cast<wxPanel*>(cSimplebook->GetParent());
    if (mainPanel)
    {
        mainPanel->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
        mainPanel->Refresh();
    }

    // Update frame background
    SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    Refresh();

    // Refresh all panels
    if (wordlePanel)
        wordlePanel->RefreshTheme();
    if (menuPanel)
        menuPanel->RefreshTheme();
    if (optionsPanel)
        optionsPanel->RefreshTheme();
    if (savePanel)
        savePanel->RefreshTheme();
    if (loadPanel)
        loadPanel->RefreshTheme();
}
