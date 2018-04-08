/***************************************************************
 * Name:      gui_testingMain.cpp
 * Purpose:   Code for Application Frame
 * Author:     ()
 * Created:   2018-03-20
 * Copyright:  ()
 * License:
 **************************************************************/

#include "gui_testingMain.h"
#include <wx/msgdlg.h>

//(*InternalHeaders(gui_testingFrame)
#include <wx/intl.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(gui_testingFrame)
const long gui_testingFrame::ID_PANEL1 = wxNewId();
const long gui_testingFrame::ID_TEXTCTRL1 = wxNewId();
const long gui_testingFrame::ID_LED1 = wxNewId();
const long gui_testingFrame::ID_MENUITEM1 = wxNewId();
const long gui_testingFrame::idMenuAbout = wxNewId();
const long gui_testingFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(gui_testingFrame,wxFrame)
    //(*EventTable(gui_testingFrame)
    //*)
END_EVENT_TABLE()

gui_testingFrame::gui_testingFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(gui_testingFrame)
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetClientSize(wxSize(420,450));
    AuiManager1 = new wxAuiManager(this, wxAUI_MGR_DEFAULT);
    mapPanel = new wxPanel(this, ID_PANEL1, wxPoint(338,182), wxDefaultSize, wxTAB_TRAVERSAL, _T("ID_PANEL1"));
    AuiManager1->AddPane(mapPanel, wxAuiPaneInfo().Name(_T("Map")).DefaultPane().Caption(_("Map")).CaptionVisible().Position(1).Right());
    TextCtrl1 = new wxTextCtrl(this, ID_TEXTCTRL1, _("Text"), wxPoint(141,124), wxSize(96,431), 0, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    AuiManager1->AddPane(TextCtrl1, wxAuiPaneInfo().Name(_T("PaneName")).DefaultPane().Caption(_("Pane caption")).CaptionVisible().Left().BestSize(wxSize(96,431)));
    Led1 = new wxLed(this,ID_LED1,wxColour(128,128,128),wxColour(0,255,0),wxColour(255,0,0),wxDefaultPosition,wxDefaultSize);
    Led1->Disable();
    Led1->SwitchOn();
    AuiManager1->AddPane(Led1, wxAuiPaneInfo().Name(_T("PaneName")).DefaultPane().Caption(_("Pane caption")).CaptionVisible().Left().BestSize(wxSize(18,103)));
    AuiManager1->Update();
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, ID_MENUITEM1, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);

    AuiManager1->Connect(wxEVT_AUI_PANE_ACTIVATED,(wxObjectEventFunction)&gui_testingFrame::OnAuiManager1Activated,0,this);
    Connect(ID_MENUITEM1,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&gui_testingFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&gui_testingFrame::OnAbout);
    //*)
}

gui_testingFrame::~gui_testingFrame()
{
    //(*Destroy(gui_testingFrame)
    //*)
}

void gui_testingFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void gui_testingFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void gui_testingFrame::OnAuiToolBar1Dropdown(wxAuiToolBarEvent& event)
{
}

void gui_testingFrame::OnAuiManager1Activated(wxAuiManagerEvent& event)
{
}
