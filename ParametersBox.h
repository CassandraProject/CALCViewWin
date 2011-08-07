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
#pragma once
#include "afxwin.h"
#include "Machine.h"
#include "QuickList.h"
#include <vector>

class CCALCViewWinDoc;

// CParametersBox dialog

class CParametersBox : public CDialog
{
	DECLARE_DYNAMIC(CParametersBox)

public:
	CParametersBox(CWnd* pParent = NULL, Machine *m = NULL, CCALCViewWinDoc *doc = NULL);   // standard constructor
	virtual ~CParametersBox();

// Dialog Data
	enum { IDD = IDD_PARAMETERSBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CQuickList m_paramlist;
	
	std::vector<int>	m_usedregisters;
	
	afx_msg void OnBnClickedOk();
	
	afx_msg LRESULT OnGetListItem(WPARAM wParam, LPARAM lParam);
	
	Machine *m_machine;
	CCALCViewWinDoc *m_doc;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnLvnEndlabeleditParamlist(NMHDR *pNMHDR, LRESULT *pResult);
};

