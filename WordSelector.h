#pragma once
#include <wx/wx.h>
#include <vector>
#include <random>

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
};