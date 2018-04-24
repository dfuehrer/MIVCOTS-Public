#pragma once
#include <wx\wx.h>
#include <wx/dcbuffer.h>

class PictureWindow : public wxWindow
{
public:
	PictureWindow(wxWindow* parent, const wxImage& image);
	PictureWindow(wxWindow * parent, const wxBitmap & image);
	void OnPaint(wxPaintEvent& event);
	void onEraseBackground(wxEraseEvent &event);

	bool setBitmap(wxBitmap img);
private:
	static const int MaxWidth = 1280;
	static const int MaxHeight = 1280;
	wxBitmap Bitmap;
	DECLARE_EVENT_TABLE()
};

