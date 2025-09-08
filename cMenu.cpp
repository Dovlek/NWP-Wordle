#include "cMenu.h"

cMenu::cMenu(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
    SetBackgroundColour(wxColor(20, 20, 20));
    
    wxStaticText* title = new wxStaticText(this, wxID_ANY, "Let's play WORDLE!", wxDefaultPosition, wxDefaultSize);
    title->SetBackgroundColour(wxColor(20, 20, 20));
    title->SetForegroundColour(wxColor(*wxWHITE));
    title->SetFont(wxFont(24, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));
    
    continueButton = new wxButton(this, ID_CONTINUE, "Continue", wxDefaultPosition, wxSize(350, 80), wxBORDER_NONE);
    newGameButton = new wxButton(this, ID_NEW_GAME, "New Game", wxDefaultPosition, wxSize(350, 80), wxBORDER_NONE);
    saveLoadButton = new wxButton(this, ID_SAVE_LOAD, "Save/Load game", wxDefaultPosition, wxSize(350, 80), wxBORDER_NONE);
    optionsButton = new wxButton(this, ID_OPTIONS, "Options", wxDefaultPosition, wxSize(350, 80), wxBORDER_NONE);
    exitButton = new wxButton(this, ID_EXIT, "Exit", wxDefaultPosition, wxSize(350, 80), wxBORDER_NONE);
    
    // Style buttons
    wxFont buttonFont(18, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    for (int buttonId = ID_CONTINUE; buttonId <= ID_EXIT; buttonId++)
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

    // Bind button events
    continueButton->Bind(wxEVT_BUTTON, &cMenu::OnContinueClicked, this);
    newGameButton->Bind(wxEVT_BUTTON, &cMenu::OnNewGameClicked, this);
    saveLoadButton->Bind(wxEVT_BUTTON, &cMenu::OnSaveLoadClicked, this);
    optionsButton->Bind(wxEVT_BUTTON, &cMenu::OnOptionsClicked, this);
    exitButton->Bind(wxEVT_BUTTON, &cMenu::OnExitClicked, this);
    
    // Bind mouse events
    for (int buttonId = ID_CONTINUE; buttonId <= ID_EXIT; buttonId++)
    {
        wxButton* button = GetButtonById(buttonId);
        if (button)
        {
            button->Bind(wxEVT_ENTER_WINDOW, &cMenu::OnButtonEnter, this);
            button->Bind(wxEVT_LEAVE_WINDOW, &cMenu::OnButtonLeave, this);
        }
    }
    
    // Bind focus events
    for (int buttonId = ID_CONTINUE; buttonId <= ID_EXIT; buttonId++)
    {
        wxButton* button = GetButtonById(buttonId);
        if (button)
        {
            button->Bind(wxEVT_SET_FOCUS, &cMenu::OnButtonSetFocus, this);
            button->Bind(wxEVT_KILL_FOCUS, &cMenu::OnButtonKillFocus, this);
        }
    }
    
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
    wxCommandEvent switchEvent(wxEVT_START_NEW_GAME);
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

wxButton* cMenu::GetButtonById(int id)
{
    switch (id)
    {
    case ID_CONTINUE: 
        return continueButton;
    case ID_NEW_GAME:
        return newGameButton;
    case ID_SAVE_LOAD:
        return saveLoadButton;
    case ID_OPTIONS:
        return optionsButton;
    case ID_EXIT:
        return exitButton;
    default:
        return nullptr;
    }
}