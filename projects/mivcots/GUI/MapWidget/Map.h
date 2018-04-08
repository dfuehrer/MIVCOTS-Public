#pragma once
#include <wx/wx.h>
#include <string.h>
#include "PictureWindow.h"



class Map
{
public:
	Map(wxWindow *parent);
	Map();
	~Map();

	bool initMap();
	wxPanel* getPanel();

	bool drawCar(double lat, double lon);



private:
	
	wxPanel * panel;
	bool createWidgets();
	const wxImage *imgImg;
	wxBitmap *imgBitmap;
	PictureWindow * picWindow;
	
};

