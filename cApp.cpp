#include "cApp.h"
#include "EmbeddedResources.h"
#include "Theme.h"
#include <wx/fs_mem.h>

wxIMPLEMENT_APP(cApp);

cApp::cApp()
{
}

cApp::~cApp()
{
}

bool cApp::OnInit()
{
    // Initialize all image handlers (required for ICO, BMP, PNG, etc.)
    wxInitAllImageHandlers();

    // Initialize memory filesystem handler for embedded resources
    wxFileSystem::AddHandler(new wxMemoryFSHandler);
    InitializeEmbeddedResources();

    m_frame = new cMain();
    m_frame->Center();
    m_frame->SetBackgroundColour(ThemeManager::Get().GetBackgroundColor());
    m_frame->Show();
    m_frame->SetMinClientSize(m_frame->GetClientSize());

    return true;
}
