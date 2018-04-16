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

wxPanel * StatusWidget::getPanel()
{
	return panel;
}

bool StatusWidget::initStatusWidget(MIVCOTS * aMIVCOTS)
{
	this->aMIVCOTS = aMIVCOTS;
	panel = new wxPanel(parent, wxID_ANY);
	wxTextCtrl* text1 = new wxTextCtrl(panel, -1, _("Status Widget"),
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER | wxTE_MULTILINE);
	return true;
}
