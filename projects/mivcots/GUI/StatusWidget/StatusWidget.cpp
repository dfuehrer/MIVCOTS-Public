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

long StatusWidget::getCarID()
{
	return carID;
}

wxPanel * StatusWidget::getPanel()
{
	return panel;
}

bool StatusWidget::initStatusWidget(MIVCOTS * aMIVCOTS, long carID, double* baseLat, double* baseLon)
{
	this->aMIVCOTS = aMIVCOTS;
	this->carID = carID;
	this->baseLat = baseLat;
	this->baseLon = baseLon;
	panel = new wxPanel(parent, wxID_ANY);
	wxBoxSizer* vbox = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* hbox1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* hbox2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* hbox3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* hbox4 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* vbox1 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* vbox2 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* vbox3 = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* vbox4 = new wxBoxSizer(wxVERTICAL);

	wxFlexGridSizer* fgs = new wxFlexGridSizer(2, 2, 10, 10);

	wxTextCtrl* text1 = new wxTextCtrl(panel, -1, _("Status Widget for carID:"+ std::to_string(carID)),
		wxDefaultPosition, wxSize(200, 150),
		wxNO_BORDER | wxTE_MULTILINE);

	mphText = new wxStaticText(panel, -1, "0.0");
	hbox1->Add(mphText);
	hbox1->Add(new wxStaticText(panel, -1, "MPH"));
	vbox1->Add(new wxStaticText(panel, -1, "Speed:"));
	vbox1->Add(hbox1);

	tempText = new wxStaticText(panel, -1, "85.2");
	hbox2->Add(tempText);
	hbox2->Add(new wxStaticText(panel, -1, "F"));
	vbox2->Add(new wxStaticText(panel, -1, "Temp:"));
	vbox2->Add(hbox2);

	distText = new wxStaticText(panel, -1, "200");
	hbox3->Add(distText);
	hbox3->Add(new wxStaticText(panel, -1, "Feet"));
	vbox3->Add(new wxStaticText(panel, -1, "Distance to pit:"));
	vbox3->Add(hbox3);

	accText = new wxStaticText(panel, -1, "9.8");
	hbox4->Add(accText);
	hbox4->Add(new wxStaticText(panel, -1, "m/s^2"));
	vbox4->Add(new wxStaticText(panel, -1, "Average Acceleration:"));
	vbox4->Add(hbox4);

	fgs->Add(vbox1);
	fgs->Add(vbox2);
	fgs->Add(vbox3);
	fgs->Add(vbox4);

	vbox->Add(text1, 1, wxALL | wxEXPAND, 15);
	vbox->Add(fgs, 1, wxALL | wxEXPAND, 15);
	panel->SetSizer(vbox);
	return true;
}

int StatusWidget::update()
{
	sharedCache<CarData*>::cacheIter iter;
	double lat, lon, mph, dist, temp;
	int rc = SUCCESS;
	std::shared_lock<std::shared_mutex> toLock;
	rc = aMIVCOTS->acquireReadLock(carID, &toLock);
	if (rc != SUCCESS) {
		wxLogDebug("Couldn't read cache for car %d in status widget", carID);
		aMIVCOTS->endCacheRead(carID, &toLock);
		return rc;
	}
	rc = aMIVCOTS->readLatestUpdate(carID, &iter, 1);
	
	if (rc == SUCCESS) {
		if (((*iter)->get(LON_D, &lon) | 
			(*iter)->get(LAT_D, &lat) | 
			(*iter)->get(MPH_D, &mph) |
			(*iter)->get(TEMP_D, &temp)
			) != SUCCESS) {
		}
		else {
			long R = 6371000;
			//do things
			if (*baseLat != -1 && *baseLon != -1 ) {	//TODO: Check this math its probably wrong
				double phi_1 = *baseLat * 0.0174533;
				double phi_2 = lat * 0.0174533;

				double del_phi = phi_2 - phi_1;
				double del_lam = (lon - *baseLon) * 0.0174533;
				double a = sin(del_phi / 2.0) *sin(del_phi / 2.0) + cos(phi_1)*cos(phi_2)*sin(del_lam / 2.0)*sin(del_lam / 2.0);
				double c = atan2(sqrt(a), sqrt(1 - a));

				dist = 5280 * 0.000621371 * R *c;
				distText->SetLabel(std::to_string(dist));
			}
			
			mphText->SetLabel(std::to_string(mph));
			tempText->SetLabel(std::to_string(temp));
		}
	}
	else {
		wxLogDebug("Couldn't read updates from cache for car %d in status widget", carID);
	}

	aMIVCOTS->endCacheRead(carID, &toLock);
	
	return SUCCESS;
}
