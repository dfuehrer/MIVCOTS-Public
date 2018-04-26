#include "Plotting.h"
#include <iostream>

Plotting::Plotting(wxWindow *parent) {
	this->parent = parent;
}
Plotting::Plotting()
{
}

Plotting::~Plotting()
{
}

void Plotting::createPlot(MIVCOTS* aMIVCOTS) {
	//panel = new wxPanel(parent, wxID_ANY, wxPoint(0, 0), wxSize(500, 500), wxBORDER_RAISED,wxT("Graph"));
	this->aMIVCOTS = aMIVCOTS;
	mpLayer *e;

	m_plot = new mpWindow(parent, -1, wxPoint(-1, -1), wxSize(1000, 1000), wxSUNKEN_BORDER);
	m_plot->AddLayer(new mpScaleX(wxT("time")));
	m_plot->AddLayer(new mpScaleY(wxT("mph")));

	e = test;
	m_plot->EnableDoubleBuffer(true);// eliminating the flicker

	//x.push_back(2);
	//y.push_back(2);
	test->SetData(x, y);
	e->SetPen(wxPen(*wxRED, 3, wxSOLID));
	e->SetContinuity(TRUE);
	m_plot->AddLayer(e);
	m_plot->Fit();
}

mpWindow* Plotting::getPlot() {
	return m_plot;

}

void Plotting::SetDataVector(std::vector<double>a, std::vector<double>b) {// for test

	x = a;
	y = b;

}


void Plotting::Reload() {

	double Speed = 0.0;
	double time = 0.0;
	long timeLong;
	long timeA;
	double sec;

	int rc = SUCCESS;
	sharedCache<CarData*>::cacheIter iter;
	std::shared_lock<std::shared_mutex> toLock;
	rc = aMIVCOTS->acquireReadLock(0, &toLock);

	rc = aMIVCOTS->readLatestUpdate(0, &iter, 1);

	if (rc == SUCCESS) {
		if (((*iter)->get(MPH_D, &Speed) | (*iter)->get(TIME_S, &timeLong)) != SUCCESS) {
		}
		else {

			timeA = convertTimestamp(timeLong);
			sec = timeA * 0.001;
			x.push_back(sec);
			y.push_back(Speed);
			test->SetData(x, y);
			m_plot->Refresh();
		}
	}
	else {
		wxLogDebug("Couldn't read updates from cache for car %d", 0);
	}
	aMIVCOTS->endCacheRead(0, &toLock);







}

