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
// CALCViewWinDoc.cpp : implementation of the CCALCViewWinDoc class
//

#include "stdafx.h"
#include "CALCViewWin.h"

#include "CALCViewWinDoc.h"
#include ".\calcviewwindoc.h"

#include "Mem_Step.h"

#include "SettingsBox.h"
#include "ConnectBox.h"
#include "ActivationBox.h"
#include "ParametersBox.h"

#include "StringUtils.h"

#include "Preferences.h"

#include <fstream>
#include <sstream>
#include <ctype.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCALCViewWinDoc

IMPLEMENT_DYNCREATE(CCALCViewWinDoc, CDocument)

BEGIN_MESSAGE_MAP(CCALCViewWinDoc, CDocument)
	ON_COMMAND(ID_CONTROLS_PLAY, OnControlsPlay)
	ON_COMMAND(ID_CONTROLS_PLAYCONTINUOUS, OnControlsPlaycontinuous)
	ON_COMMAND(ID_CONTROLS_STOP, OnControlsStop)
	ON_COMMAND(ID_CONTROLS_STEP, OnControlsStep)
	ON_COMMAND(ID_CONTROLS_CALCA, OnControlsCalca)
	ON_COMMAND(ID_CONTROLS_CALC, OnControlsCalc)
	ON_COMMAND(ID_CONTROLS_MATH, OnControlsMath)
	ON_COMMAND(ID_CONTROLS_LOGIC, OnControlsLogic)
	ON_UPDATE_COMMAND_UI(ID_CONTROLS_CALCA, OnUpdateControlsCalca)
	ON_UPDATE_COMMAND_UI(ID_CONTROLS_CALC, OnUpdateControlsCalc)
	ON_UPDATE_COMMAND_UI(ID_CONTROLS_MATH, OnUpdateControlsMath)
	ON_UPDATE_COMMAND_UI(ID_CONTROLS_LOGIC, OnUpdateControlsLogic)
	ON_COMMAND(ID_HELP_LICENSE, OnHelpLicense)
	ON_COMMAND(ID_ONLINE_SETTINGS, OnOnlineSettings)
	ON_COMMAND(ID_CONTROLS_INIT, OnControlsInit)
	ON_UPDATE_COMMAND_UI(ID_CONTROLS_INIT, OnUpdateControlsInit)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_ONLINE_CONNECT, OnOnlineConnect)
	ON_COMMAND(ID_EDIT_PARAMETERS, OnEditParameters)
END_MESSAGE_MAP()


// CCALCViewWinDoc construction/destruction

CCALCViewWinDoc::CCALCViewWinDoc()
	:	_registers_table_model(&_machine),
		_steps_table_model(&_machine,&_registers_table_model),
		m_init(false),
		m_looping(false),
		m_loop_counter(0)
{
	Load_Prefs ();
}

CCALCViewWinDoc::~CCALCViewWinDoc()
{
	Save_Prefs();
}

BOOL CCALCViewWinDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// Check for a valid code
	if (CActivationBox::CheckValidCode (Prefs_Get_Name(), Prefs_Get_Activation(), Prefs_Get_Accept()) != CActivationBox::CODE_OK) {
		CActivationBox activationBox(NULL);
		int nRetCode = activationBox.DoModal();
	}

	return TRUE;
}




// CCALCViewWinDoc serialization

void CCALCViewWinDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CCALCViewWinDoc diagnostics

#ifdef _DEBUG
void CCALCViewWinDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCALCViewWinDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CCALCViewWinDoc commands



void CCALCViewWinDoc::OnControlsPlay()
{
	_emulator.Set_Play();
	_emulator.Run(_machine,_registers_table_model,_stack_table_model,_steps_table_model,m_init);
	UpdateAllViews(NULL, UPDATE_ALL_TABLE_CONTENTS);
	
	if (_emulator.Is_Error())
		m_looping = false;
}

