#pragma once
#include <wx/wx.h>
#include "MIVCOTS.h"

class StatusWidget {
public:
	StatusWidget(wxWindow *parent);
	StatusWidget();
	~StatusWidget();
	
	long getCarID();
	wxPanel* getPanel();
	bool initStatusWidget(MIVCOTS* aMIVCOTS, long carID);
	int update();
private:
	wxWindow * parent;
	wxPanel * panel;
	MIVCOTS* aMIVCOTS;
	wxStaticText* mphText;
	wxStaticText* tempText;
	wxStaticText* distText;
	wxStaticText* accText;

	
	long carID;
};


