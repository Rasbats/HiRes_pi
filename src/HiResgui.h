///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __HRGUI_H__
#define __HRGUI_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/textctrl.h>
#include <wx/sizer.h>
#include <wx/choice.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/statbox.h>
#include <wx/event.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class DlgDef
///////////////////////////////////////////////////////////////////////////////
class DlgDef : public wxDialog 
{
	private:
	
	protected:
		wxStaticText* m_staticText1511;
		wxChoice* m_Nship;
		wxButton* m_button3111;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnExtract( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		
	
	public:
		wxBoxSizer* bSframe;

		DlgDef( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("HiRes"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 438,210 ), long style = wxCAPTION|wxCLOSE_BOX|wxDIALOG_NO_PARENT|wxRESIZE_BORDER|wxSYSTEM_MENU ); 
		~DlgDef();
	
};

#endif //__HRGUI_H__
