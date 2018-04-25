#include "GUI.h"
#include "MIVCOTS\MIVCOTS.h"
#include <iostream>

DECLARE_APP(GUI);
IMPLEMENT_APP(GUI);

bool GUI::OnInit()
{
	frame = new Frame(NULL);

	std::vector<serial::PortInfo> devices_found = serial::list_ports();
	std::vector<serial::PortInfo>::iterator iter = devices_found.begin();
	while (iter != devices_found.end())
	{
		serial::PortInfo device = *iter++;
		frame->comObjects.push_back(device.port);
	}

	frame->comObjects.push_back("COM43");
	/*activeCars.push_back(0);
	activeCars.push_back(1);*/
	bool tmp;
	aMIVCOTS.getCarNums(&activeCars, &tmp);

	frame->initFrame(&aMIVCOTS, &activeCars, &displayedCars);
	SetTopWindow(frame);
	frame->Show();
	frame->ShowFullScreen(true, wxFULLSCREEN_NOBORDER);

	aMIVCOTS.initialize();
	aMIVCOTS.start();

	timer = new wxTimer(this, gui_timer);
	timer->Start(1000 / FRAMERATE);		//main update thread


	wxLogDebug("Thread safety: %d", mysql_thread_safe());
	std::vector<databaseInfo> thing;
	aMIVCOTS.AvailablePlaybackData(&thing);

	//databaseInfo test;
	//test.carID = 1;
	//test.startTime = 1;
	//test.endTime = 5000;
	//test.startDate = 20180415;
	//test.endDate = 20180417;

	//aMIVCOTS.startPlayback(test, .5);

	return true;
}

void GUI::update(wxTimerEvent & event)
{
	//wxLogMessage("updating gui");
	frame->mapPanel.update();
	frame->Show();
	//frame->checkForNewCars(); //this seems like a bad thing to do every frame

}

void GUI::onExit(wxCommandEvent & event)
{
	timer->Stop();
	frame->Close(true);
	free(timer);
}

void Frame::onCheck(wxCommandEvent & event)
{
	this->displayedCars->clear();
	wxArrayInt tmp;
	carCheckListBox->GetCheckedItems(tmp);
	for (int i : tmp) {
		displayedCars->push_back(activeCars->at(i));
		wxLogMessage("Display: %d", i);
	}
}

void Frame::comStart(wxCommandEvent & event)
{
	int selection = comComboBox->GetSelection();
	if (!(aMIVCOTS->serialOpen())) {
		if (selection == wxNOT_FOUND) {
			wxLogMessage("No com port selected");
		}
		else {
			aMIVCOTS->initSerial(115200, comObjects.at(selection));
			aMIVCOTS->startSerial();
			openComButton->SetLabel("Stop");
		}
	}
	else {
		wxLogMessage("Closed serial port");
		aMIVCOTS->stopSerial();
		openComButton->SetLabel("Start");
	}
	//aMIVCOTS->getCarNums(activeCars);
	//carCheckTimer->StartOnce(1000);	//hacky way to get all the cars after serial opens
	return;
}

void Frame::carSelect(wxCommandEvent & event)
{
	int selection = carComboBox->GetSelection();
	checkForNewCars();
	if (selection == wxNOT_FOUND) {
		wxLogMessage("No car selected");
		return;
	}
	bool found = false;
	for (unsigned int i = 0; i < statusWidgets.size(); i++) {
		if (activeCars->at(selection) == statusWidgets.at(i).getCarID()) {
			found = true;
		}
	}
	if (!found) {
		createStatusWidget(activeCars->at(selection));
	}
	else {
		wxLogMessage("Car%d is already open", activeCars->at(selection));
	}
}

void Frame::onEraseBackground(wxEraseEvent & event)
{
	//doing nothing on purpose
}

void GUI::OnQuit(wxCloseEvent & evt)
{
	timer->Stop();
	frame->GetParent()->Close(true);
	aMIVCOTS.stop();
	aMIVCOTS.~MIVCOTS();
}

Frame::Frame(wxWindow * parent) : wxFrame(parent, -1, _("wxAUI Test"),
	wxDefaultPosition, wxDefaultSize,
	wxDEFAULT_FRAME_STYLE)
{

}

Frame::~Frame()
{
	m_mgr.UnInit();
	logTimer->Stop();
}

