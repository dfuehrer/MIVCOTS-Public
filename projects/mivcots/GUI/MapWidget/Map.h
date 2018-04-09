#pragma once
#include <wx/wx.h>
#include <string.h>
#include "PictureWindow.h"
#include <fstream>
#include <sstream>
#include <iomanip>


typedef struct coords_s {
	std::pair <double, double> southWest;
	std::pair <double, double> northEast;
}coords;

class Map
{
public:
	Map(wxWindow *parent);
	Map();
	~Map();

	bool initMap();
	wxPanel* getPanel();

	bool drawCar(double lat, double lon, double angle);



private:
	
	wxPanel * panel;
	const wxImage *imgImg;
	wxBitmap *imgBitmap;
	PictureWindow * picWindow;
	std::string mapName;
	coords coordinates;

	double latFactor;
	double lonFactor;
	double latOffset;
	double lonOffset;

	bool createWidgets();
	bool getCoords(std::string mapName);
	void printCoords();
	bool calcFactors();
	
};

