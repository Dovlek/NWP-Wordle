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

    // Create back button
    backButton = new wxButton(this, wxID_ANY, wxT("menu"), wxDefaultPosition, wxSize(50, 50), wxBORDER_NONE);
    backButton->SetFont(wxFont(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Material Symbols Outlined")));
    backButton->SetBackgroundColour(wxColor(20, 20, 20));
    backButton->SetForegroundColour(wxColor(*wxWHITE));
    backButton->SetCursor(wxCursor(wxCURSOR_HAND));
    backButton->Bind(wxEVT_BUTTON, &cWordle::OnBackButtonClicked, this);

    // Stats labels
    wxFont statsFont(14, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    winsText = new wxStaticText(this, wxID_ANY, "Wins: 0", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    winsText->SetBackgroundColour(wxColor(20, 20, 20));
    winsText->SetForegroundColour(wxColor(*wxWHITE));
    winsText->SetFont(statsFont);

    lossesText = new wxStaticText(this, wxID_ANY, "Loses: 0", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    lossesText->SetBackgroundColour(wxColor(20, 20, 20));
    lossesText->SetForegroundColour(wxColor(*wxWHITE));
    lossesText->SetFont(statsFont);

    streakText = new wxStaticText(this, wxID_ANY, "Streak: 0", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    streakText->SetBackgroundColour(wxColor(20, 20, 20));
    streakText->SetForegroundColour(wxColor(*wxWHITE));
    streakText->SetFont(statsFont);

    wxBoxSizer* topBarSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* statsSizer = new wxBoxSizer(wxVERTICAL);

    // Set Sizers
    statsSizer->Add(winsText, wxSizerFlags().Expand().Border(wxRIGHT, 5));
    statsSizer->Add(lossesText, wxSizerFlags().Expand().Border(wxTOP | wxRIGHT, 5));
    statsSizer->Add(streakText, wxSizerFlags().Expand().Border(wxTOP | wxRIGHT, 5));

    topBarSizer->Add(backButton, wxSizerFlags().Align(wxALIGN_TOP).Border(wxLEFT | wxTOP, 5));
    topBarSizer->AddStretchSpacer();
    topBarSizer->Add(statsSizer, wxSizerFlags().Align(wxALIGN_TOP).Border(wxRIGHT | wxTOP, 5));

    gameSizer->Add(topBarSizer, wxSizerFlags().Expand());
    gameSizer->Add(title, wxSizerFlags().CenterHorizontal().Border(wxALL, 25));
    gameSizer->Add(statusMessage, wxSizerFlags().CenterHorizontal().Border(wxALL, 2));
    gameSizer->Add(gridSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, 10));
    gameSizer->Add(keyboardPanel, wxSizerFlags().CenterHorizontal().Border(wxALL, 25));
    gameSizer->AddStretchSpacer();
    this->SetSizer(gameSizer);
    gameSizer->SetSizeHints(this);

    // Set up accelerator table
    wxAcceleratorEntry entries[3];
    entries[0].Set(wxACCEL_NORMAL, WXK_RETURN, ID_ACCEL_ENTER);
    entries[1].Set(wxACCEL_NORMAL, WXK_BACK, ID_ACCEL_BACKSPACE);
    entries[2].Set(wxACCEL_NORMAL, WXK_ESCAPE, ID_ACCEL_ESCAPE);
    wxAcceleratorTable accelTable(3, entries);
    SetAcceleratorTable(accelTable);

    // Bind accelerator events
    Bind(wxEVT_COMMAND_MENU_SELECTED, &cWordle::OnAcceleratorPressed, this, ID_ACCEL_ENTER);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &cWordle::OnAcceleratorPressed, this, ID_ACCEL_BACKSPACE);
    Bind(wxEVT_COMMAND_MENU_SELECTED, &cWordle::OnAcceleratorPressed, this, ID_ACCEL_ESCAPE);

    // Bind keyboard events
    Bind(wxEVT_CHAR_HOOK, &cWordle::OnKeyboardButtonPressed, this);

    // Bind mouse events
    backButton->Bind(wxEVT_ENTER_WINDOW, &cWordle::OnBackButtonEnter, this);
    backButton->Bind(wxEVT_LEAVE_WINDOW, &cWordle::OnBackButtonLeave, this);

    wordSelector = new WordSelector();
    
    statusTimer = new wxTimer(this);
    Bind(wxEVT_TIMER, &cWordle::HideStatusMessage, this);

    UpdateStatsUI();
}

void cWordle::OnAcceleratorPressed(wxCommandEvent& evt)
{
    int id = evt.GetId();

    if (id == ID_ACCEL_ENTER)
    {
        ProcessKey("ENTER");
    }
    else if (id == ID_ACCEL_BACKSPACE)
    {
        ProcessKey("backspace");
    }
    else if (id == ID_ACCEL_ESCAPE)
    {
        wxCommandEvent switchEvent(wxEVT_SWITCH_TO_MENU);
        switchEvent.SetEventObject(this);
        
        wxWindow* parent = GetParent();
        while (parent && !parent->IsTopLevel())
            parent = parent->GetParent();

        if (parent)
            wxPostEvent(parent, switchEvent);
    }

    SetFocus();
}

void cWordle::OnBackButtonClicked(wxCommandEvent& evt)
{
    wxCommandEvent switchEvent(wxEVT_SWITCH_TO_MENU);
    switchEvent.SetEventObject(this);

    wxWindow* parent = GetParent();
    while (parent && !parent->IsTopLevel())
        parent = parent->GetParent();

    if (parent)
        wxPostEvent(parent, switchEvent);
}

void cWordle::OnBackButtonEnter(wxMouseEvent& evt)
{
    backButton->SetBackgroundColour(wxColor(58, 58, 60));
    backButton->Refresh();
    evt.Skip();
}

void cWordle::OnBackButtonLeave(wxMouseEvent& evt)
{
    backButton->SetBackgroundColour(wxColor(20, 20, 20));
    backButton->Refresh();
    evt.Skip();
}

void cWordle::OnKeyboardButtonPressed(wxKeyEvent& evt)
{
    int keyCode = evt.GetKeyCode();
    wxString key;

    if (keyCode >= 'A' && keyCode <= 'Z')
        key = wxString(static_cast<char>(keyCode));
    else if (keyCode >= 'a' && keyCode <= 'z')
        key = wxString(static_cast<char>(keyCode - 32));

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
    // Don't process keys if game is not active
    if (gameState != GameState::ACTIVE)
        return;

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

            int guessRow = currentRow;
            prevRow = currentRow;
            prevCol = currentCol;
            currentRow++;
            currentCol = 0;

            CheckGuess(currentGuess, guessRow);
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
        gameState = GameState::COMPLETED;
        ShowGameEndDialog(true);
    }
    else if (row >= cgrid->GetHeight() - 1)
    {
        gameState = GameState::COMPLETED;
        ShowGameEndDialog(false);
    }
}

void cWordle::ShowGameEndDialog(bool won)
{
    if (won)
    {
        wins++;
        streak++;
    }
    else
    {
        losses++;
        streak = 0;
    }
    UpdateStatsUI();

    wxString message;
    if (won)
        message = wxString::Format("Congratulations! You won!\n\nStart new round?");
    else
        message = wxString::Format("Game Over! The word was: %s\n\nStart new round?", targetWord);
    
    wxMessageDialog dialog(this, message, "Game Over", wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);
    
    dialog.SetYesNoLabels("New Round", "Back to Menu");
    
    int result = dialog.ShowModal();
    
    if (result == wxID_YES)
    {
        StartNewRound();
    }
    else
    {
        wxCommandEvent switchEvent(wxEVT_SWITCH_TO_MENU);
        switchEvent.SetEventObject(this);
        
        wxWindow* parent = GetParent();
        while (parent && !parent->IsTopLevel())
            parent = parent->GetParent();

        if (parent)
            wxPostEvent(parent, switchEvent);
    }
}

void cWordle::StartNewRound()
{
    currentRow = 0;
    currentCol = 0;
    prevRow = 0;
    prevCol = 0;
    gameState = GameState::ACTIVE;
    
    targetWord = wordSelector->GetRandomWord();
    wxLogMessage("Target word: %s", targetWord);
 
    cgrid->ResetGrid();
    ckeyboard_eng->ResetKeyboard();
    
    statusMessage->SetLabel(wxEmptyString);
    gameSizer->Layout();
    
    SetFocus();
}

bool cWordle::IsGameInProgress() const
{
    return gameState == GameState::ACTIVE;
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

void cWordle::UpdateStatsUI()
{
    winsText->SetLabel(wxString::Format("Wins: %d", wins));
    lossesText->SetLabel(wxString::Format("Losses: %d", losses));
    streakText->SetLabel(wxString::Format("Streak: %d", streak));
    gameSizer->Layout();
}

cWordle::~cWordle()
{
    delete cgrid;
    delete ckeyboard_eng;
    delete wordSelector;
    delete statusTimer;
}