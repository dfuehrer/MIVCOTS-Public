#pragma once
//TODO: Make the log happen before the program closes

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/aui/aui.h>
#include <wx/spinctrl.h>
#include <wx/datetimectrl.h>
#include <wx/timectrl.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>

#include "MapWidget/Map.h"
#include "StatusWidget/StatusWidget.h"
#include "MIVCOTS.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include "serial/serial.h"
#include "Utilities.h"
#include <vector>
#include "Plotting.h"
#define FRAMERATE 30
#define LOG_FREQUENCY 60000 //once a minute
#define DEFAULT_SERIAL "COM43"


class Frame : public wxFrame
{
public:
	//Frame(const wxString& title, const wxPoint& pos, const wxSize& size);
	Frame(wxWindow* parent);
	~Frame();
	Plotting graph;
	Map mapPanel;
	//StatusWidget statusWidget;
	std::vector<StatusWidget> statusWidgets;
	std::vector<std::string> comObjects;
	std::vector<long>* activeCars;

	bool initFrame(MIVCOTS* aMIVCOTS, std::vector<long>* activeCars, std::vector<long>* displayedCars, std::vector<databaseInfo>* playBackOptions);
	void checkForNewCars();
	void checkForNewCarsTimer(wxTimerEvent &event);
	void onEraseBackground(wxEraseEvent &event);
	//wxTimer *timer;
	//void onExit(wxCommandEvent &event);
private:
	wxAuiManager m_mgr;
	MIVCOTS* aMIVCOTS;
	wxTimer* logTimer;
	wxTimer* carCheckTimer;
	wxTextCtrl* log;
	wxPanel* uiPanel;
	wxComboBox* comComboBox;
	wxButton* openComButton;
	wxComboBox* carComboBox;
	wxButton* changeCarButton;
	wxCheckListBox* carCheckListBox;
	bool carComboOpen = false;
	bool playComboOpen = false;
	std::vector<long>* displayedCars;

	std::vector<databaseInfo>* playBackOptions;
	wxComboBox* playIdComboBox;
	wxBoxSizer* playBackBox = new wxBoxSizer(wxHORIZONTAL);
	wxDatePickerCtrl* playDateStartSpinBox;
	wxDatePickerCtrl* playDateEndSpinBox;
	wxTimePickerCtrl* playTimeStartSpinBox;
	wxTimePickerCtrl* playTimeEndSpinBox;
	std::vector<int> playBackCars;
	std::vector<long> playBackStartDates;
	std::vector<long> playBackEndDates;
	int maxTime;
	int minTime;


	void onAbout(wxCommandEvent &event);
	void onToggleFullscreen(wxCommandEvent &event);
	void update(wxTimerEvent &event);
	void onCarCombo(wxCommandEvent &event);
	void onCarComboClose(wxCommandEvent &event);
	void comStart(wxCommandEvent &event);
	void carSelect(wxCommandEvent &event);
	bool createUIPanel();
	StatusWidget* createStatusWidget(long carID);
	bool createStatusWidgets();
	void onCheck(wxCommandEvent &event);
	bool createPlaybackWidget();
	bool updatePlayBackWidget();
	void onPlayComboOpen(wxCommandEvent &event);
	void onPlayComboClose(wxCommandEvent &event);
	void onPlayBackCombo(wxCommandEvent &event);

	void onPlayStartDateOpen(wxCommandEvent &event);
	void onPlayStartDateClose(wxCommandEvent &event);
	void onPlayBackStartDate(wxDateEvent &event);
	void onPlayBackStartDate2(wxDateTime d1);
	void onPlayBackEndDate2(wxDateTime d1, wxDateTime d2);

	void paneClosed(wxAuiManagerEvent& event);

	int createDateTimes(long start, long end, wxDateTime* d1, wxDateTime* d2);
	long monthToLong(wxDateTime in);
	wxDECLARE_EVENT_TABLE();
};
class GUI : public wxApp
{
public:
	wxTimer * timer;

	virtual bool OnInit();
	void OnQuit(wxCloseEvent& evt);

private:
	Frame * frame;
	MIVCOTS aMIVCOTS;
	std::vector<long> activeCars;
	std::vector<long> displayedCars;
	std::vector<databaseInfo> playBackOptions;
	//std::vector<CarData> playBackCars;

	void update(wxTimerEvent &event);
	void onExit(wxCommandEvent &event);
	

	wxDECLARE_EVENT_TABLE();
};
enum
{
	gui_timer = wxID_HIGHEST,
	log_timer,
	toggleFullscreen,
	checkCarTimer,
	comStartButton,
	carSelectButton,
	carCombo,
	carCheckList,
	auiManager,
	playBackIdCombo,
	playDateStartSpinBoxID,
	playDateEndSpinBoxID,
	playTimeStartSpinBoxID,
	playTimeEndSpinBoxID
};
wxBEGIN_EVENT_TABLE(GUI, wxApp)
	EVT_TIMER(gui_timer, GUI::update)
	EVT_MENU(wxID_EXIT, GUI::onExit)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_ABOUT, Frame::onAbout)
	EVT_MENU(toggleFullscreen, Frame::onToggleFullscreen)
	EVT_TIMER(log_timer, Frame::update)
	EVT_TIMER(checkCarTimer, Frame::checkForNewCarsTimer)
	EVT_BUTTON(comStartButton, Frame::comStart)
	EVT_BUTTON(carSelectButton, Frame::carSelect)
	EVT_AUI_PANE_CLOSE(Frame::paneClosed)
	EVT_COMBOBOX_DROPDOWN(carCombo, Frame::onCarCombo)
	EVT_COMBOBOX_CLOSEUP(carCombo, Frame::onCarComboClose)
	EVT_ERASE_BACKGROUND(Frame::onEraseBackground)
	EVT_CHECKLISTBOX(carCheckList, Frame::onCheck)

	EVT_COMBOBOX_DROPDOWN(playBackIdCombo, Frame::onPlayComboOpen)
	EVT_COMBOBOX_CLOSEUP(playBackIdCombo, Frame::onPlayComboClose)
	EVT_COMBOBOX(playBackIdCombo, Frame::onPlayBackCombo)
	EVT_DATE_CHANGED(playDateStartSpinBoxID, Frame::onPlayBackStartDate)
	//EVT_COMBOBOX_CLOSEUP(playDateStartSpinBoxID, Frame::onPlayStartDateClose)
	//EVT_COMBOBOX(playDateStartSpinBoxID, Frame::onPlayBackStartDate)


	//EVT_CLOSE(GUI::OnQuit)
wxEND_EVENT_TABLE()



