/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  HiRes Plugin
 * Author:   Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2016 by Mike Rossiter                                *
 *   $EMAIL$                                                               *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************
 */

#include "wx/wxprec.h"

#ifndef  WX_PRECOMP
  #include "wx/wx.h"
#endif //precompiled headers

#include "HiRes_pi.h"
#include "HiResgui_impl.h"
#include "HiResgui.h"


class HiRes_pi;

// the class factories, used to create and destroy instances of the PlugIn

extern "C" DECL_EXP opencpn_plugin* create_pi(void *ppimgr)
{
    return new HiRes_pi(ppimgr);
}

extern "C" DECL_EXP void destroy_pi(opencpn_plugin* p)
{
    delete p;
}

//---------------------------------------------------------------------------------------------------------
//
//    HiRes PlugIn Implementation
//
//---------------------------------------------------------------------------------------------------------

#include "icons.h"

//---------------------------------------------------------------------------------------------------------
//
//          PlugIn initialization and de-init
//
//---------------------------------------------------------------------------------------------------------

HiRes_pi::HiRes_pi(void *ppimgr)
      :opencpn_plugin_17 (ppimgr)
{
      // Create the PlugIn icons
      initialize_images();
	  m_bShowHiRes = false;
}

HiRes_pi::~HiRes_pi(void)
{
     delete _img_hires_pi;
     delete _img_hires;
     
}

int HiRes_pi::Init(void)
{
      AddLocaleCatalog( _T("opencpn-HiRes_pi") );

      // Set some default private member parameters
      m_hr_dialog_x = 40;
      m_hr_dialog_y = 80;
      ::wxDisplaySize(&m_display_width, &m_display_height);

      //    Get a pointer to the opencpn display canvas, to use as a parent for the POI Manager dialog
      m_parent_window = GetOCPNCanvasWindow();

      //    Get a pointer to the opencpn configuration object
      m_pconfig = GetOCPNConfigObject();

      //    And load the configuration items
      LoadConfig();

      //    This PlugIn needs a toolbar icon, so request its insertion
	if(m_bHiResShowIcon)
      m_leftclick_tool_id  = InsertPlugInTool(_T(""), _img_hires, _img_hires, wxITEM_CHECK,
            _("HiRes"), _T(""), NULL,
             HIRES_TOOL_POSITION, 0, this);

      m_pDialog = NULL;

      return (
              WANTS_TOOLBAR_CALLBACK    |
              INSTALLS_TOOLBAR_TOOL     |          
              WANTS_CONFIG           
           );
}

bool HiRes_pi::DeInit(void)
{
      //    Record the dialog position
      if (NULL != m_pDialog)
      {
            //Capture dialog position
            wxPoint p = m_pDialog->GetPosition();
            SetHiResDialogX(p.x);
            SetHiResDialogY(p.y);
            m_pDialog->Close();
            delete m_pDialog;
            m_pDialog = NULL;

			m_bShowHiRes = false;
			SetToolbarItemState( m_leftclick_tool_id, m_bShowHiRes );
      }	
    
    SaveConfig();

    RequestRefresh(m_parent_window); // refresh main window

    return true;
}

int HiRes_pi::GetAPIVersionMajor()
{
      return MY_API_VERSION_MAJOR;
}

int HiRes_pi::GetAPIVersionMinor()
{
      return MY_API_VERSION_MINOR;
}

int HiRes_pi::GetPlugInVersionMajor()
{
      return PLUGIN_VERSION_MAJOR;
}

int HiRes_pi::GetPlugInVersionMinor()
{
      return PLUGIN_VERSION_MINOR;
}

wxBitmap *HiRes_pi::GetPlugInBitmap()
{
      return _img_hires_pi;
}

wxString HiRes_pi::GetCommonName()
{
      return _("HiRes");
}


wxString HiRes_pi::GetShortDescription()
{
      return _("HiRes Coastlines");
}

wxString HiRes_pi::GetLongDescription()
{
      return _("Extract HiRes coastline files");
}

int HiRes_pi::GetToolbarToolCount(void)
{
      return 1;
}

void HiRes_pi::SetColorScheme(PI_ColorScheme cs)
{
      if (NULL == m_pDialog)
            return;

      DimeWindow(m_pDialog);
}

void HiRes_pi::OnToolbarToolCallback(int id)
{
    
	if(NULL == m_pDialog)
      {
            m_pDialog = new Dlg(m_parent_window);
            m_pDialog->plugin = this;
            m_pDialog->Move(wxPoint(m_hr_dialog_x, m_hr_dialog_y));			
      }

	  m_pDialog->Fit();
	  //Toggle 
	  m_bShowHiRes = !m_bShowHiRes;	  

      //    Toggle dialog? 
      if(m_bShowHiRes) {
          m_pDialog->Show();         
      } else
          m_pDialog->Hide();
     
      // Toggle is handled by the toolbar but we must keep plugin manager b_toggle updated
      // to actual status to ensure correct status upon toolbar rebuild
      SetToolbarItemState( m_leftclick_tool_id, m_bShowHiRes);

      RequestRefresh(m_parent_window); // refresh main window
}

bool HiRes_pi::LoadConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T( "/Settings/HiRes_pi" ) );
			pConf->Read ( _T( "ShowHiResIcon" ), &m_bHiResShowIcon, 1 );
           
            m_hr_dialog_x =  pConf->Read ( _T ( "DialogPosX" ), 40L );
            m_hr_dialog_y =  pConf->Read ( _T ( "DialogPosY" ), 140L );
         
            if((m_hr_dialog_x < 0) || (m_hr_dialog_x > m_display_width))
                  m_hr_dialog_x = 40;
            if((m_hr_dialog_y < 0) || (m_hr_dialog_y > m_display_height))
                  m_hr_dialog_y = 140;

            return true;
      }
      else
            return false;
}

bool HiRes_pi::SaveConfig(void)
{
      wxFileConfig *pConf = (wxFileConfig *)m_pconfig;

      if(pConf)
      {
            pConf->SetPath ( _T ( "/Settings/HiRes_pi" ) );
			pConf->Write ( _T ( "ShowHiResIcon" ), m_bHiResShowIcon );
          
            pConf->Write ( _T ( "DialogPosX" ),   m_hr_dialog_x );
            pConf->Write ( _T ( "DialogPosY" ),   m_hr_dialog_y );
            
            return true;
      }
      else
            return false;
}

void HiRes_pi::OnHiResDialogClose()
{
    m_bShowHiRes = false;
    SetToolbarItemState( m_leftclick_tool_id, m_bShowHiRes);
    m_pDialog->Hide();
    SaveConfig();

    RequestRefresh(m_parent_window); // refresh main window
}


