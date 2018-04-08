#pragma once

#include <wx/wx.h>
#include <wx/app.h>
#include <wx/aui/aui.h>

#include "MapWidget/Map.h"

#include <string.h>


class GUI: public wxApp
{
public:
	virtual bool OnInit();

private:

};

class Frame : public wxFrame
{
public:
	//Frame(const wxString& title, const wxPoint& pos, const wxSize& size);
	Frame(wxWindow* parent);
	~Frame();
	Map mapPanel;

private:
	wxAuiManager m_mgr;
	

	void onExit(wxCommandEvent &event);
	void onAbout(wxCommandEvent &event);
	void onToggleFullscreen(wxCommandEvent &event);
	wxDECLARE_EVENT_TABLE();
};

enum
{
	toggleFullscreen
};

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_EXIT, Frame::onExit)
	EVT_MENU(wxID_ABOUT, Frame::onAbout)
	EVT_MENU(toggleFullscreen, Frame::onToggleFullscreen)
wxEND_EVENT_TABLE()



