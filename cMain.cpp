#include "cMain.h"
#include "cKeyboard.h"
#include <wx/gbsizer.h>

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "NWP - Wordle", wxDefaultPosition, wxSize(800, 600))
{
	wxPanel* panel = new wxPanel(this);
	panel->SetBackgroundColour(wxColor(20, 20, 20));

	gridButton = new wxButton*[nFieldHeight * nFieldWidth];
	gridText = new wxStaticText*[nFieldHeight * nFieldWidth];

	wxBoxSizer* gameSizer = new wxBoxSizer(wxVERTICAL);
	wxGridSizer* gridSizer = new wxGridSizer(nFieldHeight, nFieldWidth, wxSize(0, 0));
	wxBoxSizer* keyboardSizer = new wxBoxSizer(wxVERTICAL);

	wxVector<wxBitmap> bitmaps;
	bitmaps.push_back(wxBitmap("Resources/Grid/Unmarked.bmp", wxBITMAP_TYPE_BMP));
	bitmaps.push_back(wxBitmap("Resources/Grid/Marked.bmp", wxBITMAP_TYPE_BMP));
	bitmaps.push_back(wxBitmap("Resources/Grid/Cold.bmp", wxBITMAP_TYPE_BMP));
	bitmaps.push_back(wxBitmap("Resources/Grid/Warm.bmp", wxBITMAP_TYPE_BMP));
	bitmaps.push_back(wxBitmap("Resources/Grid/Hot.bmp", wxBITMAP_TYPE_BMP));
	bitmaps.push_back(wxBitmap("Resources/Keys/Unused.bmp", wxBITMAP_TYPE_BMP));
	bitmaps.push_back(wxBitmap("Resources/Keys/Used.bmp", wxBITMAP_TYPE_BMP));
	bitmaps.push_back(wxBitmap("Resources/Keys/Correct.bmp", wxBITMAP_TYPE_BMP));
	bitmaps.push_back(wxBitmap("Resources/Keys/Almost.bmp", wxBITMAP_TYPE_BMP));
	bitmaps.push_back(wxBitmap("Resources/Keys/BigButton.bmp", wxBITMAP_TYPE_BMP));

	wxFont gridFont(22, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	//Create guess Grid
	for (int x = 0; x < nFieldHeight; x++)
	{
		for (int y = 0; y < nFieldWidth; y++)
		{
			// Create letter box
			gridButton[x * nFieldWidth + y] = new wxButton(panel, 10000 + (x * nFieldWidth + y), wxEmptyString, wxDefaultPosition, wxSize(bitmaps.at(0).GetWidth(), bitmaps.at(0).GetHeight()), wxBORDER_NONE | wxBU_NOTEXT);
			gridButton[x * nFieldWidth + y]->SetBitmap(bitmaps.at(0));

			// Create label for letter box
			gridText[x * nFieldWidth + y] = new wxStaticText(gridButton[x * nFieldWidth + y], wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
			gridText[x * nFieldWidth + y]->SetBackgroundColour(wxColor(18, 18, 19));
			gridText[x * nFieldWidth + y]->SetForegroundColour(wxColor(*wxWHITE));
			gridText[x * nFieldWidth + y]->SetFont(gridFont);
			gridText[x * nFieldWidth + y]->CenterOnParent();
			
			// Add Box with label to GridSizer
			gridSizer->Add(gridButton[x * nFieldWidth + y], 1, wxALL, 2);
		}
	}

	wxStaticText* title = new wxStaticText(panel, wxID_ANY, "WORDLE", wxDefaultPosition, wxDefaultSize);

	title->SetBackgroundColour(wxColor(20, 20, 20));
	title->SetForegroundColour(wxColor(*wxWHITE));
	title->SetFont(gridFont);

	//Create keyboard Grid

	int selectedKeyboard = 1;

	switch (selectedKeyboard)
	{
		case 1:
		{
			cKeyboardENG keyo;
			keyboardSizer->Add(keyo.CreateKeyboard(panel), wxSizerFlags().CenterHorizontal().Border(wxALL, 2));
			break;
		}

		default:
			break;
	}

	gameSizer->Add(title, wxSizerFlags().CenterHorizontal().Border(wxALL, 25));
	gameSizer->Add(gridSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, 10));
	gameSizer->Add(keyboardSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, 25));
	gameSizer->AddStretchSpacer();
	panel->SetSizer(gameSizer);
	gameSizer->SetSizeHints(this);
}

cMain::~cMain()
{
	delete[]gridButton;
	delete[]gridText;
}
