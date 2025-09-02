#include "cGrid.h"

cGrid::cGrid(int width, int height)
{
	nFieldWidth = width;
	nFieldHeight = height;

	gridButton = new wxButton * [nFieldHeight * nFieldWidth];
	gridText = new wxStaticText * [nFieldHeight * nFieldWidth];

	bitmapsGrid.push_back(wxBitmap("Resources/Grid/Unmarked.bmp", wxBITMAP_TYPE_BMP));
	bitmapsGrid.push_back(wxBitmap("Resources/Grid/Marked.bmp", wxBITMAP_TYPE_BMP));
	bitmapsGrid.push_back(wxBitmap("Resources/Grid/Cold.bmp", wxBITMAP_TYPE_BMP));
	bitmapsGrid.push_back(wxBitmap("Resources/Grid/Warm.bmp", wxBITMAP_TYPE_BMP));
	bitmapsGrid.push_back(wxBitmap("Resources/Grid/Hot.bmp", wxBITMAP_TYPE_BMP));
}

wxGridSizer* cGrid::CreateGrid(wxWindow* parent)
{
	wxGridSizer* grid = new wxGridSizer(nFieldHeight, nFieldWidth, wxSize(0, 0));
	wxFont gridFont(22, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

	for (int x = 0; x < nFieldHeight; x++)
	{
		for (int y = 0; y < nFieldWidth; y++)
		{
			// Create letter box
			gridButton[x * nFieldWidth + y] = new wxButton(parent, 10000 + (x * nFieldWidth + y), wxEmptyString, wxDefaultPosition, wxSize(bitmapsGrid.at(0).GetWidth(), bitmapsGrid.at(0).GetHeight()), wxBORDER_NONE | wxBU_NOTEXT);
			gridButton[x * nFieldWidth + y]->SetBitmap(bitmapsGrid.at(0));

			// Create label for letter box
			gridText[x * nFieldWidth + y] = new wxStaticText(gridButton[x * nFieldWidth + y], wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
			gridText[x * nFieldWidth + y]->SetBackgroundColour(wxColor(18, 18, 19));
			gridText[x * nFieldWidth + y]->SetForegroundColour(wxColor(*wxWHITE));
			gridText[x * nFieldWidth + y]->SetFont(gridFont);
			gridText[x * nFieldWidth + y]->CenterOnParent();

			// Add Box with label to GridSizer
			grid->Add(gridButton[x * nFieldWidth + y], 1, wxALL, 2);
		}
	}

	return grid;
}

void cGrid::SetLetter(int row, int col, const wxString& letter)
{
    if (!IsValidPosition(row, col))
        return;

    int index = row * nFieldWidth + col;
	gridText[index]->SetLabel(letter);
    gridText[index]->CenterOnParent();
}

bool cGrid::IsValidPosition(int row, int col) const
{
    return row >= 0 && row < nFieldHeight && col >= 0 && col < nFieldWidth;
}

cGrid::~cGrid()
{
	delete[]gridButton;
	delete[]gridText;
}
