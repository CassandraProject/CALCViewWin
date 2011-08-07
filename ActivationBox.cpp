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
// ActivationBox.cpp : implementation file
//

#include "stdafx.h"
#include "CALCViewWin.h"
#include "ActivationBox.h"
#include "ActivationCode.h"
#include "Preferences.h"

#include <string>
#include <sstream>

#include ".\activationbox.h"


// CActivationBox dialog

IMPLEMENT_DYNAMIC(CActivationBox, CDialog)
CActivationBox::CActivationBox(CWnd* pParent /*=NULL*/)
	: CDialog(CActivationBox::IDD, pParent)
{

}

CActivationBox::~CActivationBox()
{
}

void CActivationBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT3, m_license);
	DDX_Control(pDX, IDC_CHECK1, m_accept_license);
	DDX_Control(pDX, IDC_EDIT1, m_name);
	DDX_Control(pDX, IDC_EDIT2, m_code);
	DDX_Control(pDX, IDC_EDIT4, m_message);
	DDX_Control(pDX, IDOK, m_ok);
}


BEGIN_MESSAGE_MAP(CActivationBox, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_CHECKCODE, OnBnClickedCheckcode)
END_MESSAGE_MAP()


// CActivationBox message handlers

void CActivationBox::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}

void CActivationBox::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	exit(0);
}

void CActivationBox::OnBnClickedCheckcode()
{
	ActivationCode code_checker;
	
	CString cname;
	CString ccode;
	int caccept;
		
	m_name.GetWindowText(cname);
	m_code.GetWindowText(ccode);
	caccept = m_accept_license.GetCheck();
	
	// Save the prefs
	Prefs_Set_Name (cname);
	Prefs_Set_Activation (ccode);
	Prefs_Set_Accept (caccept);
	
	
	switch (CheckValidCode (cname, ccode, caccept)) {
		case CODE_OK:		m_message.SetWindowText("Valid Code Entered.");
							m_ok.EnableWindow(true);
							break;
							
		case CODE_EXPIRED:	m_message.SetWindowText("Code Expired");
							m_ok.EnableWindow(false);
							break;
							
		case CODE_GOOD_ACCEPT_LICENSE:
							m_message.SetWindowText("Must accept license to finish unlocking software.");
							m_ok.EnableWindow(false);
							break;
							
		case CODE_INAVLID:
		default:			m_message.SetWindowText("Invalid Code. Double check that both the name and code were entered correctly.");
							m_ok.EnableWindow(false);
							break;
	};
	
}

BOOL CActivationBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	std::stringstream ss;
	
ss << "CALCView is Licensed inder the GNU LESSER GENERAL PUBLIC LICENSE VERSION 3.0" << "\r\n";
	
m_license.SetWindowText(ss.str().c_str());


	m_accept_license.SetCheck(Prefs_Get_Accept());
	m_name.SetWindowText(Prefs_Get_Name());
	m_code.SetWindowText(Prefs_Get_Activation());
	
	OnBnClickedCheckcode();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


int CActivationBox::CheckValidCode (CString cname, CString ccode, int license_accept)
{
	ActivationCode code_checker;

	std::string name = LPCTSTR(cname);
	std::string code = LPCTSTR(ccode);
		
	try {
		if (code_checker.Check_Code (name, code)) {
		
			if (!license_accept)
				return CODE_GOOD_ACCEPT_LICENSE;
			else
				return CODE_OK;
				
		} else {
			return CODE_EXPIRED;
		}
	}
	catch (...) {
		return CODE_INAVLID;
	}
}
