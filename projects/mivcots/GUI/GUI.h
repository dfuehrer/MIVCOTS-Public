#pragma once

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/aui/aui.h>

#include "MapWidget/Map.h"
#include "MIVCOTS.h"
#include <string.h>

#define FRAMERATE 30

class GUI: public wxApp
{
public:
	virtual bool OnInit();

private:
	MIVCOTS aMIVCOTS;
};

class Frame : public wxFrame
{
public:
	//Frame(const wxString& title, const wxPoint& pos, const wxSize& size);
	Frame(wxWindow* parent);
	~Frame();
	Map mapPanel;
	wxTimer *timer;
	

private:
	wxAuiManager m_mgr;
	
	

	void onExit(wxCommandEvent &event);
	void onAbout(wxCommandEvent &event);
	void onToggleFullscreen(wxCommandEvent &event);
	void update(wxTimerEvent &event);
	wxDECLARE_EVENT_TABLE();
};

enum
{
	gui_timer = wxID_HIGHEST,
	toggleFullscreen
};

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_EXIT, Frame::onExit)
	EVT_MENU(wxID_ABOUT, Frame::onAbout)
	EVT_MENU(toggleFullscreen, Frame::onToggleFullscreen)
	EVT_TIMER(gui_timer, Frame::update)
wxEND_EVENT_TABLE()



