#pragma once
#include <random>
#include <vector>
#include <wx/wx.h>

class WordSelector
{
private:
    std::vector<wxString> words;
    std::mt19937 rng;

public:
    WordSelector();
    bool LoadWordsFromResource();
    wxString GetRandomWord();
    size_t GetWordCount() const { return words.size(); }
    bool IsValidWord(const wxString& word) const;
};