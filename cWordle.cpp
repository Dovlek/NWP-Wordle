#include "cWordle.h"
#include "Theme.h"
#include <wx/fileconf.h>
#include <wx/filename.h>
#include <wx/stdpaths.h>
#include <wx/tokenzr.h>

cWordle::cWordle(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxWANTS_CHARS), statusTimer(this), gameTimer(this)
{
    SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());

    UIScaler& scaler = UIScaler::GetInstance();

    gameSizer = new wxBoxSizer(wxVERTICAL);
    wxGridSizer* gridSizer = new wxGridSizer(0, 0, wxSize(0, 0));

    wxStaticText* title = new wxStaticText(this, wxID_ANY, "WORDLE", wxDefaultPosition, wxDefaultSize);
    title->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    title->SetForegroundColour(ThemeManager::Get().GetTextColor());
    title->SetFont(wxFont(scaler.ScaledFontSize(22), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

    statusMessage = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize);
    statusMessage->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    statusMessage->SetForegroundColour(ThemeManager::Get().GetTextColor());
    statusMessage->SetFont(wxFont(scaler.ScaledFontSize(16), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false));

    // Create Grid
    cgrid = new cGrid(5, 6);
    gridSizer->Add(cgrid->CreateGrid(this));

    // Create keyboard Grid
    ckeyboard_eng = new cKeyboardENG();
    wxBoxSizer* keyboardPanel = ckeyboard_eng->CreateKeyboard(this);

    // Bind keyboard buttons
    ckeyboard_eng->BindKeyboardEvents(this, static_cast<void (wxEvtHandler::*)(wxCommandEvent&)>(&cWordle::OnKeyboardButtonClicked));

    // Create back button
    bool hasMaterialFont = false;
    if (wxFileExists("Resources/Fonts/material-symbols-outlined.ttf"))
    {
        hasMaterialFont = wxFont::AddPrivateFont("Resources/Fonts/material-symbols-outlined.ttf");
    }

    wxString buttonText = hasMaterialFont ? wxT("menu") : wxT("☰");
    wxFont buttonFont = hasMaterialFont ? wxFont(scaler.ScaledFontSize(20), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Material Symbols Outlined"))
                                        : wxFont(scaler.ScaledFontSize(18), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);

    backButton = new wxButton(this, wxID_ANY, buttonText, wxDefaultPosition, scaler.ScaledSize(50, 50), wxBORDER_NONE);
    backButton->SetFont(buttonFont);
    backButton->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    backButton->SetForegroundColour(ThemeManager::Get().GetTextColor());
    backButton->SetCursor(wxCursor(wxCURSOR_HAND));
    backButton->Bind(wxEVT_BUTTON, &cWordle::OnBackButtonClicked, this);

    // Stats labels
    wxFont statsFont(scaler.ScaledFontSize(14), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    winsText = new wxStaticText(this, wxID_ANY, "Wins: 0", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    winsText->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    winsText->SetForegroundColour(ThemeManager::Get().GetTextColor());
    winsText->SetFont(statsFont);

    lossesText = new wxStaticText(this, wxID_ANY, "Loses: 0", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    lossesText->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    lossesText->SetForegroundColour(ThemeManager::Get().GetTextColor());
    lossesText->SetFont(statsFont);

    streakText = new wxStaticText(this, wxID_ANY, "Streak: 0", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    streakText->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    streakText->SetForegroundColour(ThemeManager::Get().GetTextColor());
    streakText->SetFont(statsFont);

    maxStreakText = new wxStaticText(this, wxID_ANY, "Best streak: 0", wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
    maxStreakText->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    maxStreakText->SetForegroundColour(ThemeManager::Get().GetTextColor());
    maxStreakText->SetFont(statsFont);

    // Timer display
    wxFont timerFont(scaler.ScaledFontSize(16), wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false);
    timerDisplay = new wxStaticText(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER);
    timerDisplay->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    timerDisplay->SetForegroundColour(ThemeManager::Get().GetTextColor());
    timerDisplay->SetFont(timerFont);

    wxBoxSizer* topBarSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* statsSizer = new wxBoxSizer(wxHORIZONTAL);

    wxBoxSizer* firstColumnSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer* secondColumnSizer = new wxBoxSizer(wxVERTICAL);

    // Set sizers
    firstColumnSizer->Add(winsText, wxSizerFlags().Expand());
    firstColumnSizer->Add(streakText, wxSizerFlags().Expand().Border(wxTOP, scaler.ScaledValue(5)));

    secondColumnSizer->Add(lossesText, wxSizerFlags().Expand());
    secondColumnSizer->Add(maxStreakText, wxSizerFlags().Expand().Border(wxTOP, scaler.ScaledValue(5)));

    statsSizer->Add(firstColumnSizer, wxSizerFlags().Expand().Border(wxRIGHT, scaler.ScaledValue(15)));
    statsSizer->Add(secondColumnSizer, wxSizerFlags().Expand().Border(wxRIGHT, scaler.ScaledValue(5)));

    topBarSizer->Add(backButton, wxSizerFlags().Align(wxALIGN_TOP).Border(wxLEFT | wxTOP, scaler.ScaledValue(5)));
    topBarSizer->AddStretchSpacer();
    topBarSizer->Add(statsSizer, wxSizerFlags().Align(wxALIGN_TOP).Border(wxRIGHT | wxTOP, scaler.ScaledValue(5)));

    gameSizer->Add(topBarSizer, wxSizerFlags().Expand());
    gameSizer->Add(title, wxSizerFlags().CenterHorizontal().Border(wxTOP | wxLEFT | wxRIGHT, scaler.ScaledValue(25)));
    gameSizer->Add(statusMessage, wxSizerFlags().CenterHorizontal().Border(wxTOP | wxLEFT | wxRIGHT, scaler.ScaledValue(10)));
    gameSizer->Add(gridSizer, wxSizerFlags().CenterHorizontal().Border(wxALL, scaler.ScaledValue(10)));
    gameSizer->Add(timerDisplay, wxSizerFlags().CenterHorizontal().Border(wxBOTTOM | wxLEFT | wxRIGHT, scaler.ScaledValue(10)));
    gameSizer->Add(keyboardPanel, wxSizerFlags().CenterHorizontal().Border(wxBOTTOM | wxLEFT | wxRIGHT, scaler.ScaledValue(25)));
    gameSizer->AddStretchSpacer();
    this->SetSizer(gameSizer);

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

    Bind(wxEVT_TIMER, &cWordle::HideStatusMessage, this, statusTimer.GetId());
    Bind(wxEVT_TIMER, &cWordle::OnTimerTick, this, gameTimer.GetId());

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
    PauseTimer();

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
    backButton->SetBackgroundColour(ThemeManager::Get().GetButtonHoverColor());
    backButton->Refresh();
    evt.Skip();
}

void cWordle::OnBackButtonLeave(wxMouseEvent& evt)
{
    backButton->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
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
    wxBitmapButton* button = wxDynamicCast(evt.GetEventObject(), wxBitmapButton);
    if (!button)
        return;

    wxString letter = button->GetLabel();

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

            if (!wordSelector.IsValidWord(currentGuess))
            {
                ShowStatusMessage("Not in word list", ThemeManager::Get().GetErrorColor());
                return;
            }

            if (!ValidateHardModeGuess(currentGuess))
            {
                ShowStatusMessage("Hard mode: Must use revealed hints", ThemeManager::Get().GetErrorColor());
                return;
            }

            int guessRow = currentRow;
            prevRow = currentRow;
            prevCol = currentCol;
            currentRow++;
            currentCol = 0;

            CheckGuess(currentGuess, guessRow);
        }
        else if (currentCol < cgrid->GetWidth() && currentRow < cgrid->GetHeight())
        {
            ShowStatusMessage("Not enough letters", ThemeManager::Get().GetErrorColor());
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

    UpdateHardModeConstraints(guess, states);

    std::vector<int> intStates;
    intStates.reserve(states.size());
    for (const auto& state : states)
        intStates.push_back(static_cast<int>(state));

    cgrid->UpdateCellColors(row, intStates);
    ckeyboard_eng->UpdateKeyboardColors(guess, intStates);

    bool allCorrect = std::all_of(states.begin(), states.end(), [](LetterState state) { return state == LetterState::CORRECT; });

    if (allCorrect)
    {
        PauseTimer();
        gameState = GameState::COMPLETED;
        ShowGameEndDialog(true);
    }
    else if (row >= cgrid->GetHeight() - 1)
    {
        PauseTimer();
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
        if (streak > maxStreak)
            maxStreak = streak;
    }
    else
    {
        losses++;
        streak = 0;
    }
    UpdateStatsUI();

    wxString message;
    if (won)
    {
        ShowStatusMessage("Congratulations! You won!", ThemeManager::Get().GetSuccessColor());
        message = wxString::Format("Congratulations! You won!\n\nStart new round?");
    }
    else
    {
        ShowStatusMessage("You lost!", ThemeManager::Get().GetErrorColor());
        message = wxString::Format("You lost! The word was: %s\n\nStart new round?", targetWord);
    }

    wxMessageDialog dialog(this, message, "Round Finished!", wxYES_NO | wxYES_DEFAULT | wxICON_QUESTION);

    dialog.SetYesNoLabels("New Round", "Back to Menu");

    int result = dialog.ShowModal();

    if (result == wxID_YES)
    {
        StartNewRound();
    }
    else
    {
        gameState = GameState::COMPLETED_AWAITING_CONTINUE;

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

    targetWord = wordSelector.GetRandomWord();
    // wxLogMessage("Target word: %s", targetWord);

    cgrid->ResetGrid();
    ckeyboard_eng->ResetKeyboard();

    // Load game settings from config
    wxString configPath = wxStandardPaths::Get().GetUserDataDir();
    wxString configFile = configPath + wxFileName::GetPathSeparator() + "wordle_config.ini";
    wxFileConfig config(wxEmptyString, wxEmptyString, configFile, wxEmptyString, wxCONFIG_USE_LOCAL_FILE);

    timedModeEnabled = config.ReadBool("/Settings/TimedMode", false);
    hardModeEnabled = config.ReadBool("/Settings/HardMode", false);

    // Reset and start timer if enabled
    ResetTimer();
    ResetHardModeConstraints();

    if (timedModeEnabled)
    {
        StartTimer();
    }
    else
    {
        timerDisplay->SetLabel(wxEmptyString);
    }

    statusMessage->SetLabel(wxEmptyString);
    gameSizer->Layout();

    SetFocus();
}

void cWordle::ContinueFromFinishedGame()
{
    if (gameState == GameState::COMPLETED_AWAITING_CONTINUE)
        StartNewRound();
}

void cWordle::ForceEndCurrentGame()
{
    if (gameTimer.IsRunning())
        gameTimer.Stop();

    gameState = GameState::NOT_STARTED;
    currentRow = 0;
    currentCol = 0;
}

bool cWordle::IsGameInProgress() const
{
    return gameState == GameState::ACTIVE || gameState == GameState::COMPLETED_AWAITING_CONTINUE;
}

void cWordle::ShowStatusMessage(const wxString& message, const wxColor& color)
{
    statusMessage->SetLabel(message);
    statusMessage->SetForegroundColour(color);
    gameSizer->Layout();

    statusTimer.StartOnce(2000);
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
    maxStreakText->SetLabel(wxString::Format("Best streak: %d", maxStreak));
    gameSizer->Layout();
}

void cWordle::ResetStats()
{
    wins = 0;
    losses = 0;
    streak = 0;
    maxStreak = 0;
    UpdateStatsUI();
}

wxString cWordle::GetGameStateData() const
{
    wxString data;

    // Save game metadata
    data += wxString::Format("VERSION=In-progress\n"); // No version number yet
    data += wxString::Format("TARGET_WORD=%s\n", targetWord);
    data += wxString::Format("CURRENT_ROW=%d\n", currentRow);
    data += wxString::Format("CURRENT_COL=%d\n", currentCol);
    data += wxString::Format("GAME_STATE=%d\n", static_cast<int>(gameState));

    // Save stats
    data += wxString::Format("WINS=%d\n", wins);
    data += wxString::Format("LOSSES=%d\n", losses);
    data += wxString::Format("STREAK=%d\n", streak);
    data += wxString::Format("MAX_STREAK=%d\n", maxStreak);

    // Save timer settings
    data += wxString::Format("TIMED_MODE=%d\n", timedModeEnabled ? 1 : 0);
    data += wxString::Format("TIMER_REMAINING=%d\n", timerRemainingSeconds);

    // Save hard mode settings
    data += wxString::Format("HARD_MODE=%d\n", hardModeEnabled ? 1 : 0);

    // Save hard mode constraints
    wxString requiredPosStr;
    for (const auto& pair : requiredLetters)
    {
        for (wxChar letter : pair.second)
        {
            if (!requiredPosStr.IsEmpty())
                requiredPosStr += ",";
            requiredPosStr += wxString::Format("%c:%d", letter, pair.first);
        }
    }
    data += wxString::Format("HARD_MODE_REQUIRED_POSITIONS=%s\n", requiredPosStr);

    wxString mustUseStr;
    for (wxChar letter : mustUseLetters)
    {
        if (!mustUseStr.IsEmpty())
            mustUseStr += ",";
        mustUseStr += letter;
    }
    data += wxString::Format("HARD_MODE_MUST_USE=%s\n", mustUseStr);

    // Save grid state
    data += "GRID_DATA=\n";
    for (int row = 0; row < cgrid->GetHeight(); row++)
    {
        for (int col = 0; col < cgrid->GetWidth(); col++)
        {
            wxString letter = cgrid->GetLetter(row, col);
            if (letter.IsEmpty())
                letter = "_";
            data += letter;
        }
        data += "\n";
    }

    return data;
}

bool cWordle::SetGameStateData(const wxString& data)
{
    wxStringTokenizer tokenizer(data, "\n");

    cgrid->ResetGrid();
    ckeyboard_eng->ResetKeyboard();

    // Parse each line
    bool inGridData = false;
    int gridRow = 0;

    while (tokenizer.HasMoreTokens())
    {
        wxString line = tokenizer.GetNextToken().Trim().Trim(false);

        if (line.IsEmpty())
            continue;

        if (line == "GRID_DATA=")
        {
            inGridData = true;
            gridRow = 0;
            continue;
        }

        if (inGridData)
        {
            // Process grid row data
            if (gridRow < cgrid->GetHeight())
            {
                for (int col = 0; col < cgrid->GetWidth() && col < static_cast<int>(line.Length()); col++)
                {
                    wxString letter = wxString(line[col]);
                    if (letter != "_")
                        cgrid->SetLetter(gridRow, col, letter);
                }
                gridRow++;
            }
        }
        else
        {
            // Process key-value pairs
            int equalPos = line.Find('=');
            if (equalPos == wxNOT_FOUND)
                continue;

            wxString key = line.SubString(0, equalPos - 1);
            wxString value = line.SubString(equalPos + 1, line.Length() - 1);

            if (key == "TARGET_WORD")
                targetWord = value;
            else if (key == "CURRENT_ROW")
                currentRow = std::stoi(value.ToStdString());
            else if (key == "CURRENT_COL")
                currentCol = std::stoi(value.ToStdString());
            else if (key == "GAME_STATE")
                gameState = static_cast<GameState>(std::stoi(value.ToStdString()));
            else if (key == "WINS")
                wins = std::stoi(value.ToStdString());
            else if (key == "LOSSES")
                losses = std::stoi(value.ToStdString());
            else if (key == "STREAK")
                streak = std::stoi(value.ToStdString());
            else if (key == "MAX_STREAK")
                maxStreak = std::stoi(value.ToStdString());
            else if (key == "TIMED_MODE")
                timedModeEnabled = (std::stoi(value.ToStdString()) != 0);
            else if (key == "TIMER_REMAINING")
                timerRemainingSeconds = std::stoi(value.ToStdString());
            else if (key == "HARD_MODE")
                hardModeEnabled = (std::stoi(value.ToStdString()) != 0);
            else if (key == "HARD_MODE_REQUIRED_POSITIONS" && !value.IsEmpty())
            {
                // Parse format: "A:0,D:3"
                wxStringTokenizer posTokenizer(value, ",");
                while (posTokenizer.HasMoreTokens())
                {
                    wxString pair = posTokenizer.GetNextToken();
                    size_t colonPos = pair.Find(':');
                    if (colonPos != wxString::npos && colonPos > 0)
                    {
                        wxChar letter = pair[0];
                        int pos = std::stoi(pair.SubString(colonPos + 1, pair.Length() - 1).ToStdString());
                        requiredLetters[pos].insert(letter);
                    }
                }
            }
            else if (key == "HARD_MODE_MUST_USE" && !value.IsEmpty())
            {
                // Parse format: "E,R"
                wxStringTokenizer mustUseTokenizer(value, ",");
                while (mustUseTokenizer.HasMoreTokens())
                {
                    wxString letterStr = mustUseTokenizer.GetNextToken();
                    if (letterStr.Length() > 0)
                        mustUseLetters.insert(letterStr[0]);
                }
            }
        }
    }

    if (targetWord.IsEmpty())
    {
        wxLogError("Invalid save data: missing target word");
        return false;
    }

    // Recreate the visual state of completed rows
    for (int row = 0; row < currentRow && row < cgrid->GetHeight(); row++)
    {
        wxString guess;
        bool hasCompleteRow = true;

        // Get guess from grid
        for (int col = 0; col < cgrid->GetWidth(); col++)
        {
            wxString letter = cgrid->GetLetter(row, col);
            if (letter.IsEmpty())
            {
                hasCompleteRow = false;
                break;
            }
            guess += letter;
        }

        // Update colors on completed row
        if (hasCompleteRow && static_cast<int>(guess.Length()) == cgrid->GetWidth())
        {
            std::vector<LetterState> states = CompareWords(guess, targetWord);
            std::vector<int> intStates;
            for (const auto& state : states)
                intStates.push_back(static_cast<int>(state));

            cgrid->UpdateCellColors(row, intStates);
            ckeyboard_eng->UpdateKeyboardColors(guess, intStates);
        }
    }

    // Mark active row cells that already have letters
    if (currentRow >= 0 && currentRow < cgrid->GetHeight())
        cgrid->UpdateActiveRowCells(currentRow);

    UpdateStatsUI();
    statusMessage->SetLabel(wxEmptyString);

    // Restore timer display state
    if (timedModeEnabled)
    {
        UpdateTimerDisplay();
        // Don't auto-start timer when loading - it will start when game resumes
    }
    else
    {
        timerDisplay->SetLabel(wxEmptyString);
    }

    gameSizer->Layout();

    return true;
}

// Timer methods
void cWordle::StartTimer()
{
    if (timedModeEnabled && gameState == GameState::ACTIVE)
    {
        UpdateTimerDisplay();
        gameTimer.Start(1000);
    }
}

void cWordle::PauseTimer()
{
    if (gameTimer.IsRunning())
    {
        gameTimer.Stop();
    }
}

void cWordle::ResetTimer()
{
    gameTimer.Stop();
    timerRemainingSeconds = 300;
    UpdateTimerDisplay();
}

void cWordle::ResumeTimer()
{
    // Resume timer only if game is active and timed mode is enabled
    if (gameState == GameState::ACTIVE && timedModeEnabled)
    {
        StartTimer();
    }
}

void cWordle::OnTimerTick(wxTimerEvent& evt)
{
    if (timerRemainingSeconds > 0)
    {
        timerRemainingSeconds--;
        UpdateTimerDisplay();
    }
    else
    {
        // Time's up! End game as loss
        gameTimer.Stop();
        gameState = GameState::COMPLETED;
        losses++;
        streak = 0;
        UpdateStatsUI();
        ShowStatusMessage("Time's up!", ThemeManager::Get().GetErrorColor());
        ShowGameEndDialog(false);
    }
}

void cWordle::UpdateTimerDisplay()
{
    int minutes = timerRemainingSeconds / 60;
    int seconds = timerRemainingSeconds % 60;
    wxString timeStr = wxString::Format("Time: %02d:%02d", minutes, seconds);
    timerDisplay->SetLabel(timeStr);

    // Set color based on remaining time
    if (timerRemainingSeconds > 150) // Above 2.5 minutes (50%)
    {
        timerDisplay->SetForegroundColour(ThemeManager::Get().GetTimerGreenColor());
    }
    else if (timerRemainingSeconds > 45) // Above 45 seconds
    {
        timerDisplay->SetForegroundColour(ThemeManager::Get().GetTimerOrangeColor());
    }
    else // Under 45 seconds
    {
        timerDisplay->SetForegroundColour(ThemeManager::Get().GetTimerRedColor());
    }

    timerDisplay->Refresh();
}

// Hard mode methods
bool cWordle::ValidateHardModeGuess(const wxString& guess)
{
    if (!hardModeEnabled)
        return true;

    // Check that all required letters are in their required positions
    for (const auto& pair : requiredLetters)
    {
        int pos = pair.first;
        const std::set<wxChar>& letters = pair.second;

        if (pos < static_cast<int>(guess.Length()))
        {
            wxChar guessChar = guess[pos];
            if (letters.find(guessChar) == letters.end())
            {
                return false; // Required letter not in correct position
            }
        }
    }

    // Check that all must-use letters appear somewhere in the guess
    for (wxChar letter : mustUseLetters)
    {
        if (guess.Find(letter) == wxNOT_FOUND)
        {
            return false; // Required letter not used
        }
    }

    return true;
}

void cWordle::UpdateHardModeConstraints(const wxString& guess, const std::vector<LetterState>& states)
{
    if (!hardModeEnabled)
        return;

    for (size_t i = 0; i < states.size() && i < guess.Length(); i++)
    {
        wxChar letter = guess[i];

        if (states[i] == LetterState::CORRECT)
        {
            // This letter must stay in this position
            requiredLetters[i].insert(letter);
        }
        else if (states[i] == LetterState::WRONG_POSITION)
        {
            // This letter must be used somewhere
            mustUseLetters.insert(letter);
        }
    }
}

void cWordle::ResetHardModeConstraints()
{
    requiredLetters.clear();
    mustUseLetters.clear();
}

void cWordle::RefreshTheme()
{
    // Update panel background
    SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());

    // Update title colors
    for (wxWindowList::compatibility_iterator node = GetChildren().GetFirst(); node; node = node->GetNext())
    {
        wxWindow* child = node->GetData();
        if (wxStaticText* text = dynamic_cast<wxStaticText*>(child))
        {
            text->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
            text->SetForegroundColour(ThemeManager::Get().GetTextColor());
            text->Refresh();
        }
    }

    // Update back button
    if (backButton)
    {
        backButton->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
        backButton->SetForegroundColour(ThemeManager::Get().GetTextColor());
        backButton->Refresh();
    }

    // Reload grid and keyboard bitmaps
    if (cgrid)
    {
        cgrid->ReloadBitmaps();
        cgrid->RefreshAllCells();
    }

    if (ckeyboard_eng)
    {
        ckeyboard_eng->ReloadBitmaps();
    }

    Refresh();
}

cWordle::~cWordle()
{
    delete cgrid;
    delete ckeyboard_eng;
}