#include "StatusWidget.h"

StatusWidget::StatusWidget(wxWindow * parent)
{
	this->parent = parent;
}

StatusWidget::StatusWidget()
{
}

StatusWidget::~StatusWidget()
{
}

int StatusWidget::getCarID()
{
	return carID;
}

wxPanel * StatusWidget::getPanel()
{
	return panel;
}

bool StatusWidget::initStatusWidget(MIVCOTS * aMIVCOTS, int carID)
{
	this->aMIVCOTS = aMIVCOTS;
	this->carID = carID;
	panel = new wxPanel(parent, wxID_ANY);
	wxStaticText* text1 = new wxStaticText(panel, -1, _("Status Widget for carID:"+ std::to_string(carID)),
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER | wxTE_MULTILINE);
	return true;
}
