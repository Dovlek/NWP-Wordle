#include "cMain.h"

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "NWP - Wordle", wxDefaultPosition, wxSize(800, 600))
{
    wxPanel* mainPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);
    mainPanel->SetBackgroundColour(wxColor(20, 20, 20));
    
    cSimplebook = new wxSimplebook(mainPanel, wxID_ANY);
    
    // Create and add the Wordle game panel as the first page
    wordlePanel = new cWordle(cSimplebook);
    cSimplebook->AddPage(wordlePanel, "Wordle", true);
    
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