void CCALCViewWinDoc::OnControlsPlaycontinuous()
{
	m_looping = true;
}

void CCALCViewWinDoc::OnControlsStop()
{
	// TODO: Add your command handler code here
	m_looping = false;
	
	_emulator.Set_Stop();
	_emulator.Run(_machine,_registers_table_model,_stack_table_model,_steps_table_model,m_init);
	UpdateAllViews(NULL, UPDATE_ALL_TABLE_CONTENTS);
}

void CCALCViewWinDoc::OnControlsStep()
{
	// TODO: Add your command handler code here
	_emulator.Set_Step();
	_emulator.Run(_machine,_registers_table_model,_stack_table_model,_steps_table_model,m_init);
	UpdateAllViews(NULL, UPDATE_ALL_TABLE_CONTENTS);
}

void CCALCViewWinDoc::OnControlsCalca()
{
	// TODO: Add your command handler code here
	_machine.Set_Up_Memory_CALCA();
	UpdateAllViews(NULL, UPDATE_STEPS_TABLE_SIZE);
}

void CCALCViewWinDoc::OnControlsCalc()
{
	// TODO: Add your command handler code here
	_machine.Set_Up_Memory_CALC();
	UpdateAllViews(NULL, UPDATE_STEPS_TABLE_SIZE);
}

void CCALCViewWinDoc::OnControlsMath()
{
	// TODO: Add your command handler code here
	_machine.Set_Up_Memory_MATH();
	UpdateAllViews(NULL, UPDATE_STEPS_TABLE_SIZE);
}

void CCALCViewWinDoc::OnControlsLogic()
{
	// TODO: Add your command handler code here
	_machine.Set_Up_Memory_LOGIC();
	UpdateAllViews(NULL, UPDATE_STEPS_TABLE_SIZE);
}

