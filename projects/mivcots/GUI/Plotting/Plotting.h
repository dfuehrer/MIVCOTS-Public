#pragma once
#include <wx/wx.h>
#include <math.h>
#include <mathplot.h>
#include "MIVCOTS.h"

class Plotting {
public:
	Plotting(wxWindow *parent);
	Plotting();
	~Plotting();
	void createPlot(MIVCOTS* aMIVCOTS);
	mpWindow *getPlot();
	void Reload();
	void SetDataVector(std::vector<double>a, std::vector<double>b);

private:
	mpFXYVector * test = new mpFXYVector();;
	MIVCOTS * aMIVCOTS;
	wxWindow * parent;
	wxPanel * panel;
	mpWindow   *m_plot;
	wxTextCtrl *m_log;
	std::vector<double> x = { 0 };
	std::vector<double> y = { 0 };
};
