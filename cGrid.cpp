#include "cGrid.h"
#include "NinePatchScaler.h"
#include "UIScaler.h"

cGrid::cGrid(int width, int height)
{
	nFieldWidth = width;
	nFieldHeight = height;

	gridButton.resize(nFieldHeight * nFieldWidth);
	gridText.resize(nFieldHeight * nFieldWidth);

	// Load and scale bitmaps based on current resolution
	UIScaler& uiScaler = UIScaler::GetInstance();
	NinePatchScaler& ninePatchScaler = NinePatchScaler::GetInstance();
	
	// Base size for grid cells
	wxSize baseGridSize(64, 64);
	
	// Calculate scaled size
	wxSize scaledGridSize = uiScaler.ScaledSize(baseGridSize.GetWidth(), baseGridSize.GetHeight());
	
	// Load scaled bitmaps using nine-patch scaling
    int borderSize = 4;
	bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_UNMARKED"), baseGridSize, scaledGridSize, borderSize));
	bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_MARKED"), baseGridSize, scaledGridSize, borderSize));
	bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_COLD"), baseGridSize, scaledGridSize, borderSize));
	bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_WARM"), baseGridSize, scaledGridSize, borderSize));
	bitmapsGrid.push_back(ninePatchScaler.GetScaledBitmap(wxT("IDB_HOT"), baseGridSize, scaledGridSize, borderSize));
}

wxGridSizer* cGrid::CreateGrid(wxWindow* parent)
{
	wxGridSizer* grid = new wxGridSizer(nFieldHeight, nFieldWidth, wxSize(0, 0));
	
	UIScaler& scaler = UIScaler::GetInstance();
	wxFont gridFont(scaler.ScaledFontSize(22), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    int gridGap = scaler.ScaledValue(2);

	for (int x = 0; x < nFieldHeight; x++)
	{
		for (int y = 0; y < nFieldWidth; y++)
		{
			int index = x * nFieldWidth + y;
			
			// Create letter box
			gridButton[index] = new wxButton(parent, 10000 + index, wxEmptyString, wxDefaultPosition, wxSize(bitmapsGrid.at(0).GetWidth(), bitmapsGrid.at(0).GetHeight()), wxBORDER_NONE | wxBU_NOTEXT);
			gridButton[index]->SetBitmap(bitmapsGrid.at(0));

			// Create label for letter box
			gridText[index] = new wxStaticText(gridButton[index], wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
			gridText[index]->SetBackgroundColour(wxColor(18, 18, 19));
			gridText[index]->SetForegroundColour(wxColor(*wxWHITE));
			gridText[index]->SetFont(gridFont);
			gridText[index]->CenterOnParent();

			// Add Box with label to GridSizer
			grid->Add(gridButton[index], 1, wxALL, gridGap);
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

void cGrid::UpdateActiveRowCells(int row)
{
    if (row < 0 || row >= nFieldHeight)
        return;

    for (int col = 0; col < nFieldWidth; ++col)
    {
        int index = row * nFieldWidth + col;
        if (!gridText[index]->GetLabel().IsEmpty())
            gridButton[index]->SetBitmap(bitmapsGrid.at(1));
    }
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

void cGrid::ResetGrid()
{
    for (int x = 0; x < nFieldHeight; x++)
    {
        for (int y = 0; y < nFieldWidth; y++)
        {
            int index = x * nFieldWidth + y;
            
            gridText[index]->SetLabel(wxEmptyString);
            gridText[index]->SetBackgroundColour(wxColor(18, 18, 19));
            gridButton[index]->SetBitmap(bitmapsGrid.at(0)); // IDB_UNMARKED
            
            gridText[index]->Refresh();
            gridButton[index]->Refresh();
        }
    }
}

cGrid::~cGrid()
{
}
