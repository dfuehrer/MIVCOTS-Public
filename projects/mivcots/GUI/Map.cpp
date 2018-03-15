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
	std::string filePath = "C:/Users/Sean/Documents/MIVCOTS_SOFTWARE/projects/mivcots/GUI/test.png";
	const wxImage *img = new wxImage(filePath, wxBITMAP_TYPE_PNG);
	PictureWindow* picWindow = new PictureWindow(panel, *img);
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
