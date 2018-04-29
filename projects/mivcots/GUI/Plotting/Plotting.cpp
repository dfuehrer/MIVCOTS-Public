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

void Plotting::createPlot(MIVCOTS* aMIVCOTS, std::vector<long>* activeCars) {
	//panel = new wxPanel(parent, wxID_ANY, wxPoint(0, 0), wxSize(500, 500), wxBORDER_RAISED,wxT("Graph"));
	this->aMIVCOTS = aMIVCOTS;
	this->activeCars = activeCars;
	mpLayer *e, *h;

	//mpInfoCoords*c;

	//c= new mpInfoCoords(wxRect(100, 100,150, 50), wxLIGHT_GREY_BRUSH);

	mpScaleX* xaxis = new mpScaleX(wxT("time"), mpALIGN_CENTER, false);
	mpScaleY* yaxis = new mpScaleY(wxT("mph"), mpALIGN_LEFT,false);
	m_plot = new mpWindow(parent, -1, wxPoint(-1, -1), wxSize(1000, 1000), wxSUNKEN_BORDER);
	m_plot->AddLayer(xaxis);
	m_plot->AddLayer(yaxis);

	e = car0;
	h = car1;

	m_plot->EnableDoubleBuffer(true);// eliminating the flicker

	

	car0->SetData(x, y);
	car1->SetData(m, n);

	e->SetPen(wxPen(*wxBLUE, 3, wxSOLID));
	e->SetContinuity(TRUE);

	h->SetPen(wxPen(*wxRED, 3, wxSOLID));
	h->SetContinuity(TRUE);

//	m_plot->AddLayer(c);
	m_plot->AddLayer(e);
	m_plot->AddLayer(h);
	m_plot->Fit();
}

mpWindow* Plotting::getPlot() {
	return m_plot;

}

void Plotting::SetDataVector(double a, double b, int carNum) {// for test

	if (carNum == 1) {
		x.push_back(a);// time 
		y.push_back(b);// speed
		car0->SetData(x, y);

     }
	if (carNum == 2) {
		m.push_back(a);
		n.push_back(b);
		car1->SetData(m, n);
	}

}


void Plotting::Reload() {

	sharedCache<CarData*>::cacheIter iter;
	for (int i : *activeCars) {
		double Speed = 0.0;
		double time = 0.0;
		long timeLong;
		long timeA;
		double sec;

		int rc = SUCCESS;
		std::shared_lock<std::shared_mutex> toLock;
		//wxLogDebug("Map is trying to read cache");
		rc = aMIVCOTS->acquireReadLock(i, &toLock);

		if (rc != SUCCESS) {
			//wxLogDebug("Couldn't read cache for car %d", i);
			aMIVCOTS->endCacheRead(i, &toLock);
			continue;
		}

		rc = aMIVCOTS->readLatestUpdate(i, &iter, 1); // TODO: don't hardcode the update count
		if (rc == SUCCESS) {
			if (((*iter)->get(MPH_D, &Speed) | (*iter)->get(TIME_S, &timeLong)) != SUCCESS) {
			}
			else {
				timeA = convertTimestamp(timeLong);
						sec = timeA * 0.001;
						SetDataVector(sec, Speed, i);
						m_plot->Refresh();
						m_plot->Fit();
			}
		}
		else {
			//wxLogDebug("Couldn't read updates from cache for car %d", i);
		}

		aMIVCOTS->endCacheRead(i, &toLock);
		//wxLogDebug("Map released cache read");
	}



}

