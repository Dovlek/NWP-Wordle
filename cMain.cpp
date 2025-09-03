#include "cMain.h"

cMain::cMain() : wxFrame(nullptr, wxID_ANY, "NWP - Wordle", wxDefaultPosition, wxSize(800, 600))
{
	wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS);
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
	panel->SetFocus();
	
	// Bind keyboard events to both panel and frame
	panel->Bind(wxEVT_CHAR_HOOK, &cMain::OnKeyboardButtonPressed, this);
	this->Bind(wxEVT_CHAR_HOOK, &cMain::OnKeyboardButtonPressed, this);
}

// TODO: Fix Enter key not bypassing button focus
void cMain::OnKeyboardButtonPressed(wxKeyEvent& evt)
{
    int keyCode = evt.GetKeyCode();
    wxString key;

    if (keyCode >= 'A' && keyCode <= 'Z')
        key = wxString(static_cast<char>(keyCode));
    else if (keyCode >= 'a' && keyCode <= 'z')
        key = wxString(static_cast<char>(keyCode - 32));
    else if (keyCode == WXK_RETURN || keyCode == WXK_NUMPAD_ENTER || keyCode == 13)
        key = "ENTER";
    else if (keyCode == WXK_BACK)
        key = "backspace";

    if (!key.IsEmpty())
    {
        ProcessKey(key);
        this->SetFocus();
    }   
    else
        evt.Skip();
}

void cMain::OnKeyboardButtonClicked(wxCommandEvent& evt)
{
    wxButton* button = wxDynamicCast(evt.GetEventObject(), wxButton);
    if (!button)
        return;

    wxString letter;
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

    ProcessKey(letter);
    this->SetFocus();
    evt.Skip();
}

void cMain::ProcessKey(const wxString& key)
{
    if (key == "ENTER")
    {
        if (currentCol == cgrid->GetWidth() && currentRow < cgrid->GetHeight() - 1)
        {
            prevRow = currentRow;
            prevCol = currentCol - 1;
            currentRow++;
            currentCol = 0;
        }
    }
    else if (key == "backspace")
    {
        if (currentCol > 0)
        {
            prevRow = currentRow;
            prevCol = currentCol;
            currentCol--;
            cgrid->UpdateActiveCell(prevRow, prevCol, currentRow, currentCol, false);
            cgrid->SetLetter(currentRow, currentCol, "");
        }
    }
    else if (key.Length() == 1 && currentCol < cgrid->GetWidth() && currentRow < cgrid->GetHeight())
    {
        cgrid->SetLetter(currentRow, currentCol, key.Upper());
        prevRow = currentRow;
        prevCol = currentCol;
        currentCol++;
        cgrid->UpdateActiveCell(prevRow, prevCol, currentRow, currentCol, true);
    }
}

cMain::~cMain()
{
    delete cgrid;
    delete ckeyboard_eng;
}
