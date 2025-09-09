#include "cMain.h"
#ifdef __WXMSW__
    #include <windows.h>
    #include <dwmapi.h>
    #pragma comment(lib, "dwmapi.lib")
#endif

// Define custom events
wxDEFINE_EVENT(wxEVT_SWITCH_TO_MENU, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_START_NEW_GAME, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_CONTINUE_GAME, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_SWITCH_TO_SAVE, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_SWITCH_TO_LOAD, wxCommandEvent);

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "NWP - Wordle", wxDefaultPosition, wxSize(800, 600))
{
    // Set the application icon
    wxIcon appIcon;
    appIcon.LoadFile(wxT("IDI_ICON"));
    if (appIcon.IsOk())
        SetIcon(appIcon);

    // Enable dark title bar on Windows (Windows 10 build 18362+)
    #ifdef __WXMSW__
        HWND hwnd = GetHWND();
        if (hwnd)
        {
            BOOL value = TRUE;
            DwmSetWindowAttribute(hwnd, 20, &value, sizeof(value));
        }
    #endif

    wxPanel* mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);
    mainPanel->SetBackgroundColour(wxColor(20, 20, 20));
    
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
    
    // Bind custom events
    Bind(wxEVT_SWITCH_TO_MENU, &cMain::OnSwitchToMenu, this);
    Bind(wxEVT_START_NEW_GAME, &cMain::OnStartNewGame, this);
    Bind(wxEVT_CONTINUE_GAME, &cMain::OnContinueGame, this);
    Bind(wxEVT_SWITCH_TO_SAVE, &cMain::OnSwitchToSave, this);
    Bind(wxEVT_SWITCH_TO_LOAD, &cMain::OnSwitchToLoad, this);
    
    // Set up the main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(cSimplebook, 1, wxEXPAND);
    mainPanel->SetSizer(mainSizer);
    mainSizer->SetSizeHints(this);
    mainPanel->SetFocus();
}

cMain::~cMain()
{
}

void cMain::SwitchPageToMenu()
{
    if (cSimplebook)
        cSimplebook->SetSelection(0);
    
    UpdateMenuState();
}

void cMain::SwitchPageToWordle()
{
    if (cSimplebook)
        cSimplebook->SetSelection(1);
}

void cMain::SwitchPageToSave()
{
    if (cSimplebook)
        cSimplebook->SetSelection(2);
}

void cMain::SwitchPageToLoad()
{
    if (cSimplebook)
        cSimplebook->SetSelection(3);
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
        SwitchPageToMenu();
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
        wordlePanel->ContinueFromFinishedGame();
    
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
