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

#include <sstream>

class CCALCViewWinDoc;

// CConnectBox dialog

class CConnectBox : public CDialog
{
	DECLARE_DYNAMIC(CConnectBox)

public:
	CConnectBox(CWnd* pParent = NULL, Machine *m = NULL, CCALCViewWinDoc *doc = NULL);   // standard constructor
	virtual ~CConnectBox();

// Dialog Data
	enum { IDD = IDD_CONNECTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	
public:
	Machine *m_machine;
	CCALCViewWinDoc *m_doc;
	
	afx_msg void OnBnClickedGetBlock();
	afx_msg void OnBnClickedAddBlock();
	afx_msg void OnBnClickedModifyBlock();
	afx_msg void OnBnClickedOk();
	
	void RunICC (std::stringstream &in, std::stringstream &out, std::stringstream &error);
	BOOL CreateChildProcess(HANDLE hstdin, HANDLE hstdout, HANDLE herror);
	
	CComboBox m_cp;
	CEdit m_compoundblock;
	CEdit m_messages;
	
	HANDLE	m_hChildStdinRd, m_hChildStdinWr, m_hChildStdinWrDup, 
			m_hChildStdoutRd, m_hChildStdoutWr, m_hChildStdoutRdDup, 
			m_hChildErrorRd, m_hChildErrorWr, m_hChildErrorRdDup; 
	virtual BOOL OnInitDialog();
};

