#include "GUI.h"
#include "MIVCOTS\MIVCOTS.h"
#include <iostream>

DECLARE_APP(GUI);
IMPLEMENT_APP(GUI);

bool GUI::OnInit()
{
	Frame* frame = new Frame(NULL);

	SetTopWindow(frame);
	frame->Show();
	frame->ShowFullScreen(true, wxFULLSCREEN_NOBORDER);
	
	 aMIVCOTS.initSerial(115200, "COM43");
	 aMIVCOTS.startSerial();
	
	//frame->timer = wxTimer(frame);
	frame->mapPanel.drawCar(32.320264, -111.015069, 90 * 0.01745329252);


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

	wxTextCtrl* text3 = new wxTextCtrl(this, -1, _("Main content window\n"),
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER | wxTE_MULTILINE);
	/*wxStreamToTextRedirector redirect(text3);
	std::cout << "cout test" << std::endl;
	printf("printf test\n");*/
	wxLog::SetActiveTarget(new wxLogTextCtrl(text3));
	wxLogMessage("test in gui");

	mapPanel = Map(this);
	mapPanel.initMap();
	// add the panes to the manager
	m_mgr.AddPane(mapPanel.getPanel(), wxAuiPaneInfo().Center().MinSize(1280, 1280).BestSize(1280, 1280).MaxSize(1280, 1280));

	m_mgr.AddPane(text1, wxLEFT, wxT("Pane Number One"));
	m_mgr.AddPane(text2, wxBOTTOM, wxT("Pane Number Two"));
	m_mgr.AddPane(text3, wxBOTTOM);


	// tell the manager to "commit" all the changes just made
	m_mgr.Update();
	timer = new wxTimer(this, gui_timer);
	timer->Start(1000/FRAMERATE);

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

void Frame::update(wxTimerEvent &event)
{
	//wxLogMessage("updating gui");
	mapPanel.update();
	Show();
}



