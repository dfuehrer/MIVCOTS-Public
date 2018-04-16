#pragma once
#include <wx/wx.h>
#include "MIVCOTS.h"

class StatusWidget {
public:
	StatusWidget(wxWindow *parent);
	StatusWidget();
	~StatusWidget();
	
	wxPanel* getPanel();
	bool initStatusWidget(MIVCOTS* aMIVCOTS);
private:
	wxWindow * parent;
	wxPanel * panel;
	MIVCOTS* aMIVCOTS;
};


