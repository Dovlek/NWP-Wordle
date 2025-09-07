#include "cMenu.h"

cMenu::cMenu(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
    SetBackgroundColour(wxColor(20, 20, 20));
    
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "Welcome to WORDLE!", wxDefaultPosition, wxDefaultSize);
    title->SetBackgroundColour(wxColor(20, 20, 20));
    title->SetForegroundColour(wxColor(*wxWHITE));
    title->SetFont(wxFont(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));
    
    wxFont buttonFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    continueButton = new wxButton(this, ID_CONTINUE, "Continue", wxDefaultPosition, wxSize(350, 80));
    continueButton->SetFont(buttonFont);
    newGameButton = new wxButton(this, ID_NEW_GAME, "New Game", wxDefaultPosition, wxSize(350, 80));
    newGameButton->SetFont(buttonFont);
    saveLoadButton = new wxButton(this, ID_SAVE_LOAD, "Save/Load game", wxDefaultPosition, wxSize(350, 80));
    saveLoadButton->SetFont(buttonFont);
    optionsButton = new wxButton(this, ID_OPTIONS, "Options", wxDefaultPosition, wxSize(350, 80));
    optionsButton->SetFont(buttonFont);
    exitButton = new wxButton(this, ID_EXIT, "Exit", wxDefaultPosition, wxSize(350, 80));
    exitButton->SetFont(buttonFont);
    
    // Style buttons
    continueButton->SetBackgroundColour(wxColor(60, 60, 60));
    continueButton->SetForegroundColour(wxColor(*wxWHITE));
    newGameButton->SetBackgroundColour(wxColor(60, 60, 60));
    newGameButton->SetForegroundColour(wxColor(*wxWHITE));
    saveLoadButton->SetBackgroundColour(wxColor(60, 60, 60));
    saveLoadButton->SetForegroundColour(wxColor(*wxWHITE));
    optionsButton->SetBackgroundColour(wxColor(60, 60, 60));
    optionsButton->SetForegroundColour(wxColor(*wxWHITE));
    exitButton->SetBackgroundColour(wxColor(60, 60, 60));
    exitButton->SetForegroundColour(wxColor(*wxWHITE));
    
    // Bind button events
    continueButton->Bind(wxEVT_BUTTON, &cMenu::OnContinueClicked, this);
    newGameButton->Bind(wxEVT_BUTTON, &cMenu::OnNewGameClicked, this);
    saveLoadButton->Bind(wxEVT_BUTTON, &cMenu::OnSaveLoadClicked, this);
    optionsButton->Bind(wxEVT_BUTTON, &cMenu::OnOptionsClicked, this);
    exitButton->Bind(wxEVT_BUTTON, &cMenu::OnExitClicked, this);
    
    // Set up layout
    wxBoxSizer* menuSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
    
    menuSizer->Add(title, wxSizerFlags().CenterHorizontal().Border(wxALL, 30));
    
    buttonSizer->Add(continueButton, wxSizerFlags().CenterHorizontal().Border(wxALL, 20));
    buttonSizer->Add(newGameButton, wxSizerFlags().CenterHorizontal().Border(wxALL, 20));
    buttonSizer->Add(saveLoadButton, wxSizerFlags().CenterHorizontal().Border(wxALL, 20));
    buttonSizer->Add(optionsButton, wxSizerFlags().CenterHorizontal().Border(wxALL, 20));
    buttonSizer->Add(exitButton, wxSizerFlags().CenterHorizontal().Border(wxALL, 20));
    
    menuSizer->Add(buttonSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, 20));
    menuSizer->AddStretchSpacer();
    
    this->SetSizer(menuSizer);
    menuSizer->SetSizeHints(this);
}

cMenu::~cMenu()
{
}

// TODO: Implement continue functionality
void cMenu::OnContinueClicked(wxCommandEvent& evt)
{
}

// TODO: Implement new game functionality
void cMenu::OnNewGameClicked(wxCommandEvent& evt)
{
}

// TODO: Implement save/load game functionality
void cMenu::OnSaveLoadClicked(wxCommandEvent& evt)
{
}

// TODO: Implement options functionality
void cMenu::OnOptionsClicked(wxCommandEvent& evt)
{
}

void cMenu::OnExitClicked(wxCommandEvent& evt)
{
    wxWindow* topWindow = this;
    while (topWindow->GetParent())
    {
        topWindow = topWindow->GetParent();
    }
    topWindow->Close();
}