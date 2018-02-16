#pragma once

#include <wx/wx.h>
#include <wx/app.h>


class GUI: public wxApp
{
public:
	virtual bool OnInit();

private:

};

class Frame : public wxFrame
{
public:
	Frame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
	void onExit(wxCommandEvent &event);
	void onAbout(wxCommandEvent &event);

	wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(Frame, wxFrame)
	EVT_MENU(wxID_EXIT, Frame::onExit)
	EVT_MENU(wxID_ABOUT, Frame::onAbout)
wxEND_EVENT_TABLE()