void CCALCViewWinDoc::OnUpdateControlsCalca(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (_machine.Get_Type() == Machine::CALCA)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CCALCViewWinDoc::OnUpdateControlsCalc(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (_machine.Get_Type() == Machine::CALC)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CCALCViewWinDoc::OnUpdateControlsMath(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (_machine.Get_Type() == Machine::MATH)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}

void CCALCViewWinDoc::OnUpdateControlsLogic(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	if (_machine.Get_Type() == Machine::LOGIC)
		pCmdUI->SetCheck(TRUE);
	else
		pCmdUI->SetCheck(FALSE);
}




bool				CCALCViewWinDoc::StepsGetBreak		(int step)	
{	
	return _steps_table_model.Get_Step(step)->Get_Break();			
}

bool				CCALCViewWinDoc::StepsGetVisited	(int step)	
{	
	return _steps_table_model.Get_Step(step)->Get_Visited();			
}

bool				CCALCViewWinDoc::StepsGetCurrent	(int step)
{
	return _steps_table_model.Get_Current_Step_Index() == step;

}

bool				CCALCViewWinDoc::StepsGetError		(int step)
{
	return _steps_table_model.Get_Step(step)->Get_Error();	
}
	
std::string			CCALCViewWinDoc::StepsGetValueAt	(int step)	
{	
	return _steps_table_model.Get_Step(step)->Get_Configuration();	
}

void				CCALCViewWinDoc::StepsToggleBreak	(int step)
{
	if (_steps_table_model.Get_Step(step)->Get_Break())
		_steps_table_model.Get_Step(step)->Set_Break(false);
	else
		_steps_table_model.Get_Step(step)->Set_Break(true);	
}

void				CCALCViewWinDoc::StepsSetValueAt	(int step, std::string value)
{
	_steps_table_model.Get_Step(step)->Set_Configuration(value);
	_steps_table_model.Compile_Step (step);
	UpdateAllViews(NULL, UPDATE_REGISTER_TABLE_SIZE);
		
	SetModifiedFlag(true);
}


void				CCALCViewWinDoc::StepsInsertLine	(int i)
{	
	_steps_table_model.Insert_Line	(i);

	UpdateAllViews(NULL, UPDATE_STEPS_TABLE_SIZE);
	UpdateAllViews(NULL, UPDATE_REGISTER_TABLE_SIZE);
	UpdateAllViews(NULL, UPDATE_ALL_TABLE_CONTENTS);
}

void				CCALCViewWinDoc::StepsDeleteLine	(int i)
{	
	_steps_table_model.Delete_Line	(i);
	
	UpdateAllViews(NULL, UPDATE_STEPS_TABLE_SIZE);
	UpdateAllViews(NULL, UPDATE_REGISTER_TABLE_SIZE);
	UpdateAllViews(NULL, UPDATE_ALL_TABLE_CONTENTS);
}


void				CCALCViewWinDoc::RegistersToggleOOS		(int row)
{
	if (_registers_table_model.Get_OOS(row))
		_registers_table_model.Set_OOS(row,false);
	else
		_registers_table_model.Set_OOS(row,true);
}

void				CCALCViewWinDoc::RegistersToggleBAD		(int row)
{
	if (_registers_table_model.Get_BAD(row))
		_registers_table_model.Set_BAD(row,false);
	else
		_registers_table_model.Set_BAD(row,true);
}

void				CCALCViewWinDoc::RegistersToggleERR		(int row)
{
	if (_registers_table_model.Get_ERR(row))
		_registers_table_model.Set_ERR(row,false);
	else
		_registers_table_model.Set_ERR(row,true);
}

void				CCALCViewWinDoc::RegistersToggleSEC		(int row)
{
	if (_registers_table_model.Get_SEC(row))
		_registers_table_model.Set_SEC(row,false);
	else
		_registers_table_model.Set_SEC(row,true);
}


void				CCALCViewWinDoc::Timer_Fired				(void)
{
	if (m_looping) {
		m_loop_counter += 1;
		
		if (m_loop_counter >= Prefs_Get_Period()) {
			OnControlsPlay();
			m_loop_counter = 0;
		}
	} else {
		m_loop_counter = 0;
	}
}

void CCALCViewWinDoc::OnOnlineSettings()
{
	// TODO: Add your command handler code here

	CSettingsBox settingsBox(NULL);

	int nRetCode = settingsBox.DoModal();

	// Check for OK
	if (nRetCode == IDOK) {

	}

}


void CCALCViewWinDoc::OnHelpLicense()
{
	CActivationBox activationBox(NULL);
	int nRetCode = activationBox.DoModal();

}
void CCALCViewWinDoc::OnControlsInit()
{
	m_init = !m_init;
}

void CCALCViewWinDoc::OnUpdateControlsInit(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_init);
}

void CCALCViewWinDoc::OnFileOpen()
{
	CFileDialog openDlg(	true,
							NULL,
							NULL,
							OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
							"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||",
							NULL,
							0
						);


	if (openDlg.DoModal() == IDOK) {
		OpenFile(openDlg.GetPathName());
	}

}

void CCALCViewWinDoc::OnFileSave()
{
	CString path = GetPathName();
	
	if (path.IsEmpty()) {
		OnFileSaveAs();
	} else {
		SaveFile(path);
	}
}

void CCALCViewWinDoc::OnFileSaveAs()
{
	CFileDialog saveDlg(	false,
							NULL,
							NULL,
							OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
							"Text Files (*.txt)|*.txt|All Files (*.*)|*.*||",
							NULL,
							0
						);

	if (saveDlg.DoModal() == IDOK) {
		SaveFile(saveDlg.GetPathName());
	}
	
	
}

bool CCALCViewWinDoc::SaveFile(CString path)
{
	std::stringstream ss;
	
	for (int i = 0; i < NUM_REGISTERS; ++i) {
		try {
			std::string reg_name = _machine.Index_To_Name_Memory(i);
			
			Mem_Status *reg = _machine.Index_To_Memory(i);
			std::string reg_value = reg->Get_Configuration();
			
			// Make sure the parameter is configurable
			if (reg->Can_Config()) {
				
				// Add space at the beginning if it's not NAME
				if (i != NAME)	
					reg_name = " " + reg_name;
				
				// Add padding after regname
				while (reg_name.size() < 8)
					reg_name = reg_name + " ";
			
				// Add remainder of string
				ss << reg_name << " = " << reg_value << std::endl;
			}
		}
		catch (...) {
		
		}
	}
	
	// Add the last line
	ss << "END" << std::endl;
	
	std::ofstream f;
	f.open(LPCTSTR(path), std::ios::out);
	f.write(ss.str().c_str(), ss.str().size());
	f.close();
	
	// Update path
	SetPathName(path, true);
	SetModifiedFlag(false);

	return true;
}

bool CCALCViewWinDoc::OpenFile(CString path)
{
	std::ifstream f;
	f.open(LPCTSTR(path), std::ios::in);
	
	char buffer[1024];
	
	while (1) {
		f.getline(buffer, 1024);
		
		// Make sure the line was read
		if (f.eof() || f.bad() || f.fail())
			break;
		
		std::string s(buffer);
		std::string::size_type location = s.find_first_of("=");
		
		// If there is an equals sign
		if (location != std::string::npos) {
			std::string reg_name = Trim(s.substr(0,location));
			std::string reg_config = Trim(s.substr(location + 2));
							
			// We have to change the type of the block if we get the TYPE param
			if (reg_name == "TYPE") {
				if (reg_config == "CALC")		OnControlsCalc();
				else if (reg_config == "CALCA")	OnControlsCalca();
				else if (reg_config == "MATH")	OnControlsMath();
				else if (reg_config == "LOGIC")	OnControlsLogic();
			}
			
			// Set the configuration
			try {
				int i = _machine.Name_To_Index_Memory(reg_name);
				Mem_Status *reg = _machine.Index_To_Memory(i);
				reg->Set_Configuration(reg_config);
			}
			catch (...) {
			
			}
		}
		
	};
	
	f.close();
	
	// Put everything in a good state
	_steps_table_model.Compile_Steps();
	_steps_table_model.Begin_Running_Steps (false);
	_stack_table_model.Clear ();
	_registers_table_model.Refresh_Registers();
	_emulator.Set_Stop();
	
	// Redraw
	UpdateAllViews(NULL, UPDATE_ALL_TABLE_CONTENTS);
	
	// Update path
	SetPathName(path, true);
	SetModifiedFlag(false);
	
	return true;
}

BOOL CCALCViewWinDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	OpenFile(lpszPathName);

	return TRUE;
}

