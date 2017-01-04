/******************************************************************************
 *
 * Project:  OpenCPN
 * Purpose:  HiRes Plugin
 * Author:   Mike Rossiter
 *
 ***************************************************************************
 *   Copyright (C) 2016 by Mike Rossiter                                   *
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

#include "HiResgui_impl.h"
#include <wx/progdlg.h>
#include <wx/wx.h>
#include "wx/dir.h"
#include <list>
#include "HiRes_pi.h"
#include <wx/filesys.h>
#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include <wx/progdlg.h>
#include <wx/url.h>
#include <memory>

#define BUFSIZE 0x10000

bool Dlg::ExtractZipFiles(const wxString& aZipFile, const wxString& aTargetDir) {
	bool ret = true;

	//wxFileSystem fs;
	std::auto_ptr<wxZipEntry> entry(new wxZipEntry());

	do {

		wxFileInputStream in(aZipFile);

		if (!in) {
			wxLogError(_("Cannot open file '") + aZipFile + _T("'."));
			ret = false;
			break;
		}
		wxZipInputStream zip(in);

		while (entry.reset(zip.GetNextEntry()), entry.get() != NULL) {
			// access meta-data
			wxString name = entry->GetName();
			name = aTargetDir + wxFileName::GetPathSeparator() + name;

			// read 'zip' to access the entry's data
			if (entry->IsDir()) {
				int perm = entry->GetMode();
				wxFileName::Mkdir(name, perm, wxPATH_MKDIR_FULL);
			}
			else {
				zip.OpenEntry(*entry.get());
				if (!zip.CanRead()) {
					wxLogError(_("Cannot read zip entry '") + entry->GetName() + _T("'."));
					ret = false;
					break;
				}

				wxFileOutputStream file(name);

				if (!file) {
					wxLogError(_("Cannot create file '") + name + _T("'."));
					ret = false;
					break;
				}
				zip.Read(file);

			}

		}

	} while (false);

	return ret;
}


bool Dlg::TestForHiRes(){

	wxString zpath = GetOCPN_ExePath();
	zpath.Replace(_T("opencpn.exe"), _T(""));
	zpath += _T("gshhs\\");
	
	wxArrayString str;

	//Get All Files in directory
	//Set wxDIR_FILES flag. There's no wxDIR_DIRS in flags argument, so 
	//this command lists only files under the directory non-recursively.
	wxDir::GetAllFiles(zpath, &str, wxEmptyString, wxDIR_FILES);

	zpath += _T("poly-f-1.dat");
	
	for (unsigned int index = 0; index < str.GetCount(); index++)
	{
		if (str[index] == zpath){			
			return true;
		}
	}
	return false;
}
void Dlg::OnExtract(wxCommandEvent &event)
{
	bool m_bFound;
	m_bFound = TestForHiRes();
	if (m_bFound){
		wxMessageBox(_("HiRes Files are already installed"), _("HiRes Files"));
		plugin->OnHiResDialogClose();
		return;
	}
	
	wxURL url(wxT("http://opencpn.org/ocpn/downloads/data/GshhsHiRes.zip"));

	if (url.GetError() == wxURL_NOERR)
	{
		wxInputStream *in = url.GetInputStream();

		if (in && in->IsOk())
		{
			unsigned char tempbuf[BUFSIZE];
			wxMemoryBuffer buf;

			size_t total_len = in->GetSize();
			size_t data_loaded = 0;
			bool abort = false;

			wxFileName tempFileName;
			wxFile tempFile;
			wxString m_Filename;
			tempFileName.AssignTempFileName(m_Filename, &tempFile);
			wxFileOutputStream* localFile = new wxFileOutputStream(tempFile);

			wxProgressDialog progress(_("Downloading..."), _("Download in progress"), total_len, this, wxPD_CAN_ABORT);

			while (in->CanRead() && !in->Eof() && !abort)
			{
				in->Read(tempbuf, BUFSIZE);
				size_t readlen = in->LastRead();

				//write buffer to file
				localFile->WriteAll(tempbuf, readlen);
				
				if (readlen>0)   {
					buf.AppendData(tempbuf, readlen);
					data_loaded += readlen;
				}

				if (total_len>0)
				{
					// if we know the length of the file, display correct progress
					wxString msg;
					msg.Printf(_("Downloaded %ld of %ld bytes"), data_loaded, total_len);
					abort = !progress.Update(data_loaded, msg);
				}
				else
				{
					// if we don't know the length of the file, just Pulse					
					abort = !progress.Pulse();
				}
			}
			
			if (abort)
			{
				wxMessageBox(_("Download was cancelled"));
				localFile->Close();
				tempFile.Close();
				wxString m_tempFileName = tempFileName.GetFullPath();
				wxRemoveFile(tempFileName.GetFullPath());
			}
			else
			{
				localFile->Close();
				tempFile.Close();
				wxString m_tempFileName = tempFileName.GetFullPath();
				ExtractData(m_tempFileName);
				wxRemoveFile(tempFileName.GetFullPath());
			}
		}
		delete in;		
	}
}

Dlg::Dlg( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : DlgDef( parent, id, title, pos, size, style )
{	
    this->Fit();
    dbg=false; //for debug output set to true
}

void Dlg::ExtractData(wxString filename)
{
	wxMessageBox(_("Download is complete, please wait for confirmation \n that the files have been extracted"), _("Please wait"));

	wxString zpath = GetOCPN_ExePath();
	zpath.Replace(_T("opencpn.exe"), _T(""));
	zpath += _T("gshhs\\");
    
	bool unzip;
	unzip = ExtractZipFiles(filename, zpath);
	
	if (unzip == true) {
		wxMessageBox(_("HiRes files have been copied to OpenCPN \n Please re-start OpenCPN"), _("Success"));
	}
	else {

		wxMessageBox(_("Unable to copy the HiRes files"), _("Extraction failed"));
	}

}

void Dlg::OnClose(wxCloseEvent& event)
{	
	plugin->OnHiResDialogClose();
}

