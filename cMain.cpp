#include "cMain.h"
#ifdef __WXMSW__
    #include <windows.h>
    #include <dwmapi.h>
    #pragma comment(lib, "dwmapi.lib")
#endif

// Define custom events
wxDEFINE_EVENT(wxEVT_SWITCH_TO_GAME, wxCommandEvent);
wxDEFINE_EVENT(wxEVT_SWITCH_TO_MENU, wxCommandEvent);

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
    
    // Bind custom events
    Bind(wxEVT_SWITCH_TO_MENU, &cMain::OnSwitchToMenu, this);
    Bind(wxEVT_SWITCH_TO_GAME, &cMain::OnSwitchToGame, this);
    
    // Set up the main sizer
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(cSimplebook, wxSizerFlags().Expand());
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
}

void cMain::SwitchPageToWordle()
{
    if (cSimplebook)
        cSimplebook->SetSelection(1);
}

void cMain::OnSwitchToMenu(wxCommandEvent& evt)
{
    SwitchPageToMenu();
}

void cMain::OnSwitchToGame(wxCommandEvent& evt)
{
    SwitchPageToWordle();
}
