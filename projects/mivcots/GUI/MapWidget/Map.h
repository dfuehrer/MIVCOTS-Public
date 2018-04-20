#pragma once
#include <wx/wx.h>
#include <string.h>
#include "PictureWindow.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <wx/dcbuffer.h>
#include "MIVCOTS.h"



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

	bool initMap(MIVCOTS* aMIVCOTS, std::vector<long>* activeCars);
	wxPanel* getPanel();
	

	bool refresh();
	bool drawCar(double lat, double lon, double angle, int carID);
	coords getCoords();
	bool update();

	double angleTmp;
	double latTmp;
	double lonTmp;
	std::vector<long>* activeCars;


private:
	wxWindow * parent;
	wxPanel * panel;
	const wxImage *imgImg;
	wxBitmap *imgBitmap;
	PictureWindow * picWindow;
	std::string mapName;
	coords coordinates;
	wxBufferedDC* buffDC;
	wxMemoryDC* dc;
	MIVCOTS* aMIVCOTS;
	//wxImage* carImg;
	const wxImage *carimg;
	const wxImage *baseStationimg;

	double latFactor;
	double lonFactor;
	double latOffset;
	double lonOffset;

	bool createWidgets();
	bool getCoords(std::string mapName);
	void printCoords();
	bool calcFactors();
	
};

