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

	frame->initFrame(&aMIVCOTS, &activeCars, &displayedCars, &playBackOptions);
	SetTopWindow(frame);
	frame->Show();
	frame->ShowFullScreen(true, wxFULLSCREEN_NOBORDER);

	aMIVCOTS.initialize();
	aMIVCOTS.start();

	timer = new wxTimer(this, gui_timer);
	timer->Start(1000 / FRAMERATE);		//main update thread


	wxLogDebug("Thread safety: %d", mysql_thread_safe());

	return true;
}

void GUI::update(wxTimerEvent & event)
{
	//wxLogMessage("updating gui");
	frame->mapPanel.update();
	//frame->(graph->Reload());
	frame->graph.Reload();
	for (StatusWidget* cur : frame->statusWidgets) {
		cur->update();
	}

	frame->Show();
	//frame->checkForNewCars(); //this seems like a bad thing to do every frame

}

void GUI::onExit(wxCommandEvent & event)
{
	closeProgram();
}

void Frame::onCheck(wxCommandEvent & event)
{
	int oldSize = this->displayedCars->size();
	this->displayedCars->clear();
	wxArrayInt tmp;
	carCheckListBox->GetCheckedItems(tmp);
	for (int i : tmp) {
		displayedCars->push_back(activeCars->at(i));
		wxLogMessage("Display: %d", i);
	}
	int newSize = this->displayedCars->size();;
	if (newSize < oldSize) {
		mapPanel.mapRefresh();
	}
}

bool Frame::createPlaybackWidget()
{
	wxBoxSizer* vbox1 = new wxBoxSizer(wxVERTICAL);
	wxFlexGridSizer* fgs = new wxFlexGridSizer(5, 3, 10, 10);

	wxStaticText* carIdText = new wxStaticText(uiPanel, wxID_ANY, "Cars in Database");
	playIdComboBox = new wxComboBox(uiPanel, playBackIdCombo, wxEmptyString, wxDefaultPosition, wxDefaultSize);



	//playDateStartSpinBox = new wxSpinCtrl(uiPanel, playDateStartSpinBoxID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP, 0, 0, 0);
	//playDateEndSpinBox = new wxSpinCtrl(uiPanel, playDateEndSpinBoxID, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxSP_WRAP, 0, 0, 0);

	playDateStartSpinBox = new wxDatePickerCtrl(uiPanel, playDateStartSpinBoxID);
	playDateEndSpinBox = new wxDatePickerCtrl(uiPanel, playDateEndSpinBoxID);
	playTimeStartSpinBox = new wxTimePickerCtrl(uiPanel, playTimeStartSpinBoxID);
	playTimeEndSpinBox = new wxTimePickerCtrl(uiPanel, playTimeEndSpinBoxID);
	startPlayBack = new wxButton(uiPanel, startPlayBackButton, "Start");

	playDateStartSpinBox->Enable(false);
	playDateEndSpinBox->Enable(false);
	playTimeStartSpinBox->Enable(false);
	playTimeEndSpinBox->Enable(false);
	fgs->Add(carIdText, 1, wxEXPAND, 15);
	//fgs->Add(new wxStaticText(uiPanel, wxID_ANY, "Date"), 1, wxEXPAND, 15);
	//fgs->Add(new wxStaticText(uiPanel, wxID_ANY, "TIme"), 1, wxEXPAND, 15);

	fgs->Add(new wxStaticText(uiPanel, wxID_ANY, "Start Date"), 1, wxEXPAND, 15);
	fgs->Add(new wxStaticText(uiPanel, wxID_ANY, "Start Time"), 1, wxEXPAND, 15);

	fgs->Add(playIdComboBox, 1, wxEXPAND, 15);
	//fgs->AddSpacer(0);
	fgs->Add(playDateStartSpinBox, 1, wxEXPAND, 15);
	fgs->Add(playTimeStartSpinBox, 1, wxEXPAND, 15);

	fgs->AddSpacer(0);
	fgs->Add(new wxStaticText(uiPanel, wxID_ANY, "End Date"), 1, wxEXPAND, 15);
	fgs->Add(new wxStaticText(uiPanel, wxID_ANY, "End Time"), 1, wxEXPAND, 15);

	//fgs->AddSpacer(0);
	fgs->Add(startPlayBack, 1, wxEXPAND, 15);
	fgs->Add(playDateEndSpinBox, 1, wxEXPAND, 15);
	fgs->Add(playTimeEndSpinBox, 1, wxEXPAND, 15);

	playBackBox->Add(fgs, 1, wxEXPAND, 15);


	return true;
}

