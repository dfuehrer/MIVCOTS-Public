/***************************************************************
 * Name:      gui_testingMain.h
 * Purpose:   Defines Application Frame
 * Author:     ()
 * Created:   2018-03-20
 * Copyright:  ()
 * License:
 **************************************************************/

#ifndef GUI_TESTINGMAIN_H
#define GUI_TESTINGMAIN_H

//(*Headers(gui_testingFrame)
#include <wx/aui/aui.h>
#include <wx/frame.h>
#include <wx/led.h>
#include <wx/menu.h>
#include <wx/panel.h>
#include <wx/statusbr.h>
#include <wx/textctrl.h>
//*)

class gui_testingFrame: public wxFrame
{
    public:

        gui_testingFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~gui_testingFrame();

    private:

        //(*Handlers(gui_testingFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnAuiToolBar1Dropdown(wxAuiToolBarEvent& event);
        void OnAuiToolBar1Dropdown1(wxAuiToolBarEvent& event);
        void OnAuiManager1Activated(wxAuiManagerEvent& event);
        //*)

        //(*Identifiers(gui_testingFrame)
        static const long ID_PANEL1;
        static const long ID_TEXTCTRL1;
        static const long ID_LED1;
        static const long ID_MENUITEM1;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(gui_testingFrame)
        wxAuiManager* AuiManager1;
        wxLed* Led1;
        wxPanel* mapPanel;
        wxStatusBar* StatusBar1;
        wxTextCtrl* TextCtrl1;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // GUI_TESTINGMAIN_H
