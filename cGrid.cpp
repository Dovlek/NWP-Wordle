#include "cGrid.h"

cGrid::cGrid(int width, int height)
{
	nFieldWidth = width;
	nFieldHeight = height;

	gridButton = new wxButton * [nFieldHeight * nFieldWidth];
	gridText = new wxStaticText * [nFieldHeight * nFieldWidth];

	bitmapsGrid.push_back(wxBitmap(wxT("IDB_UNMARKED"), wxBITMAP_TYPE_BMP_RESOURCE));
	bitmapsGrid.push_back(wxBitmap(wxT("IDB_MARKED"), wxBITMAP_TYPE_BMP_RESOURCE));
	bitmapsGrid.push_back(wxBitmap(wxT("IDB_COLD"), wxBITMAP_TYPE_BMP_RESOURCE));
	bitmapsGrid.push_back(wxBitmap(wxT("IDB_WARM"), wxBITMAP_TYPE_BMP_RESOURCE));
	bitmapsGrid.push_back(wxBitmap(wxT("IDB_HOT"), wxBITMAP_TYPE_BMP_RESOURCE));
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

wxString cGrid::GetLetter(int row, int col) const
{
    if (!IsValidPosition(row, col))
        return wxEmptyString;
    
    int index = row * nFieldWidth + col;

    return gridText[index]->GetLabel();
}

bool cGrid::IsValidPosition(int row, int col) const
{
    return row >= 0 && row < nFieldHeight && col >= 0 && col < nFieldWidth;
}

void cGrid::UpdateActiveCell(int prevRow, int prevCol, int currRow, int currCol, bool forward)
{
    if (IsValidPosition(prevRow, prevCol) && forward)
        gridButton[prevRow * nFieldWidth + prevCol]->SetBitmap(bitmapsGrid.at(1));
    if (IsValidPosition(currRow, currCol) && !forward)
        gridButton[currRow * nFieldWidth + currCol]->SetBitmap(bitmapsGrid.at(0));
}

void cGrid::UpdateCellColors(int row, const std::vector<int>& states)
{
    for (int col = 0; col < nFieldWidth; ++col)
    {
        int index = row * nFieldWidth + col;
        int bitmapIndex = 0;
        wxColor textBackgroundColor = wxColor(18, 18, 19);
        
        switch (states[col])
        {
        case 0: // WRONG
            bitmapIndex = 2; // IDB_COLD
            textBackgroundColor = wxColor(58, 58, 60);
            break;
        case 1: // WRONG_POSITION  
            bitmapIndex = 3; // IDB_WARM
            textBackgroundColor = wxColor(181, 159, 59);
            break;
        case 2: // CORRECT
            bitmapIndex = 4; // IDB_HOT
            textBackgroundColor = wxColor(83, 141, 78);
            break;
        default:
            break;
        }
        
        gridButton[index]->SetBitmap(bitmapsGrid.at(bitmapIndex));
        gridText[index]->SetBackgroundColour(textBackgroundColor);
        gridText[index]->Refresh();
    }
}

cGrid::~cGrid()
{
	delete[]gridButton;
	delete[]gridText;
}
