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


// CSettingsBox dialog

class CSettingsBox : public CDialog
{
	DECLARE_DYNAMIC(CSettingsBox)

public:
	CSettingsBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSettingsBox();

// Dialog Data
	enum { IDD = IDD_SETTINGSBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_iccdriverpath;
	CEdit m_cplnspath;
	CButton m_showallicc;
	CEdit m_loopperiod;
	CButton m_showerrors;
	
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};