bool Frame::initFrame(MIVCOTS * aMIVCOTS, std::vector<long>* activeCars, std::vector<long>* displayedCars)
{
	this->aMIVCOTS = aMIVCOTS;
	this->activeCars = activeCars;
	this->displayedCars = displayedCars;

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

	log = new wxTextCtrl(this, -1, wxEmptyString,
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER | wxTE_MULTILINE | wxTE_READONLY);

	//adding log to gui
	wxLog::SetActiveTarget(new wxLogTextCtrl(log));

	mapPanel = Map(this);
	mapPanel.initMap(aMIVCOTS, this->displayedCars);
	

	createUIPanel();

	// add the panes to the manager
	m_mgr.SetFlags(m_mgr.GetFlags() ^ wxAUI_MGR_LIVE_RESIZE);

	m_mgr.AddPane(mapPanel.getPanel(), wxAuiPaneInfo().Caption(wxT("Map")).
		Center().
		MinSize(200, 200).
		BestSize(200, 200));
	m_mgr.AddPane(uiPanel, wxAuiPaneInfo().Caption(wxT("UI")).
		Bottom().
		MinSize(200, 200).
		BestSize(200, 200));
	m_mgr.AddPane(log, wxAuiPaneInfo().Caption(wxT("LOG")).
		Bottom().
		MinSize(200, 200).
		BestSize(200, 200).
		MaxSize(200, 200));
	createStatusWidgets();

	// tell the manager to "commit" all the changes just made
	m_mgr.Update();

	logTimer = new wxTimer(this, log_timer);
	logTimer->Start(LOG_FREQUENCY);

	carCheckTimer = new wxTimer(this, checkCarTimer);	//check if we have new cars every 5 seconds
	carCheckTimer->Start(5000);

	return true;
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

void Frame::onCarCombo(wxCommandEvent & event)
{
	carComboOpen = true;
	checkForNewCars();
}

void Frame::onCarComboClose(wxCommandEvent & event)
{
	carComboOpen = false;
}

bool Frame::createUIPanel()
{
	uiPanel = new wxPanel(this, wxID_ANY);
	wxArrayString com_arr;
	wxArrayString car_arr;

	wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
	wxFlexGridSizer* fgs1 = new wxFlexGridSizer(2, 3, 10, 10);
	wxFlexGridSizer* fgs2 = new wxFlexGridSizer(2, 1, 10, 10);

	int comSelection = 0;
	for (unsigned int i = 0; i < comObjects.size(); i++) {
		com_arr.Add((comObjects.at(i).c_str()));
		if (comObjects.at(i) == DEFAULT_SERIAL) {
			comSelection = i;
		}
	}
	wxStaticText* comText = new wxStaticText(uiPanel, wxID_ANY, "COM list");
	comComboBox = new wxComboBox(uiPanel, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, com_arr);
	comComboBox->SetSelection(comSelection);
	openComButton = new wxButton(uiPanel, comStartButton, "Start");

	fgs1->Add(comText, wxSizerFlags(1).Center());
	fgs1->Add(comComboBox, wxSizerFlags(1).Center());
	fgs1->Add(openComButton, wxSizerFlags(1).Center());

	for (int i : *activeCars) {
		car_arr.Add(std::to_string(i));
	}
	wxStaticText* carText = new wxStaticText(uiPanel, wxID_ANY, "Car list");
	carComboBox = new wxComboBox(uiPanel, carCombo, wxEmptyString, wxDefaultPosition, wxDefaultSize, car_arr);
	carComboBox->SetSelection(0);
	changeCarButton = new wxButton(uiPanel, carSelectButton, "Open");

	fgs1->Add(carText, wxSizerFlags(1).Center());
	fgs1->Add(carComboBox, wxSizerFlags(1).Center());
	fgs1->Add(changeCarButton, wxSizerFlags(1).Center());

	hbox1->Add(fgs1, 1, wxALL | wxEXPAND, 15);

	wxStaticText* mapText = new wxStaticText(uiPanel, wxID_ANY, "Cars on Map");
	carCheckListBox = new wxCheckListBox(uiPanel, carCheckList, wxDefaultPosition, wxDefaultSize, car_arr);

	fgs2->Add(mapText, wxSizerFlags(1).Center());
	fgs2->Add(carCheckListBox, wxSizerFlags(1).Center());

	hbox1->Add(fgs2, 1, wxALL | wxEXPAND, 15);

	vbox->Add(hbox1, 1, wxALL | wxEXPAND, 15);
	vbox->Add(hbox2, 1, wxALL | wxEXPAND, 15);
	uiPanel->SetSizer(vbox);
	return false;
}

StatusWidget* Frame::createStatusWidget(long carID)
{
	StatusWidget* statusWidget = new StatusWidget(this);
	statusWidget->initStatusWidget(aMIVCOTS, carID);

	statusWidgets.push_back(*statusWidget);

	wxString cap = wxString(wxT("Car" + std::to_string(carID)));
	m_mgr.AddPane(statusWidget->getPanel(), wxAuiPaneInfo().Name(wxT("Car" + std::to_string(carID))).
		DestroyOnClose(true).
		Caption(cap).
		MinSize(200, 200).
		BestSize(200, 200));

	m_mgr.Update();
	return statusWidget;
}

bool Frame::createStatusWidgets()
{
	for (long i : *activeCars) {
		bool found = false;
		for (StatusWidget temp : statusWidgets) {
			if (temp.getCarID() == i) {
				found = true;
			}
		}
		if (!found) {
			createStatusWidget(i);
		}
	}
	return true;
}

void Frame::checkForNewCars()
{
	bool tmp;
	std::vector<long> newCars;
	aMIVCOTS->getCarNums(&newCars, &tmp);
	int sel = carComboBox->GetSelection();
	for (long i : newCars) {
		bool found = false;
		for (long j : *activeCars) {
			if (i == j) {
				found = true;
			}
		}
		if (!found) {
			activeCars->push_back(i);
			carComboBox->Append(std::to_string(i));
			carCheckListBox->Append(std::to_string(i));
			carCheckListBox->Check(activeCars->size()-1);
			displayedCars->push_back(i);
		}
		carComboBox->SetSelection(sel);
	}
}

void Frame::checkForNewCarsTimer(wxTimerEvent & event)
{
	if (!carComboOpen) {	//if combo is not open check for new car
		checkForNewCars();
	}

}

void Frame::paneClosed(wxAuiManagerEvent & event)
{
	wxLogMessage("Closed " + event.GetPane()->caption);
	for (unsigned int i = 0; i < statusWidgets.size(); i++) {
		std::string tmp = "Car" + std::to_string(statusWidgets.at(i).getCarID());
		if (event.GetPane()->caption == tmp) {
			statusWidgets.erase(statusWidgets.begin() + i);
		}
	}
}
