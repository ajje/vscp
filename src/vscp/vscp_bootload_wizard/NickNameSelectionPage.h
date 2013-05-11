///////////////////////////////////////////////////////////////////////////////
// NickNameSelectionPage.h: interface for the CNickNameSelectionPage class.
//
// This file is part is part of CANAL (CAN Abstraction Layer)
// http://www.vscp.org)
//
// Copyright (C) 2000-2010 Ake Hedman, eurosource, <akhe@eurosource.se>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//
// $RCSfile: NickNameSelectionPage.h,v $                                       
// $Date: 2005/05/31 20:27:18 $                                  
// $Author: akhe $                                              
// $Revision: 1.3 $ 
///////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NICKNAMESELECTIONPAGE_H__F0B85558_A00E_4102_B7C6_A92510FF953C__INCLUDED_)
#define AFX_NICKNAMESELECTIONPAGE_H__F0B85558_A00E_4102_B7C6_A92510FF953C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#if defined(__GNUG__) && !defined(__APPLE__)
    #pragma implementation "BootHexFileInfo.cpp"
    #pragma interface "BootHexFileInfo.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

// for all others, include the necessary headers (this file is usually all you
// need because it includes almost all "standard" wxWindows headers
#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/wizard.h"

#ifndef __WXMSW__
    #include "wiztest.xpm"
    #include "wiztest2.xpm"
#endif

#include "BootControl.h"

// -----------------------------------------------------------------------------
//                                     External stuff
// -----------------------------------------------------------------------------

extern CBootControl gBootCtrl;

/*!
	Select nickname(s) for	node(s) that should be
	bootloaded.
	   .
*/

class wxNickNameSelectionPage : public wxWizardPageSimple
{

public:

    wxNickNameSelectionPage( wxWizard *parent );

	void OnWizardCancel( wxWizardEvent& event );

	void OnWizardPageChanging( wxWizardEvent& event );

	void OnAddButton( wxCommandEvent & event );

	void OnRemoveButton( wxCommandEvent & event );

	void OnTestButton( wxCommandEvent & event );

	virtual bool TransferDataToWindow();

	virtual bool TransferDataFromWindow();

	/*!
		Number of nodes to update
	*/
	unsigned char m_cntNodes;

	/*!
		List with nodes
	*/
	bool m_Node[ 256 ];
							
private:

	/*!
		Interface selection lisbox
	*/
	wxListBox *m_pNnListBox;

	/*!
		Button for adding node id
	*/
	wxButton *m_pAddButton;

	/*!
		Button for remove node id
	*/
	wxButton *m_pRemoveButton;

	/*!
		Button for testing node
	*/
	wxButton *m_pTestButton;


	DECLARE_EVENT_TABLE()

};

#endif // !defined(AFX_NICKNAMESELECTIONPAGE_H__F0B85558_A00E_4102_B7C6_A92510FF953C__INCLUDED_)