#pragma once
#include <wx\wx.h>

class PictureWindow : public wxWindow
{
public:
	PictureWindow(wxWindow* parent, const wxImage& image);
	void OnPaint(wxPaintEvent& event);
private:
	static const int MaxWidth = 320;
	static const int MaxHeight = 240;
	wxBitmap Bitmap;
	DECLARE_EVENT_TABLE()
};
