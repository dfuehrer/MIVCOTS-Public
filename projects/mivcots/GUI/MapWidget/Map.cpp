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
		std::string carPath = std::string(env_p) + std::string("maps\\favicon.png");
		carimg = new wxImage(carPath, wxBITMAP_TYPE_ANY);
		std::string filePath = std::string(env_p) + std::string("maps\\base.png");
		baseStationimg = new wxImage(filePath, wxBITMAP_TYPE_ANY);

		std::string mapPath = std::string(env_p) + std::string("maps/") + mapName + std::string(".png");
		//probably change to tmp
		//imgImg =  new wxImage(mapPath, wxBITMAP_TYPE_PNG);
		wxImage *tmpImg = new wxImage(mapPath, wxBITMAP_TYPE_PNG);
		int xSize = wxSystemSettings::GetMetric(wxSYS_SCREEN_X);
		int ySize = wxSystemSettings::GetMetric(wxSYS_SCREEN_Y);
		wxLogMessage("x size: %d\ty size: %d", xSize,ySize);
		double MaxWidth = xSize * 0.66;
		double MaxHeight = ySize * 0.66;
		
		double X_Ratio = (double)MaxWidth / (double)tmpImg->GetWidth();
		double Y_Ratio = (double)MaxHeight / (double)tmpImg->GetHeight();
		double Ratio = X_Ratio < Y_Ratio ? X_Ratio : Y_Ratio;
		tmpImg->Rescale((int)(Ratio * tmpImg->GetWidth()), (int)(Ratio * tmpImg->GetHeight()), wxIMAGE_QUALITY_HIGH);
		imgImg = (const wxImage*)tmpImg;

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

bool Map::drawCar(double lat, double lon, double angle, int carID)
{
	wxImage tmpimg;
	if (carID == 0) {
		tmpimg = baseStationimg->Copy();
	}
	else {
		tmpimg = carimg->Copy();
	}
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
	//}
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
		double lat = -1;
		double lon = -1;
		double course = -1;
		int rc = SUCCESS;
		std::shared_lock<std::shared_mutex> toLock;
		//wxLogDebug("Map is trying to read cache");
		rc = aMIVCOTS->acquireReadLock(i, &toLock);

		if (rc != SUCCESS) {
			wxLogDebug("Couldn't read cache for car %d", i);
			aMIVCOTS->endCacheRead(i, &toLock);
			continue;
		}

		rc = aMIVCOTS->readCache(&startIter, &endIter, i);
		if (rc == SUCCESS) {
			//for (startIter; startIter != endIter; startIter++) {
				if (((*startIter)->get(std::string(LON_D), &lon) | (*startIter)->get(std::string(LAT_D), &lat) | (*startIter)->get(std::string(HEADING_D), &course)) != SUCCESS) {
				}
				else {
					drawCar(lat, lon, course * 0.01745329252, i);
					refresh();
				}
			//}
			
			/*if (lat != -1 && lon != -1 && course != -1) {
				latTmp = lat / 1000000.0;
				lonTmp = lon / 1000000.0;
				angleTmp = course / 100.0;
			}*/
			
		}
		else {
			wxLogDebug("Couldn't read cache for car %d", i);
		}

		aMIVCOTS->endCacheRead(i, &toLock);
		//wxLogDebug("Map released cache read");
	}
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
	latFactor = (imgImg->GetHeight()) / (coordinates.northEast.first - coordinates.southWest.first);
	lonFactor = (imgImg->GetWidth()) / (coordinates.northEast.second - coordinates.southWest.second);

	latOffset = coordinates.northEast.first;
	lonOffset = coordinates.southWest.second;

	return true;
}
