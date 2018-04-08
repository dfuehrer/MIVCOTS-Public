/***************************************************************
 * Name:      gui_testingApp.cpp
 * Purpose:   Code for Application Class
 * Author:     ()
 * Created:   2018-03-20
 * Copyright:  ()
 * License:
 **************************************************************/

#include "gui_testingApp.h"

//(*AppHeaders
#include "gui_testingMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(gui_testingApp);

bool gui_testingApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	gui_testingFrame* Frame = new gui_testingFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