bool Frame::updatePlayBackWidget()
{
	std::vector<databaseInfo> newInfo;
	aMIVCOTS->AvailablePlaybackData(&newInfo);
	int sel = playIdComboBox->GetSelection();
	for (databaseInfo newData : newInfo) {
		bool found = false;
		for (databaseInfo cur : *playBackOptions) {
			if (newData.carID == cur.carID) {
				found = true;
			}
		}
		if (!found) {
			playIdComboBox->Append(std::to_string(newData.carID));
			playBackCars.push_back(newData.carID);
			playBackOptions->push_back(newData);
		}
	}
	playIdComboBox->SetSelection(sel);
	return true;
}

void Frame::onPlayComboOpen(wxCommandEvent & event)
{
	//playComboOpen = true;
	updatePlayBackWidget();
}

void Frame::onPlayBackCombo(wxCommandEvent & event)
{
	int sel = playIdComboBox->GetCurrentSelection();
	wxLogMessage("selected car %d", sel);
	playDateStartSpinBox->Enable(true);
	playDateEndSpinBox->Enable(false);
	playTimeStartSpinBox->Enable(false);
	playTimeEndSpinBox->Enable(false);

	
	startPlayBack->Enable(true);
	for (int cur : *activeCars) {
		if (-cur == playBackCars.at(sel)) {
			startPlayBack->Enable(false);
			break;
		}
		
	}
	//playDateStartSpinBox->Clear();
	//playDateEndSpinBox->Clear();
	//playTimeStartSpinBox->Clear();
	//playTimeEndSpinBox->Clear();
	long min = 99999999;
	long max = 0;
	playBackStartDates.clear();
	wxDateTime d1(wxDateTime::Now());
	wxDateTime d2(wxDateTime::Now());
	for (databaseInfo cur : *playBackOptions) {
		if (cur.carID == playBackCars.at(sel)) {
			playBackStartDates.push_back(cur.startDate);
			if (min > cur.startDate) {
				min = cur.startDate;
				d1.SetHour(cur.startTime / 10000000);
				d1.SetMinute((cur.startTime % 10000000) / 100000);
				d1.SetSecond(cur.startTime % 100000 / 1000);
				d2.SetMillisecond(cur.endTime % 1000);
			}
			if (max < cur.startDate) {
				max = cur.endDate;
				int tmp = cur.endTime / 10000;
				d2.SetHour(cur.endTime / 10000000);
				d2.SetMinute((cur.endTime % 10000000) / 100000);
				d2.SetSecond(cur.endTime % 100000 / 1000);
				d2.SetMillisecond(cur.endTime % 1000);
			}

		}
	}

	createDateTimes(min, max, &d1, &d2);
	playDateStartSpinBox->SetValue(d1);
	playDateStartSpinBox->SetRange((const wxDateTime)d1, (const wxDateTime)d2);
	
	//Sleep(500);
	onPlayBackStartDate2(d1);
}

void Frame::onPlayStartDateOpen(wxCommandEvent & event)
{
}

void Frame::onPlayStartDateClose(wxCommandEvent & event)
{
}

