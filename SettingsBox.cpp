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
// SettingsBox.cpp : implementation file
//

#include "stdafx.h"
#include "CALCViewWin.h"
#include "SettingsBox.h"
#include "Preferences.h"

#include ".\settingsbox.h"


// CSettingsBox dialog

IMPLEMENT_DYNAMIC(CSettingsBox, CDialog)
CSettingsBox::CSettingsBox(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingsBox::IDD, pParent)
{
}

CSettingsBox::~CSettingsBox()
{
}

void CSettingsBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ICCDRIVER, m_iccdriverpath);
	DDX_Control(pDX, IDC_CPLNS, m_cplnspath);
	DDX_Control(pDX, IDC_SHOWALLICC, m_showallicc);
	DDX_Control(pDX, IDC_LOOPPERIOD, m_loopperiod);
	DDX_Control(pDX, IDC_SHOWERRORS, m_showerrors);
}


BEGIN_MESSAGE_MAP(CSettingsBox, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CSettingsBox message handlers

void CSettingsBox::OnBnClickedOk()
{
	Prefs_Set_ICC_Errors(m_showallicc.GetCheck());
	Prefs_Set_Show_Errors(m_showerrors.GetCheck());
	
	
	CString temp;
		
	m_iccdriverpath.GetWindowText(temp);
	Prefs_Set_ICC_Path(temp);
	
	m_cplnspath.GetWindowText(temp);
	Prefs_Set_CPLNS_Path(temp);
	
	int period = max(GetDlgItemInt(IDC_LOOPPERIOD,NULL,false),1);
	Prefs_Set_Period( period );
		
	OnOK();
}

BOOL CSettingsBox::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_showallicc.SetCheck(Prefs_Get_ICC_Errors());
	m_showerrors.SetCheck(Prefs_Get_Show_Errors());
	
	m_iccdriverpath.SetWindowText(Prefs_Get_ICC_Path());
	m_cplnspath.SetWindowText(Prefs_Get_CPLNS_Path());	
	
	SetDlgItemInt(IDC_LOOPPERIOD,Prefs_Get_Period(),false);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

