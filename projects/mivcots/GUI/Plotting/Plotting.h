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
	void createPlot(MIVCOTS* aMIVCOTS, std::vector<long>* activeCars);
	mpWindow *getPlot();
	void Reload();
	void SetDataVector(double a, double b, int carNum);
	std::vector<long>* activeCars;

private:
	mpFXYVector * car0 = new mpFXYVector();
	mpFXYVector * car1 = new mpFXYVector();
	MIVCOTS * aMIVCOTS;
	wxWindow * parent;
//	wxPanel * panel;
	mpWindow   *m_plot;
	wxTextCtrl *m_log;
	std::vector<double> x = { };
	std::vector<double> y = { };
	std::vector<double> m = { };
	std::vector<double> n = { };

};
