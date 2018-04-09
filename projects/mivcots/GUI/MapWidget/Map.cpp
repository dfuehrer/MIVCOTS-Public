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
	/*wxButton *button = new wxButton(panel, wxID_EXIT, wxT("Map Test"),
		wxPoint(20, 20));*/
	wxImage::AddHandler(new wxPNGHandler);
	if (const char* env_p = std::getenv("MivcotsResources")) {
		wxLogMessage(_(env_p));
		std::string filePath = std::string(env_p) + std::string("maps/map.png");
		imgImg = new wxImage(filePath, wxBITMAP_TYPE_PNG);
		imgBitmap = new wxBitmap(*imgImg);
		picWindow = new PictureWindow(panel, *imgBitmap);
	
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

bool Map::drawCar(double lat, double lon)
{
		if (const char* env_p = std::getenv("MivcotsResources")) {
			std::string filePath = std::string(env_p) + std::string("maps\\favicon.png");
			wxLogMessage(_(filePath));
			wxImage* tmpimg = new wxImage(filePath, wxBITMAP_TYPE_ANY);
			//rotate image to match angle, probably need to get center too
			int y = 640;	//do math later
			int x = 640;
			wxBitmap tmp =  wxBitmap(*tmpimg);

			wxMemoryDC* dc =  new wxMemoryDC(*imgBitmap);

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
