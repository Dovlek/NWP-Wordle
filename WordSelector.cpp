#include "WordSelector.h"
#include "resource.h"
#include <wx/msw/private.h>
#include <sstream>
#include <ctime>

WordSelector::WordSelector() : rng(static_cast<unsigned int>(std::time(nullptr)))
{
    LoadWordsFromResource();
}

bool WordSelector::LoadWordsFromResource()
{
    words.clear();
    
    HRSRC hResource = FindResource(NULL, MAKEINTRESOURCE(IDR_WORDSENG), RT_RCDATA);
    if (!hResource)
        return false;
    
    HGLOBAL hResData = LoadResource(NULL, hResource);
    if (!hResData)
        return false;
    
    const char* pData = static_cast<const char*>(LockResource(hResData));
    if (!pData)
        return false;
    
    DWORD dataSize = SizeofResource(NULL, hResource);
    if (dataSize == 0)
        return false;
    
    std::string fileContent(pData, dataSize);
    
    std::istringstream stream(fileContent);
    std::string line;
    
    while (std::getline(stream, line))
    {
        if (!line.empty() && line.back() == '\r')
            line.pop_back();
        
        if (line.length() == 5)
        {
            words.push_back(wxString(line.c_str(), wxConvUTF8));
        }
    }
    
    return !words.empty();
}

wxString WordSelector::GetRandomWord()
{
    if (words.empty())
        return wxEmptyString;
    
    std::uniform_int_distribution<size_t> dist(0, words.size() - 1);
    size_t randomIndex = dist(rng);
    
    return words[randomIndex].Upper();
}

bool WordSelector::IsValidWord(const wxString& word) const
{
    for (const auto& validWord : words)
    {
        if (validWord.Upper() == word.Upper())
            return true;
    }
    return false;
}