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


// CActivationBox dialog

class CActivationBox : public CDialog
{
	DECLARE_DYNAMIC(CActivationBox)

public:
	CActivationBox(CWnd* pParent = NULL);   // standard constructor
	virtual ~CActivationBox();

// Dialog Data
	enum { IDD = IDD_ACTIVATIONBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckcode();
	
	
	enum {
		CODE_OK,
		CODE_EXPIRED,
		CODE_GOOD_ACCEPT_LICENSE,
		CODE_INAVLID
	};
	
	static int CheckValidCode (CString cname, CString ccode, int license_accept);
	
	CEdit m_license;
	CButton m_accept_license;
	CEdit m_name;
	CEdit m_code;
	CEdit m_message;
	virtual BOOL OnInitDialog();
	CButton m_ok;
};