void Frame::onPlayBackStartDate(wxDateEvent & event)
{
	wxLogMessage("in start");
	long date = monthToLong(playDateStartSpinBox->GetValue());
	playDateEndSpinBox->Enable(true);
	playTimeStartSpinBox->Enable(true);
	playTimeEndSpinBox->Enable(true);

	//playDateEndSpinBox->Clear();
	////playTimeStartSpinBox->Clear();
	////playTimeEndSpinBox->Clear();

	long min = 99999999;
	long max = 0;
	playBackEndDates.clear();
	for (databaseInfo cur : *playBackOptions) {
		if (cur.carID == playBackCars.at(playIdComboBox->GetCurrentSelection())) {
			if (cur.startDate == date) {
				playBackEndDates.push_back(cur.endDate);
				//playDateEndSpinBox->Append(std::to_string(cur.endDate));
				if (min > cur.startDate) {
					min = cur.startDate;
				}
				if (max < cur.startDate) {
					max = cur.startDate;
				}

			}
		}
	}
	wxDateTime d1(wxDateTime::Now());
	wxDateTime d2(wxDateTime::Now());
	createDateTimes(min, max, &d1, &d2);
	playDateEndSpinBox->SetValue(d1);
	playDateEndSpinBox->SetRange((const wxDateTime)d1, (const wxDateTime)d2);
}
void Frame::onPlayBackStartDate2(wxDateTime d3)
{
	wxLogMessage("in start");
	long date = monthToLong(d3);
	playDateEndSpinBox->Enable(true);
	playTimeStartSpinBox->Enable(false);
	playTimeEndSpinBox->Enable(false);

	//playDateEndSpinBox->Clear();
	////playTimeStartSpinBox->Clear();
	////playTimeEndSpinBox->Clear();

	long min = 99999999;
	long max = 0;
	playBackEndDates.clear();
	wxDateTime d1(wxDateTime::Now());
	wxDateTime d2(wxDateTime::Now());
	for (databaseInfo cur : *playBackOptions) {
		if (cur.carID == playBackCars.at(playIdComboBox->GetCurrentSelection())) {
			if (cur.startDate == date) {
				playBackEndDates.push_back(cur.endDate);
				//playDateEndSpinBox->Append(std::to_string(cur.endDate));
				if (min > cur.startDate) {
					min = cur.startDate;
					d1.SetHour(cur.startTime / 10000000);
					d1.SetMinute((cur.startTime % 10000000) / 100000);
					d1.SetSecond(cur.startTime % 100000 / 1000);
					d1.SetMillisecond(cur.endTime % 1000);
					
				}
				if (max < cur.endDate) {
					max = cur.endDate;
					d2.SetHour(cur.endTime / 10000000);
					d2.SetMinute((cur.endTime % 10000000) / 100000);
					d2.SetSecond(cur.endTime % 100000 / 1000);
					d2.SetMillisecond(cur.endTime % 1000);
				}

			}
		}
	}
	createDateTimes(min, max, &d1, &d2);
	playDateEndSpinBox->SetValue(d1);
	playDateEndSpinBox->SetRange((const wxDateTime)d1, (const wxDateTime)d2);
	
	onPlayBackEndDate2(d3, d2);
}

void Frame::onPlayBackEndDate2(wxDateTime d1, wxDateTime d2)
{
	playTimeStartSpinBox->Enable(true);
	playTimeEndSpinBox->Enable(true);

	maxTime = d2.GetHour()    * 10000000;
	maxTime += d2.GetMinute() * 100000;
	maxTime += d2.GetSecond() * 1000;
	//maxTime += d2.GetMillisecond();

	minTime = d1.GetHour()    * 10000000;
	minTime += d1.GetMinute() * 100000;
	minTime += d1.GetSecond() * 1000;
	//minTime += d1.GetMillisecond();

	playTimeStartSpinBox->SetTime(d1.GetHour(), d1.GetMinute(), d1.GetSecond());
	playTimeEndSpinBox->SetTime(d2.GetHour(), d2.GetMinute(), d2.GetSecond());

	
}

