#include "GUI.h"

DECLARE_APP(GUI);
IMPLEMENT_APP(GUI);

bool GUI::OnInit()
{
	wxFrame* frame = new Frame(NULL);
	SetTopWindow(frame);
	frame->Show();
	frame->ShowFullScreen(true, wxFULLSCREEN_NOBORDER);
	return true;
}


Frame::Frame(wxWindow * parent) : wxFrame(parent, -1, _("wxAUI Test"),
	wxDefaultPosition, wxDefaultSize,
	wxDEFAULT_FRAME_STYLE) 
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

	m_mgr.SetManagedWindow(this);

	// create several text controls
	wxTextCtrl* text1 = new wxTextCtrl(this, -1, _("Pane 1 - sample text"),
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER | wxTE_MULTILINE);

	wxTextCtrl* text2 = new wxTextCtrl(this, -1, _("Pane 2 - sample text"),
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER | wxTE_MULTILINE);

	wxTextCtrl* text3 = new wxTextCtrl(this, -1, _("Main content window"),
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER | wxTE_MULTILINE);

	std::string image = "C:/Users/Sean/Documents/MIVCOTS_SOFTWARE/projects/mivcots/GUI/test.png";

	Map mapPanel = Map(this);
	mapPanel.initMap();
	// add the panes to the manager
	m_mgr.AddPane(mapPanel.getPanel(), wxAuiPaneInfo().Center().MinSize(1280,1280).BestSize(1280, 1280).MaxSize(1280,1280));


	m_mgr.AddPane(text1, wxLEFT, wxT("Pane Number One"));
	m_mgr.AddPane(text2, wxBOTTOM, wxT("Pane Number Two"));
	m_mgr.AddPane(text3, wxBOTTOM);
	

	// tell the manager to "commit" all the changes just made
	m_mgr.Update();
}

Frame::~Frame()
{
	m_mgr.UnInit();
}

void Frame::onExit(wxCommandEvent & event)
{
	Close(true);
}

void Frame::onAbout(wxCommandEvent & event)
{
	wxMessageBox("This is the start of a MIVCOTS", "About MIVCOTS",
		wxOK | wxICON_INFORMATION);
}

void Frame::onToggleFullscreen(wxCommandEvent & event)
{
	ShowFullScreen(!IsFullScreen(), wxFULLSCREEN_NOBORDER);
}
