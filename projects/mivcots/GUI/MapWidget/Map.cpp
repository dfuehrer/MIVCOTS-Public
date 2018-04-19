#include "Map.h"

Map::Map(wxWindow *parent)
{
	this->parent = parent;
}

Map::Map()
{
}

Map::~Map()
{
}

bool Map::initMap(MIVCOTS* aMIVCOTS, std::vector<long>* activeCars)
{
	panel = new wxPanel(parent, wxID_ANY);
	this->activeCars = activeCars;

	mapName = "map1";
	wxImage::AddHandler(new wxPNGHandler);
	this->aMIVCOTS = aMIVCOTS;
	if (const char* env_p = std::getenv("MivcotsResources")) {
		wxLogMessage(_(env_p));
		std::string mapPath = std::string(env_p) + std::string("maps/") + mapName + std::string(".png");
		imgImg = new wxImage(mapPath, wxBITMAP_TYPE_PNG);
		imgBitmap = new wxBitmap(*imgImg);
		picWindow = new PictureWindow(panel, *imgBitmap);

		getCoords(mapName);
		//printCoords();
		calcFactors();
		dc = new wxMemoryDC(*imgBitmap);
		buffDC = new wxBufferedDC(dc, *imgBitmap);

		angleTmp = 0;
		latTmp = 32.235744;
		lonTmp = -110.953771;

	}
	else {
		wxLogFatalError("NO ENVIRONEMENT VARIABLE FOR RESOURCES");
	}

	return true;
}

wxPanel * Map::getPanel()
{
	return panel;
}

bool Map::refresh()
{
	panel->Refresh();
	return true;
}

bool Map::drawCar(double lat, double lon, double angle)
{
	if (const char* env_p = std::getenv("MivcotsResources")) {
		std::string filePath = std::string(env_p) + std::string("maps\\favicon.png");
		wxImage tmpimg = wxImage(filePath, wxBITMAP_TYPE_ANY);

		wxPoint center = wxPoint(tmpimg.GetWidth() / 2, tmpimg.GetHeight() / 2);
		tmpimg = tmpimg.Rotate(angle, center);

		double x = (lon - lonOffset)*lonFactor;
		double y = -(lat - latOffset)*latFactor;

		//wxLogMessage("x=%lf\ty=%lf", x, y);
		wxBitmap tmp = wxBitmap(tmpimg);

		
		picWindow->setBitmap(*imgBitmap);
		
		//translating to the center of the image
		x -= tmp.GetHeight() / 2;
		y -= tmp.GetWidth() / 2;
		buffDC->SelectObject(*imgBitmap);
		buffDC->DrawBitmap(tmp, x, y, true);

		buffDC->SelectObject(wxNullBitmap);

		picWindow->setBitmap(*imgBitmap);
		//free(buffDC);
	}
	return false;
}

coords Map::getCoords()
{
	return coordinates;
}

bool Map::update()
{
	*imgBitmap = wxBitmap(*imgImg);

	sharedCache<CarData*>::cacheIter startIter; 
	sharedCache<CarData*>::cacheIter endIter;
	for(int i : *activeCars) {
		long lat = -1;
		long lon = -1;
		long course = -1;
		int rc = SUCCESS;

		rc = aMIVCOTS->readCache(&startIter, &endIter, i);
		if (rc == SUCCESS) {
			for (startIter; startIter != endIter; startIter++) {
				(*startIter)->get("AD", &lon);
				(*startIter)->get("AE", &lat);
				(*startIter)->get("AG", &course);
			}
			
			if (lat != -1 && lon != -1 && course != -1) {
				latTmp = lat / 1000000.0;
				lonTmp = lon / 1000000.0;
				angleTmp = course / 100.0;
			}
			drawCar(latTmp, lonTmp, angleTmp * 0.01745329252);
			aMIVCOTS->endCacheRead(i);
			refresh();
		}
		else {
				wxLogDebug("Couldn't read cache");
		}
	}
	
	//angleTmp += 1;
	//latTmp += .001;
	//lonTmp += .001;

	//coords coord = getCoords();

	//if (latTmp > coord.northEast.first)
	//	latTmp = coord.southWest.first;
	//if (lonTmp > coord.northEast.second)
	//	lonTmp = coord.southWest.second;
	
	return true;
}

bool Map::createWidgets()
{


	return true;
}

bool Map::getCoords(std::string mapName)
{
	if (const char* env_p = std::getenv("MivcotsResources")) {
		std::string coordsPath = std::string(env_p) + std::string("maps/") + mapName + std::string("_sw_ne_coords.txt");

		std::ifstream inFile(coordsPath);
		std::string value;
		int i = 0;
		while (inFile.good()) {
			getline(inFile, value, ',');
			switch (i) {
			case 0:
				coordinates.southWest.first = std::stod(value);

				break;
			case 1:
				coordinates.southWest.second = std::stod(value);
				break;
			case 2:
				coordinates.northEast.first = std::stod(value);
				break;
			case 3:
				coordinates.northEast.second = std::stod(value);
				break;

			}
			i++;
		}
	}
	return true;
}

void Map::printCoords()
{
	std::stringstream wss;
	wss << std::setprecision(std::numeric_limits<double>::digits10 + 1);

	wss << coordinates.southWest.first;
	wxLogMessage(_(wss.str()));
	wss.str("");

	wss << coordinates.southWest.second;
	wxLogMessage(_(wss.str()));
	wss.str("");

	wss << coordinates.northEast.first;
	wxLogMessage(_(wss.str()));
	wss.str("");

	wss << coordinates.northEast.second;
	wxLogMessage(_(wss.str()));
	wss.str("");
}

bool Map::calcFactors()
{
	latFactor = (1280) / (coordinates.northEast.first - coordinates.southWest.first);
	lonFactor = (1280) / (coordinates.northEast.second - coordinates.southWest.second);

	latOffset = coordinates.northEast.first;
	lonOffset = coordinates.southWest.second;

	return true;
}