//#include <ctime>
void Frame::onStartPlayBack(wxCommandEvent & event)
{
	//auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	//wxLogMessage("Received a playback request at %s", ctime(&time));

	databaseInfo replay;
	int hour, min, sec;
	wxDateTime date(wxDateTime::Now());
	replay.carID = (playBackOptions->at(playIdComboBox->GetSelection())).carID;
	date = playDateStartSpinBox->GetValue();
	replay.startDate = monthToLong(date);
	date = playDateEndSpinBox->GetValue();
	replay.endDate = monthToLong(date);

	playTimeStartSpinBox->GetTime(&hour,&min,&sec);
	replay.startTime = ((hour * 10000000) + (min * 100000) + (sec * 1000));
	playTimeEndSpinBox->GetTime(&hour, &min, &sec);
	replay.endTime = ((hour * 10000000) + (min * 100000) + (sec * 1000));
	
	aMIVCOTS->startPlayback(replay, 1);
	startPlayBack->Enable(false);
	playIdComboBox->SetSelection(wxNOT_FOUND);
}

void Frame::onTimeStart(wxDateEvent & event)
{
	int hour, min, sec;
	playTimeStartSpinBox->GetTime(&hour,&min,&sec);
	int testTime = (hour * 10000000) + (min * 100000) + (sec * 1000);

	if (testTime < minTime) {
		hour = minTime / 10000000;
		min = (minTime % 10000000) / 100000;
		sec = (minTime % 100000 / 1000);
		playTimeStartSpinBox->SetTime(hour, min, sec);
	}
	if (testTime > maxTime) {
		hour = maxTime / 10000000;
		min = (maxTime % 10000000) / 100000;
		sec = (maxTime % 100000 / 1000);
		playTimeStartSpinBox->SetTime(hour, min, sec);
	}

	int hour2, min2, sec2;
	playTimeEndSpinBox->GetTime(&hour2, &min2, &sec2);
	int testTime2 = (hour2 * 10000000) + (min2 * 100000) + (sec2 * 1000);

	if (testTime > testTime2) {
		playTimeStartSpinBox->SetTime(hour2, min2, sec2);
	}
}

void Frame::onTimeEnd(wxDateEvent & event)
{
	int hour, min, sec;
	playTimeEndSpinBox->GetTime(&hour, &min, &sec);
	int testTime = (hour * 10000000) + (min * 100000) + (sec * 1000);

	if (testTime < minTime) {
		hour = minTime / 10000000;
		min = (minTime % 10000000) / 100000;
		sec = (minTime % 100000 / 1000);
		playTimeEndSpinBox->SetTime(hour, min, sec);
	}
	if (testTime > maxTime) {
		hour = maxTime / 10000000;
		min = (maxTime % 10000000) / 100000;
		sec = (maxTime % 100000 / 1000);
		playTimeEndSpinBox->SetTime(hour, min, sec);
	}
	int hour2, min2, sec2;
	playTimeStartSpinBox->GetTime(&hour2, &min2, &sec2);
	int testTime2 = (hour2 * 10000000) + (min2 * 100000) + (sec2 * 1000);

	if (testTime < testTime2) {
		playTimeEndSpinBox->SetTime(hour2, min2, sec2);
	}
}


