/*
* CALCView is a Calc Block Editor
* Copyright (C) 2003
*
* Created by Tod Baudais
*
* This file is part of CALCView.
*
* CALCView is free software: you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as
* published by the Free Software Foundation, either version 3 of
* the License, or (at your option) any later version.
*
* CALCView is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with CALCView.  If not, see <http://www.gnu.org/licenses/>.
*/
// ParametersBox.cpp : implementation file
//

#include "stdafx.h"
#include "CALCViewWin.h"
#include "ParametersBox.h"
#include "Constants.h"
#include "Mem_Status.h"
#include ".\parametersbox.h"
#include <algorithm>

#include "CALCViewWinDoc.h"

// CParametersBox dialog

IMPLEMENT_DYNAMIC(CParametersBox, CDialog)
CParametersBox::CParametersBox(CWnd* pParent /*=NULL*/, Machine *m, CCALCViewWinDoc *doc)
	: CDialog(CParametersBox::IDD, pParent)
{
	m_machine = m;
	m_doc = doc;
	
	for (int i = 0; i < NUM_REGISTERS; ++i) {
		try {
			std::string reg_name = m_machine->Index_To_Name_Memory(i);
			
			Mem_Status *reg = m_machine->Index_To_Memory(i);
			
			// Make sure the parameter is configurable
			if (reg->Can_Config())
				m_usedregisters.push_back(i);
		}
		catch (...) {
		
		}
	}

	std::sort(m_usedregisters.begin(), m_usedregisters.end()/*, std::less<int>*/);
}

CParametersBox::~CParametersBox()
{
}

void CParametersBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PARAMLIST, m_paramlist);
}


BEGIN_MESSAGE_MAP(CParametersBox, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_MESSAGE(WM_QUICKLIST_GETLISTITEMDATA, OnGetListItem)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_PARAMLIST, OnLvnEndlabeleditParamlist)
END_MESSAGE_MAP()


// CParametersBox message handlers

void CParametersBox::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

BOOL CParametersBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_paramlist.InsertColumn(0, "Register", LVCFMT_LEFT, 96);
	m_paramlist.InsertColumn(1, "Configuration", LVCFMT_LEFT, 256);
	m_paramlist.SetItemCountEx(m_usedregisters.size(), LVSICF_NOSCROLL);

	// Set up the style of the list
	ListView_SetExtendedListViewStyle(m_paramlist, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CParametersBox::OnGetListItem(WPARAM wParam, LPARAM lParam)
{
	//CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	//ASSERT(pDoc);
	
    //wParam is a handler to the list
    //Make sure message comes from list box
    ASSERT( (HWND)wParam == m_paramlist.GetSafeHwnd() );

    //lParam is a pointer to the data that 
    //is needed for the element
    CQuickList::CListItemData* data = 
        (CQuickList::CListItemData*) lParam;

    //Get which item and subitem that is asked for.
    int item = data->GetItem();		// Note: Steps start at index 1
    int subItem = data->GetSubItem();
    
	if (subItem == 0) {	
		data->m_allowEdit = false;
		data->m_text = _T(m_machine->Index_To_Name_Memory(m_usedregisters[item]).c_str());
	} else if (subItem == 1) {
		data->m_allowEdit = true;
		
		Mem_Status *reg = m_machine->Index_To_Memory(m_usedregisters[item]);
		std::string reg_value = reg->Get_Configuration();
		
		data->m_text = _T(reg_value.c_str());
	}

    return 0;
}

void CParametersBox::OnLvnEndlabeleditParamlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	//CCALCViewWinDoc *pDoc = dynamic_cast<CCALCViewWinDoc*>(GetDocument());
	//ASSERT(pDoc);

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

    // If pszText is NULL, editing was canceled
    if(pDispInfo->item.pszText != NULL)
    {
		int item = pDispInfo->item.iItem;		// Note: Steps start at index 1
		int subItem = pDispInfo->item.iSubItem;

		if (item != 1 && subItem == 1) {	// Can't change type
			Mem_Status *reg = m_machine->Index_To_Memory(m_usedregisters[item]);
			reg->Set_Configuration(LPCTSTR(pDispInfo->item.pszText));
		}
			
		m_paramlist.RedrawItems(item,item);
    }

    *pResult = 0;
}

