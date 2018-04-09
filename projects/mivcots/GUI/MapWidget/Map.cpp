#include "Map.h"

Map::Map(wxWindow *parent)
{
	panel = new wxPanel(parent, wxID_ANY);
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
		const wxImage *img = new wxImage(filePath, wxBITMAP_TYPE_PNG);
		PictureWindow* picWindow = new PictureWindow(panel, *img);
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

bool Map::createWidgets()
{
	return true;
}
