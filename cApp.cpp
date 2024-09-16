#include "cApp.h"

wxIMPLEMENT_APP(cApp);

cApp::cApp()
{
}

cApp::~cApp()
{
}

bool cApp::OnInit()
{
	m_frame = new cMain();
	m_frame->Center();
	m_frame->SetBackgroundColour(wxColor(20, 20, 20));
	m_frame->Show();

	return true;
}
