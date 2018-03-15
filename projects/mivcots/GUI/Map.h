#pragma once
#include <wx/wx.h>
#include <string.h>
#include "PictureWindow.h"



class Map
{
public:
	Map(wxWindow *parent);
	~Map();

	bool initMap();
	wxPanel* getPanel();

private:
	wxPanel * panel;
	bool createWidgets();
};

