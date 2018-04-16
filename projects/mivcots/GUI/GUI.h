#pragma once

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/aui/aui.h>

#include "MapWidget/Map.h"
#include "MIVCOTS.h"
#include <string.h>

#define FRAMERATE 30


class Frame : public wxFrame
{
public:
	//Frame(const wxString& title, const wxPoint& pos, const wxSize& size);
	Frame(wxWindow* parent, MIVCOTS* aMIVCOTS);
	~Frame();
	Map mapPanel;
	//wxTimer *timer;
	
	void onExit(wxCommandEvent &event);
private:
	wxAuiManager m_mgr;
	MIVCOTS* aMIVCOTS;
	
	void onAbout(wxCommandEvent &event);
	void onToggleFullscreen(wxCommandEvent &event);
	
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
	toggleFullscreen
};
wxBEGIN_EVENT_TABLE(GUI, wxApp)
	EVT_TIMER(gui_timer, GUI::update)
	EVT_MENU(wxID_EXIT, GUI::onExit)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_ABOUT, Frame::onAbout)
	EVT_MENU(toggleFullscreen, Frame::onToggleFullscreen)
	//EVT_CLOSE(GUI::OnQuit)
wxEND_EVENT_TABLE()



