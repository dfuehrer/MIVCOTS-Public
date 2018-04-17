#pragma once
//TODO: Make the log happen before the program closes

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/aui/aui.h>

#include "MapWidget/Map.h"
#include "StatusWidget/StatusWidget.h"
#include "MIVCOTS.h"
#include <string.h>
#include <iostream>
#include <fstream>
#include "serial/serial.h"
#include <vector>

#include <vector>
#define FRAMERATE 30
#define LOG_FREQUENCY 60000 //once a minute


class Frame : public wxFrame
{
public:
	//Frame(const wxString& title, const wxPoint& pos, const wxSize& size);
	Frame(wxWindow* parent);
	~Frame();
	
	bool initFrame(MIVCOTS* aMIVCOTS);
	Map mapPanel;
	StatusWidget statusWidget;
	std::vector<std::string> comObjects;

	//wxTimer *timer;
	
	void onExit(wxCommandEvent &event);
private:
	wxAuiManager m_mgr;
	MIVCOTS* aMIVCOTS;
	wxTimer* logTimer;
	wxTextCtrl* log;
	wxPanel* uiPanel;
	wxComboBox* comComboBox;
	wxButton* openComButton;
	
	
	void onAbout(wxCommandEvent &event);
	void onToggleFullscreen(wxCommandEvent &event);
	void update(wxTimerEvent &event);

	void comStart(wxCommandEvent &event);
	bool createUIPanel();
	
	
	wxDECLARE_EVENT_TABLE();
};
class GUI : public wxApp
{
public:
	wxTimer* timer;

	virtual bool OnInit();
	void OnQuit(wxCloseEvent& evt);

private:
	Frame * frame;
	MIVCOTS aMIVCOTS;

	void update(wxTimerEvent &event);
	void onExit(wxCommandEvent &event);
	
	wxDECLARE_EVENT_TABLE();
};
enum
{
	gui_timer = wxID_HIGHEST,
	log_timer,
	toggleFullscreen,
	comStartButton
};
wxBEGIN_EVENT_TABLE(GUI, wxApp)
	EVT_TIMER(gui_timer, GUI::update)
	EVT_MENU(wxID_EXIT, GUI::onExit)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_ABOUT, Frame::onAbout)
	EVT_MENU(toggleFullscreen, Frame::onToggleFullscreen)
	EVT_TIMER(log_timer, Frame::update)
	EVT_BUTTON(comStartButton, Frame::comStart)

	//EVT_CLOSE(GUI::OnQuit)
wxEND_EVENT_TABLE()