BOOL CCALCViewWinDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
	SaveFile(lpszPathName);

	return CDocument::OnSaveDocument(lpszPathName);
}

void CCALCViewWinDoc::OnOnlineConnect()
{
	// TODO: Add your command handler code here
	CConnectBox connectBox(NULL,&_machine,this);

	int nRetCode = connectBox.DoModal();

	// Check for OK
	if (nRetCode == IDOK) {

	}
}

void CCALCViewWinDoc::OnEditParameters()
{
	// TODO: Add your command handler code here
	CParametersBox connectBox(NULL,&_machine, this);

	int nRetCode = connectBox.DoModal();

	_steps_table_model.Compile_Steps();
	
	UpdateAllViews(NULL, UPDATE_STEPS_TABLE_SIZE);
	UpdateAllViews(NULL, UPDATE_REGISTER_TABLE_SIZE);
	UpdateAllViews(NULL, UPDATE_ALL_TABLE_CONTENTS);
}

void CCALCViewWinDoc::OnPrint(CDC* pDC, CPrintInfo * pInfo)
{
	const int FONTHEIGHT = 35;
		
	// Set metric mapping
	pDC->SetMapMode(MM_LOMETRIC);

	CFont fontreg;
	fontreg.CreateFont(
		FONTHEIGHT,                // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial");                  // lpszFacename
		
	CFont fontbold;
	fontbold.CreateFont(
		FONTHEIGHT,                // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Arial");                  // lpszFacename
		
	// Select the new font
	CFont *pOldFont = pDC->SelectObject(&fontbold);
	
	// Convert the page rectangle
	CRect rcRect = pInfo->m_rectDraw;
	pDC->DPtoLP(&rcRect);
	
	// Lay out the page
	const int HEADER_HEIGHT = 200;
	const int LEFT_MARGIN = 150;
	int column_width = (rcRect.right - rcRect.left - LEFT_MARGIN) / 3;
	int param_width = column_width / 4;
	
	
	// Draw date and time and block
	Mem_Status *reg = _machine.Index_To_Memory(NAME);
	std::string reg_value = reg->Get_Configuration();
	
	pDC->TextOut(LEFT_MARGIN,-FONTHEIGHT, reg_value.c_str());	
			
	CTime theTime = CTime::GetCurrentTime();
	CString date = theTime.Format("%c");

	pDC->TextOut(LEFT_MARGIN,-2*FONTHEIGHT, date);
	
	// Calculate a lineheight
	int lineheight = (rcRect.top - rcRect.bottom - HEADER_HEIGHT) / 50;
	
	int xpos, ypos;
	xpos = LEFT_MARGIN;
	ypos = -HEADER_HEIGHT;
	
	// Draw steps first
	for (int i = STEP01; i <= STEP50; ++i) {
		try {
			std::string reg_name = _machine.Index_To_Name_Memory(i);
			
			Mem_Status *reg = _machine.Index_To_Memory(i);
			std::string reg_value = "= " + reg->Get_Configuration();
			
			// Make sure the parameter is configurable
			if (reg->Can_Config()) {
				pDC->SelectObject(&fontbold);
				pDC->TextOut(xpos, ypos, reg_name.c_str());
				
				pDC->SelectObject(&fontreg);
				pDC->TextOut(xpos + param_width, ypos, reg_value.c_str());
				
				ypos -= lineheight;
				
				/*if (ypos < rcRect.bottom + lineheight)	{
					xpos += column_width;
					ypos = -HEADER_HEIGHT;
				}*/
			}
		}
		catch (...) {
		
		}
	}
	
	xpos += column_width;
	ypos = -HEADER_HEIGHT;
					
	for (int i = 0; i < STEP01-1; ++i) {
		try {
			std::string reg_name = _machine.Index_To_Name_Memory(i);
			
			Mem_Status *reg = _machine.Index_To_Memory(i);
			std::string reg_value = "= " + reg->Get_Configuration();
			
			// Make sure the parameter is configurable
			if (reg->Can_Config()) {
				pDC->SelectObject(&fontbold);
				pDC->TextOut(xpos, ypos, reg_name.c_str());
				
				pDC->SelectObject(&fontreg);
				pDC->TextOut(xpos + param_width, ypos, reg_value.c_str());
				
				ypos -= lineheight;
				
				if (ypos < rcRect.bottom + lineheight)	{
					xpos += column_width;
					ypos = -HEADER_HEIGHT;
				}
			}
		}
		catch (...) {
		
		}
	}
	
	
	// Draw the program steps
	/*for (int step = 1; step <= StepsGetRowCount(); ++step) {
		int step_index = step - 1;
	
		// Draw steps
		std::stringstream ss;
		if (step <= 9)	ss << "STEP0" << step;
		else			ss << "STEP" << step;
		pDC->TextOut(STEP_MARGIN,-step_index * lineheight - HEADER_HEIGHT, ss.str().c_str());
		
		// Draw statement
		std::string value = StepsGetValueAt(step);
		pDC->TextOut(STATEMENT_MARGIN,-step_index * lineheight - HEADER_HEIGHT, value.c_str());
	}*/
	
	// Reselct the previous font
	pDC->SelectObject(pOldFont);
	
}

