#include "GUI.h"

wxIMPLEMENT_APP(GUI);

bool GUI::OnInit()
{
	Frame *framep = new Frame("MIVCOTS", wxPoint(50, 50), wxSize(450, 340));
	framep->Show(true);
	framep->ShowFullScreen(true, wxFULLSCREEN_NOBORDER);
	return true;
}

Frame::Frame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	wxMenu *menuFile = new wxMenu;
	menuFile->Append(wxID_EXIT);

	wxMenu *menuView = new wxMenu;
	menuView->Append(toggleFullscreen, "Toggle fullscreen\tF11", "Toggle fullscreen display");

	wxMenu *menuHelp = new wxMenu;
	menuHelp->Append(wxID_ABOUT);

	wxMenuBar *menuBar = new wxMenuBar;
	menuBar->Append(menuFile, "File");
	menuBar->Append(menuView, "View");
	menuBar->Append(menuHelp, "Help");

	SetMenuBar(menuBar);
	CreateStatusBar();
	SetStatusText("Welcome to MIVCOTS!");
}

void Frame::onExit(wxCommandEvent &event)
{
	Close(true);
}

void Frame::onAbout(wxCommandEvent &event)
{
	wxMessageBox("This is the start of a MIVCOTS", "About MIVCOTS", 
				 wxOK | wxICON_INFORMATION);
}

void Frame::onToggleFullscreen(wxCommandEvent &event)
{
	ShowFullScreen(!IsFullScreen(), wxFULLSCREEN_NOBORDER);
}