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
	title->SetFont(wxFont (22, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

	//Create Grid

	cGrid grid(5, 6);
	gridSizer->Add(grid.CreateGrid(panel));

	//Create keyboard Grid

	int selectedKeyboard = 1;

	switch (selectedKeyboard)
	{
		case 1:
		{
			cKeyboardENG keyboard;
			keyboardSizer->Add(keyboard.CreateKeyboard(panel), wxSizerFlags().CenterHorizontal().Border(wxALL, 2));
			break;
		}

		default:
			break;
	}

	//Set Sizers

	gameSizer->Add(title, wxSizerFlags().CenterHorizontal().Border(wxALL, 25));
	gameSizer->Add(gridSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, 10));
	gameSizer->Add(keyboardSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, 25));
	gameSizer->AddStretchSpacer();
	panel->SetSizer(gameSizer);
	gameSizer->SetSizeHints(this);
}

cMain::~cMain()
{
}
