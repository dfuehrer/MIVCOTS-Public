#include "Map.h"

Map::Map(wxWindow *parent)
{
	panel = new wxPanel(parent, wxID_ANY);
}

Map::Map()
{
}

Map::~Map()
{
}

bool Map::initMap()
{
	mapName = "map1";
	wxImage::AddHandler(new wxPNGHandler);

	if (const char* env_p = std::getenv("MivcotsResources")) {
		wxLogMessage(_(env_p));
		std::string mapPath = std::string(env_p) + std::string("maps/") + mapName + std::string(".png");
		imgImg = new wxImage(mapPath, wxBITMAP_TYPE_PNG);
		imgBitmap = new wxBitmap(*imgImg);
		picWindow = new PictureWindow(panel, *imgBitmap);

		getCoords(mapName);
		//printCoords();
		calcFactors();

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

bool Map::drawCar(double lat, double lon, double angle)
{
	if (const char* env_p = std::getenv("MivcotsResources")) {
		std::string filePath = std::string(env_p) + std::string("maps\\favicon.png");
		wxImage* tmpimg = new wxImage(filePath, wxBITMAP_TYPE_ANY);

		wxPoint center = wxPoint(tmpimg->GetWidth() / 2, tmpimg->GetHeight() / 2);
		*tmpimg = tmpimg->Rotate(angle, center);

		double x = (lon - lonOffset)*lonFactor;
		double y = -(lat - latOffset)*latFactor;

		wxLogMessage("x=%lf\ty=%lf", x, y);
		wxBitmap tmp = wxBitmap(*tmpimg);

		wxMemoryDC* dc = new wxMemoryDC(*imgBitmap);

		//translating to the center of the image
		x -= tmp.GetHeight() / 2;
		y -= tmp.GetWidth() / 2;
		dc->DrawBitmap(tmp, x, y);

		dc->SelectObject(wxNullBitmap);

		picWindow->setBitmap(*imgBitmap);
	}
	return false;
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
