#pragma once
#include <wx/wx.h>
#include "MIVCOTS.h"

class StatusWidget {
public:
	StatusWidget(wxWindow *parent);
	StatusWidget();
	~StatusWidget();
	
	int getCarID();
	wxPanel* getPanel();
	bool initStatusWidget(MIVCOTS* aMIVCOTS, int carID);
private:
	wxWindow * parent;
	wxPanel * panel;
	MIVCOTS* aMIVCOTS;

	int carID;
};


