#include "cMain.h"

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "NWP - Wordle", wxDefaultPosition, wxSize(800, 600))
{
	wxPanel* panel = new wxPanel(this);
	panel->SetBackgroundColour(wxColor(20, 20, 20));

	wxBoxSizer* gameSizer = new wxBoxSizer(wxVERTICAL);
	wxGridSizer* gridSizer = new wxGridSizer(0, 0, wxSize(0, 0));
	wxBoxSizer* keyboardSizer = new wxBoxSizer(wxVERTICAL);

	wxStaticText* title = new wxStaticText(panel, wxID_ANY, "WORDLE", wxDefaultPosition, wxDefaultSize);
	title->SetBackgroundColour(wxColor(20, 20, 20));
	title->SetForegroundColour(wxColor(*wxWHITE));
	title->SetFont(wxFont(22, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

	// Create Grid
	cgrid = new cGrid(5, 6);
	gridSizer->Add(cgrid->CreateGrid(panel));

	// Create keyboard Grid
	ckeyboard_eng = new cKeyboardENG();
    wxBoxSizer* keyboardPanel = ckeyboard_eng->CreateKeyboard(panel);
	
	// Bind keyboard buttons
	for (int i = 0; i < ckeyboard_eng->keyboardSize + 2; i++)
	{
		ckeyboard_eng->gridKey[i]->Bind(wxEVT_BUTTON, &cMain::OnKeyboardButtonClicked, this);
	}

    keyboardSizer->Add(keyboardPanel, wxSizerFlags().CenterHorizontal().Border(wxALL, 2));

	// Set Sizers
	gameSizer->Add(title, wxSizerFlags().CenterHorizontal().Border(wxALL, 25));
	gameSizer->Add(gridSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, 10));
	gameSizer->Add(keyboardSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, 25));
	gameSizer->AddStretchSpacer();
	panel->SetSizer(gameSizer);
	gameSizer->SetSizeHints(this);
}

void cMain::OnKeyboardButtonClicked(wxCommandEvent& evt)
{
    wxButton* button = wxDynamicCast(evt.GetEventObject(), wxButton);
    if (!button) return;

    wxString letter;

    // Find the child wxStaticText and get its label
    const wxWindowList& children = button->GetChildren();
    for (wxWindowList::const_iterator it = children.begin(); it != children.end(); ++it)
    {
        wxStaticText* labelCtrl = dynamic_cast<wxStaticText*>(*it);
        if (labelCtrl)
        {
            letter = labelCtrl->GetLabel();
            break;
        }
    }

    //wxMessageBox("You clicked: " + letter);

    // Handle special keys
    if (letter == "ENTER")
    {
        if (currentCol == cgrid->GetWidth())
        {
            currentRow++;
            currentCol = 0;
        }
    }
    else if (letter == "backspace")
    {
        if (currentCol > 0) {
            currentCol--;
            cgrid->SetLetter(currentRow, currentCol, "");
        }
    }
    else if (currentCol < cgrid->GetWidth() && currentRow < cgrid->GetHeight())
    {
        cgrid->SetLetter(currentRow, currentCol, letter);
        currentCol++;
    }

    evt.Skip();
}

cMain::~cMain()
{
    delete cgrid;
    delete ckeyboard_eng;
}
