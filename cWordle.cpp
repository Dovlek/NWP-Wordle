#include "cWordle.h"

cWordle::cWordle(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS)
{
    SetBackgroundColour(wxColor(20, 20, 20));
    
    gameSizer = new wxBoxSizer(wxVERTICAL);
    wxGridSizer* gridSizer = new wxGridSizer(0, 0, wxSize(0, 0));

    wxStaticText* title = new wxStaticText(this, wxID_ANY, "WORDLE", wxDefaultPosition, wxDefaultSize);
    title->SetBackgroundColour(wxColor(20, 20, 20));
    title->SetForegroundColour(wxColor(*wxWHITE));
    title->SetFont(wxFont(22, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

    statusMessage = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    statusMessage->SetBackgroundColour(wxColor(20, 20, 20));
    statusMessage->SetForegroundColour(wxColor(*wxWHITE));
    statusMessage->SetFont(wxFont(16, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

    // Create Grid
    cgrid = new cGrid(5, 6);
    gridSizer->Add(cgrid->CreateGrid(this));

    // Create keyboard Grid
    ckeyboard_eng = new cKeyboardENG();
    wxBoxSizer* keyboardPanel = ckeyboard_eng->CreateKeyboard(this);
    
    // Bind keyboard buttons
    for (int i = 0; i < ckeyboard_eng->keyboardSize + 2; i++)
    {
        ckeyboard_eng->gridKey[i]->Bind(wxEVT_BUTTON, &cWordle::OnKeyboardButtonClicked, this);
    }

    // Set Sizers
    gameSizer->Add(title, wxSizerFlags().CenterHorizontal().Border(wxALL, 25));
    gameSizer->Add(statusMessage, wxSizerFlags().CenterHorizontal().Border(wxALL, 2));
    gameSizer->Add(gridSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, 10));
    gameSizer->Add(keyboardPanel, wxSizerFlags().CenterHorizontal().Border(wxALL, 25));
    gameSizer->AddStretchSpacer();
    this->SetSizer(gameSizer);
    gameSizer->SetSizeHints(this);

    // Bind keyboard events
    Bind(wxEVT_CHAR_HOOK, &cWordle::OnKeyboardButtonPressed, this);

    wordSelector = new WordSelector();
    targetWord = wordSelector->GetRandomWord();
    wxLogMessage("Target word: %s", targetWord);

    statusTimer = new wxTimer(this);
    Bind(wxEVT_TIMER, &cWordle::HideStatusMessage, this);
}

// TODO: Fix Enter key not bypassing button focus
void cWordle::OnKeyboardButtonPressed(wxKeyEvent& evt)
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
        SetFocus();
    }   
    else
        evt.Skip();
}

void cWordle::OnKeyboardButtonClicked(wxCommandEvent& evt)
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
    SetFocus();
    evt.Skip();
}

void cWordle::ProcessKey(const wxString& key)
{
    if (key == "ENTER")
    {
        if (currentCol == cgrid->GetWidth() && currentRow < cgrid->GetHeight())
        {
            wxString currentGuess = "";
            for (int i = 0; i < cgrid->GetWidth(); i++)
            {
                currentGuess += cgrid->GetLetter(currentRow, i);
            }
            
            if (!wordSelector->IsValidWord(currentGuess))
            {
                ShowStatusMessage("Not in word list", wxColor(255, 100, 100));
                return;
            }
            
            CheckGuess(currentGuess, currentRow);
            prevRow = currentRow;
            prevCol = currentCol;
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

std::vector<cWordle::LetterState> cWordle::CompareWords(const wxString& guess, const wxString& target)
{
    std::vector<LetterState> states(5, LetterState::WRONG);
    wxString targetCopy = target;
    wxString guessCopy = guess;
    
    // First pass: mark correct positions
    for (int i = 0; i < 5; i++)
    {
        if (guessCopy[i] == targetCopy[i])
        {
            states[i] = LetterState::CORRECT;
            targetCopy[i] = '*';
            guessCopy[i] = '*';
        }
    }
    
    // Second pass: mark wrong positions
    for (int i = 0; i < 5; i++)
    {
        if (states[i] == LetterState::WRONG && guessCopy[i] != '*')
        {
            int pos = targetCopy.Find(guessCopy[i]);
            if (pos != wxNOT_FOUND)
            {
                states[i] = LetterState::WRONG_POSITION;
                targetCopy[pos] = '*';
            }
        }
    }
    
    return states;
}

void cWordle::CheckGuess(const wxString& guess, int row)
{
    std::vector<LetterState> states = CompareWords(guess, targetWord);
    
    std::vector<int> intStates;
    for (const auto& state : states)
    {
        intStates.push_back(static_cast<int>(state));
    }
    
    cgrid->UpdateCellColors(row, intStates);
    ckeyboard_eng->UpdateKeyboardColors(guess, intStates);
    
    bool allCorrect = true;
    for (const auto& state : states)
    {
        if (state != LetterState::CORRECT)
        {
            allCorrect = false;
            break;
        }
    }
    
    if (allCorrect)
    {
        ShowStatusMessage("Congratulations! You won!", wxColor(100, 255, 100));
    }
    else if (currentRow >= cgrid->GetHeight() - 1)
    {
        ShowStatusMessage(wxString::Format("Game Over! The word was: %s", targetWord), wxColor(255, 100, 100));
    }
}

void cWordle::ShowStatusMessage(const wxString& message, const wxColor& color)
{
    statusMessage->SetLabel(message);
    statusMessage->SetForegroundColour(color);
    gameSizer->Layout();

    statusTimer->StartOnce(2000);
}

void cWordle::HideStatusMessage(wxTimerEvent& evt)
{
    statusMessage->SetLabel(wxEmptyString);
    gameSizer->Layout();
}

cWordle::~cWordle()
{
    delete cgrid;
    delete ckeyboard_eng;
    delete wordSelector;
    delete statusTimer;
}