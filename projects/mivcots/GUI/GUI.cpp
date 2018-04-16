#include "GUI.h"
#include "MIVCOTS\MIVCOTS.h"
#include <iostream>

DECLARE_APP(GUI);
IMPLEMENT_APP(GUI);

bool GUI::OnInit()
{
	frame = new Frame(NULL, &aMIVCOTS);

	SetTopWindow(frame);
	frame->Show();
	frame->ShowFullScreen(true, wxFULLSCREEN_NOBORDER);
	std::vector<serial::PortInfo> devices_found = serial::list_ports();

	std::vector<serial::PortInfo>::iterator iter = devices_found.begin();

	while (iter != devices_found.end())
	{
		serial::PortInfo device = *iter++;

		wxLogMessage("(%s, %s, %s)\n", device.port.c_str(), device.description.c_str(),
			device.hardware_id.c_str());
	}

	aMIVCOTS.initialize();
	aMIVCOTS.start();
	aMIVCOTS.initSerial(115200, "COM43");
	aMIVCOTS.startSerial();
	
	//frame->timer = wxTimer(frame);
	frame->mapPanel.drawCar(32.320264, -111.015069, 90 * 0.01745329252);

	timer = new wxTimer(this, gui_timer);
	timer->Start(1000 / FRAMERATE);

	return true;
}

void GUI::update(wxTimerEvent & event)
{
	//wxLogMessage("updating gui");
	frame->mapPanel.update();
	frame->Show();
	
}

void GUI::onExit(wxCommandEvent & event)
{
	timer->Stop();
	frame->Close(true);
	free(timer);
}

void GUI::OnQuit(wxCloseEvent & evt)
{
	timer->Stop();
	frame->GetParent()->Close(true);
}


Frame::Frame(wxWindow * parent, MIVCOTS* aMIVCOTS) : wxFrame(parent, -1, _("wxAUI Test"),
	wxDefaultPosition, wxDefaultSize,
	wxDEFAULT_FRAME_STYLE)
{
	this->aMIVCOTS = aMIVCOTS;
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

	wxTextCtrl* text2 = new wxTextCtrl(this, -1, _("Pane 2 - sample text"),
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER | wxTE_MULTILINE);

	log = new wxTextCtrl(this, -1, wxEmptyString,
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER | wxTE_MULTILINE | wxTE_READONLY);

	//adding log to gui
	wxLog::SetActiveTarget(new wxLogTextCtrl(log));
	wxLogMessage("test in gui");
	//log->SaveFile("test.txt");
	

	mapPanel = Map(this);
	mapPanel.initMap(aMIVCOTS);

	statusWidget = StatusWidget(this);
	statusWidget.initStatusWidget(aMIVCOTS);

	// add the panes to the manager
	m_mgr.SetFlags(m_mgr.GetFlags() ^ wxAUI_MGR_LIVE_RESIZE);

	m_mgr.AddPane(mapPanel.getPanel(), wxAuiPaneInfo().Center().MinSize(1280, 1280).BestSize(1280, 1280).MaxSize(1280, 1280));

	m_mgr.AddPane(statusWidget.getPanel(), wxLEFT, wxT("Status"));
	m_mgr.AddPane(text2, wxBOTTOM, wxT("Pane Number Two"));
	m_mgr.AddPane(log, wxBOTTOM, wxT("Log"));


	// tell the manager to "commit" all the changes just made
	m_mgr.Update();

	logTimer = new wxTimer(this, log_timer);
	logTimer->Start(LOG_FREQUENCY);
}

Frame::~Frame()
{
	m_mgr.UnInit();
	logTimer->Stop();
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

void Frame::update(wxTimerEvent & event)
{
	wxString str = log->GetValue();
	if (!str.empty()) {
		log->Clear();
		std::ofstream file;
		file.open("log.txt", std::ios::out | std::ios::app);
		file << str;
		file.close();
	}
}
