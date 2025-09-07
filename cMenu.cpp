#include "cMenu.h"

cMenu::cMenu(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
    SetBackgroundColour(wxColor(20, 20, 20));
    
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "Let's play WORDLE!", wxDefaultPosition, wxDefaultSize);
    title->SetBackgroundColour(wxColor(20, 20, 20));
    title->SetForegroundColour(wxColor(*wxWHITE));
    title->SetFont(wxFont(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));
    
    wxFont buttonFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    continueButton = new wxButton(this, ID_CONTINUE, "Continue", wxDefaultPosition, wxSize(350, 80), wxBORDER_NONE);
    continueButton->SetFont(buttonFont);
    newGameButton = new wxButton(this, ID_NEW_GAME, "New Game", wxDefaultPosition, wxSize(350, 80), wxBORDER_NONE);
    newGameButton->SetFont(buttonFont);
    saveLoadButton = new wxButton(this, ID_SAVE_LOAD, "Save/Load game", wxDefaultPosition, wxSize(350, 80), wxBORDER_NONE);
    saveLoadButton->SetFont(buttonFont);
    optionsButton = new wxButton(this, ID_OPTIONS, "Options", wxDefaultPosition, wxSize(350, 80), wxBORDER_NONE);
    optionsButton->SetFont(buttonFont);
    exitButton = new wxButton(this, ID_EXIT, "Exit", wxDefaultPosition, wxSize(350, 80), wxBORDER_NONE);
    exitButton->SetFont(buttonFont);
    
    // Style buttons
    continueButton->SetBackgroundColour(wxColor(60, 60, 60));
    continueButton->SetForegroundColour(wxColor(*wxWHITE));
    continueButton->SetCursor(wxCursor(wxCURSOR_HAND));
    newGameButton->SetBackgroundColour(wxColor(60, 60, 60));
    newGameButton->SetForegroundColour(wxColor(*wxWHITE));
    newGameButton->SetCursor(wxCursor(wxCURSOR_HAND));
    saveLoadButton->SetBackgroundColour(wxColor(60, 60, 60));
    saveLoadButton->SetForegroundColour(wxColor(*wxWHITE));
    saveLoadButton->SetCursor(wxCursor(wxCURSOR_HAND));
    optionsButton->SetBackgroundColour(wxColor(60, 60, 60));
    optionsButton->SetForegroundColour(wxColor(*wxWHITE));
    optionsButton->SetCursor(wxCursor(wxCURSOR_HAND));
    exitButton->SetBackgroundColour(wxColor(60, 60, 60));
    exitButton->SetForegroundColour(wxColor(*wxWHITE));
    exitButton->SetCursor(wxCursor(wxCURSOR_HAND));
    
    // Bind button events
    continueButton->Bind(wxEVT_BUTTON, &cMenu::OnContinueClicked, this);
    newGameButton->Bind(wxEVT_BUTTON, &cMenu::OnNewGameClicked, this);
    saveLoadButton->Bind(wxEVT_BUTTON, &cMenu::OnSaveLoadClicked, this);
    optionsButton->Bind(wxEVT_BUTTON, &cMenu::OnOptionsClicked, this);
    exitButton->Bind(wxEVT_BUTTON, &cMenu::OnExitClicked, this);

    // Bind mouse events
    continueButton->Bind(wxEVT_ENTER_WINDOW, &cMenu::OnButtonEnter, this);
    continueButton->Bind(wxEVT_LEAVE_WINDOW, &cMenu::OnButtonLeave, this);
    newGameButton->Bind(wxEVT_ENTER_WINDOW, &cMenu::OnButtonEnter, this);
    newGameButton->Bind(wxEVT_LEAVE_WINDOW, &cMenu::OnButtonLeave, this);
    saveLoadButton->Bind(wxEVT_ENTER_WINDOW, &cMenu::OnButtonEnter, this);
    saveLoadButton->Bind(wxEVT_LEAVE_WINDOW, &cMenu::OnButtonLeave, this);
    optionsButton->Bind(wxEVT_ENTER_WINDOW, &cMenu::OnButtonEnter, this);
    optionsButton->Bind(wxEVT_LEAVE_WINDOW, &cMenu::OnButtonLeave, this);
    exitButton->Bind(wxEVT_ENTER_WINDOW, &cMenu::OnButtonEnter, this);
    exitButton->Bind(wxEVT_LEAVE_WINDOW, &cMenu::OnButtonLeave, this);
    
    // Bind focus events
    continueButton->Bind(wxEVT_SET_FOCUS, &cMenu::OnButtonSetFocus, this);
    continueButton->Bind(wxEVT_KILL_FOCUS, &cMenu::OnButtonKillFocus, this);
    newGameButton->Bind(wxEVT_SET_FOCUS, &cMenu::OnButtonSetFocus, this);
    newGameButton->Bind(wxEVT_KILL_FOCUS, &cMenu::OnButtonKillFocus, this);
    saveLoadButton->Bind(wxEVT_SET_FOCUS, &cMenu::OnButtonSetFocus, this);
    saveLoadButton->Bind(wxEVT_KILL_FOCUS, &cMenu::OnButtonKillFocus, this);
    optionsButton->Bind(wxEVT_SET_FOCUS, &cMenu::OnButtonSetFocus, this);
    optionsButton->Bind(wxEVT_KILL_FOCUS, &cMenu::OnButtonKillFocus, this);
    exitButton->Bind(wxEVT_SET_FOCUS, &cMenu::OnButtonSetFocus, this);
    exitButton->Bind(wxEVT_KILL_FOCUS, &cMenu::OnButtonKillFocus, this);
    
    // Set up layout
    wxBoxSizer* menuSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxVERTICAL);
    
    menuSizer->Add(title, wxSizerFlags().CenterHorizontal().Border(wxALL, 40));
    
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

void cMenu::OnNewGameClicked(wxCommandEvent& evt)
{
    wxCommandEvent switchEvent(wxEVT_SWITCH_TO_GAME);
    switchEvent.SetEventObject(this);
    
    wxWindow* parent = GetParent();
    while (parent && !parent->IsTopLevel())
        parent = parent->GetParent();
    
    if (parent)
        wxPostEvent(parent, switchEvent);
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
        topWindow = topWindow->GetParent();

    topWindow->Close();
}

void cMenu::OnButtonEnter(wxMouseEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());

    // Set to mouse highlight color
    button->SetBackgroundColour(wxColor(129, 131, 132));
    button->Refresh();
    evt.Skip();
}

void cMenu::OnButtonLeave(wxMouseEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());

    // Reset to the original background color
    button->SetBackgroundColour(wxColor(58, 58, 60));
    button->Refresh();
    evt.Skip();
}

void cMenu::OnButtonSetFocus(wxFocusEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());
    
    // Set to tab highlight color
    button->SetBackgroundColour(wxColor(86, 87, 88));
    button->Refresh();
    evt.Skip();
}

void cMenu::OnButtonKillFocus(wxFocusEvent& evt)
{
    wxButton* button = static_cast<wxButton*>(evt.GetEventObject());
    
    // Reset to the original background color
    button->SetBackgroundColour(wxColor(58, 58, 60));
    button->Refresh();
    evt.Skip();
}