void Frame::onPlayComboClose(wxCommandEvent & event)
{
	//playComboOpen = false;
}
void Frame::comStart(wxCommandEvent & event)
{
	int selection = comComboBox->GetSelection();
	if (!(aMIVCOTS->serialOpen())) {
		if (selection == wxNOT_FOUND) {
			wxLogMessage("No com port selected");
		}
		else {
			int rc = aMIVCOTS->initSerial(115200, comObjects.at(selection));
			if (rc == SUCCESS) {
				aMIVCOTS->startSerial();
			}
			else {
				wxLogMessage("Couldn't open serial");
				return;
			}
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
		if (activeCars->at(selection) == statusWidgets.at(i)->getCarID()) {
			found = true;
		}
	}
	if (!found) {
		StatusWidget* temp = createStatusWidget(activeCars->at(selection));
		//statusWidgets.push_back(createStatusWidget(activeCars->at(selection)));
	}
	else {
		wxLogMessage("Car%d is already open", activeCars->at(selection));
	}
}

void Frame::onEraseBackground(wxEraseEvent & event)
{
	//doing nothing on purpose
}

void Frame::OnQuit(wxCloseEvent & evt)
{
	if (evt.CanVeto()) {
		wxCommandEvent ev(wxEVT_MENU, wxID_EXIT);
		wxTheApp->GetTopWindow()->GetEventHandler()->ProcessEvent(ev);
	}
	evt.Skip();
}

void GUI::OnQuit(wxCloseEvent & evt)
{
	//timer->Stop();
	//frame->GetParent()->Close(true);
	//aMIVCOTS.stop();
	//aMIVCOTS.~MIVCOTS();
	//evt.Veto();
	//closeProgram();
	aMIVCOTS.stop();
}

void GUI::closeProgram()
{
	timer->Stop();
	aMIVCOTS.stop();
	free(timer);
	frame->Close(true);
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

bool Frame::initFrame(MIVCOTS * aMIVCOTS, std::vector<long>* activeCars, std::vector<long>* displayedCars, std::vector<databaseInfo>* playBackOptions)
{
	this->aMIVCOTS = aMIVCOTS;
	this->activeCars = activeCars;
	this->displayedCars = displayedCars;
	this->playBackOptions = playBackOptions;

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
	mapPanel.initMap(aMIVCOTS, this->displayedCars, &baseLat, &baseLon);
	// graph plotting
	graph = Plotting(this);
	graph.createPlot(aMIVCOTS, this->displayedCars);
	m_mgr.AddPane(graph.getPlot(), wxAuiPaneInfo().Right().
		MinSize(900,1000).
		BestSize(900, 1000).
		MaxSize(900, 1000).
		CloseButton(false));

	createUIPanel();

	// add the panes to the manager
	m_mgr.SetFlags(m_mgr.GetFlags() ^ wxAUI_MGR_LIVE_RESIZE);

	m_mgr.AddPane(mapPanel.getPanel(), wxAuiPaneInfo().Caption(wxT("Map")).
		Center().
		MinSize(200, 200).
		BestSize(200, 200).
		CloseButton(false));
	m_mgr.AddPane(uiPanel, wxAuiPaneInfo().Caption(wxT("UI")).
		Bottom().
		MinSize(200, 200).
		BestSize(200, 200).
		CloseButton(false));
	m_mgr.AddPane(log, wxAuiPaneInfo().Caption(wxT("LOG")).
		Bottom().
		MinSize(200, 200).
		BestSize(200, 200).
		MaxSize(200, 200).
		CloseButton(false));
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

	createPlaybackWidget();
	hbox1->Add(playBackBox, 1, wxALL | wxEXPAND, 15);

	vbox->Add(hbox1, 1, wxALL | wxEXPAND, 15);
	//vbox->Add(hbox2, 1, wxALL | wxEXPAND, 15);


	uiPanel->SetSizer(vbox);
	return false;
}

StatusWidget* Frame::createStatusWidget(long carID)
{
	StatusWidget* statusWidget = new StatusWidget(this);
	statusWidget->initStatusWidget(aMIVCOTS, carID, &baseLat, &baseLon);

	statusWidgets.push_back(statusWidget);

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
		for (StatusWidget* temp : statusWidgets) {
			if (temp->getCarID() == i) {
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
	if (newCars.size() < activeCars->size()) {
		activeCars->clear();	//TODO: do this more efficent
		carComboBox->Clear();
		carCheckListBox->Clear();
		mapPanel.mapRefresh();
	}
	for (long i : newCars) {
		bool found = false;
		for (long j : *activeCars) {
			if (i == j) {
				found = true;
			}
		}
		if (!found) {
			if (i >= 0) {
				carComboBox->Append(std::to_string(i));
				carCheckListBox->Append(std::to_string(i));
			}
			else {
				if (i == std::numeric_limits<long>::min()) {
					carComboBox->Append("Past 0");
					carCheckListBox->Append("Past 0");
				}
				else {
					carComboBox->Append("Past" + std::to_string(-i));
					carCheckListBox->Append("Past" + std::to_string(-i));
				}
			}
			activeCars->push_back(i);
			carCheckListBox->Check(activeCars->size() - 1);
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
		std::string tmp = "Car" + std::to_string(statusWidgets.at(i)->getCarID());
		if (event.GetPane()->caption == tmp) {
			statusWidgets.erase(statusWidgets.begin() + i);
		}
	}
}

int Frame::createDateTimes(long start, long end, wxDateTime * d1, wxDateTime * d2)
{
	wxDateTime::Month month_c;
	switch ((start % 10000) / 100)
	{
	case  1: month_c = wxDateTime::Jan; break;
	case  2: month_c = wxDateTime::Feb; break;
	case  3: month_c = wxDateTime::Mar; break;
	case  4: month_c = wxDateTime::Apr; break;
	case  5: month_c = wxDateTime::May; break;
	case  6: month_c = wxDateTime::Jun; break;
	case  7: month_c = wxDateTime::Jul; break;
	case  8: month_c = wxDateTime::Aug; break;
	case  9: month_c = wxDateTime::Sep; break;
	case 10: month_c = wxDateTime::Oct; break;
	case 11: month_c = wxDateTime::Nov; break;
	case 12: month_c = wxDateTime::Dec; break;
	default: month_c = wxDateTime::Jan;
	}
	d1->SetYear((int)(start / 10000));
	d1->SetMonth(month_c);
	d1->SetDay((int)(start % 100));

	switch ((end % 10000) / 100)
	{
	case  1: month_c = wxDateTime::Jan; break;
	case  2: month_c = wxDateTime::Feb; break;
	case  3: month_c = wxDateTime::Mar; break;
	case  4: month_c = wxDateTime::Apr; break;
	case  5: month_c = wxDateTime::May; break;
	case  6: month_c = wxDateTime::Jun; break;
	case  7: month_c = wxDateTime::Jul; break;
	case  8: month_c = wxDateTime::Aug; break;
	case  9: month_c = wxDateTime::Sep; break;
	case 10: month_c = wxDateTime::Oct; break;
	case 11: month_c = wxDateTime::Nov; break;
	case 12: month_c = wxDateTime::Dec; break;
	default: month_c = wxDateTime::Jan;
	}
	d2->SetYear(end / 10000);
	d2->SetMonth(month_c);
	d2->SetDay(end % 100);
	return 0;
}

long Frame::monthToLong(wxDateTime in)
{
	long result = in.GetYear() * 10000;
	result += in.GetDay();
	switch (in.GetMonth())
	{
	case  wxDateTime::Jan: result += 1 * 100; break;
	case  wxDateTime::Feb: result += 2 * 100; break;
	case  wxDateTime::Mar: result += 3 * 100; break;
	case  wxDateTime::Apr: result += 4 * 100; break;
	case  wxDateTime::May: result += 5 * 100; break;
	case  wxDateTime::Jun: result += 6 * 100; break;
	case  wxDateTime::Jul: result += 7 * 100; break;
	case  wxDateTime::Aug: result += 8 * 100; break;
	case  wxDateTime::Sep: result += 9 * 100; break;
	case  wxDateTime::Oct: result += 10 * 100; break;
	case  wxDateTime::Nov: result += 11 * 100; break;
	case  wxDateTime::Dec: result += 12 * 100; break;
	}
	return result;
}
