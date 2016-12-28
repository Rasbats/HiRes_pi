///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Oct  8 2012)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "HiResgui.h"

///////////////////////////////////////////////////////////////////////////

DlgDef::DlgDef( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( -1,-1 ), wxDefaultSize );
	
	bSframe = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer9111;
	bSizer9111 = new wxBoxSizer( wxVERTICAL );
	
	m_staticText1511 = new wxStaticText( this, wxID_ANY, _("Utility for adding HiRes coastline files"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1511->Wrap( -1 );
	m_staticText1511->SetFont(wxFont(10, 70, 90, 92, false, wxEmptyString));
	
	bSizer9111->Add( m_staticText1511, 0, wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );	
	wxBoxSizer* bSizer1311;
	bSizer1311 = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* bSizer71;
	bSizer71 = new wxBoxSizer( wxHORIZONTAL );	
	m_button3111 = new wxButton( this, wxID_ANY, _("Extract Coastline Files"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer71->Add( m_button3111, 0, wxALL, 5 );
	
	
	bSizer1311->Add( bSizer71, 1, wxALIGN_CENTER, 5 );	
	bSizer9111->Add( bSizer1311, 0, 0, 5 );
	
	bSframe->Add( bSizer9111, 1, wxEXPAND, 5 );
	
	this->SetSizer( bSframe );
	this->Layout();
	
	this->Centre( wxBOTH );
	
	// Connect Events
	m_button3111->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgDef::OnExtract ), NULL, this );
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DlgDef::OnClose) );
}

DlgDef::~DlgDef()
{
	// Disconnect Events
	m_button3111->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DlgDef::OnExtract ), NULL, this );
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( DlgDef::OnClose ) );
}

