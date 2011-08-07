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
// CALCViewWinDoc.h : interface of the CCALCViewWinDoc class
//


#pragma once

#include "Stack_Table_Model.h"
#include "Registers_Table_Model.h"
#include "Steps_Table_Model.h"
#include "Machine.h"
#include "Emulator.h"


class CCALCViewWinDoc : public CDocument
{
protected: // create from serialization only
	CCALCViewWinDoc();
	DECLARE_DYNCREATE(CCALCViewWinDoc)

// Attributes
public:

// Operations
public:

// Overrides
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CCALCViewWinDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnControlsPlay();
	afx_msg void OnControlsPlaycontinuous();
	afx_msg void OnControlsStop();
	afx_msg void OnControlsStep();
	afx_msg void OnControlsCalca();
	afx_msg void OnControlsCalc();
	afx_msg void OnControlsMath();
	afx_msg void OnControlsLogic();
	afx_msg void OnUpdateControlsCalca(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControlsCalc(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControlsMath(CCmdUI *pCmdUI);
	afx_msg void OnUpdateControlsLogic(CCmdUI *pCmdUI);

public:
	int					StackGetRowCount		(void)		{	return _stack_table_model.GetRowCount();		}
	std::string			StackGetValueAt			(int row)	{	return _stack_table_model.GetValueAt(row);		}  


	int					StepsGetRowCount		(void)		{	return _machine.Get_Num_Steps();				}
	bool				StepsGetBreak			(int step);
	bool				StepsGetVisited			(int step);
	bool				StepsGetCurrent			(int step);
	bool				StepsGetError			(int step);
	std::string			StepsGetValueAt			(int step);
	
	void				StepsToggleBreak		(int step);
	void				StepsSetValueAt			(int step, std::string value);
	
	void				StepsInsertLine			(int i);
	void				StepsDeleteLine			(int i);
	
	int					RegistersGetRowCount	(void)		{	return _registers_table_model.GetRowCount();		}
	bool				RegistersGetOOS			(int row)	{	return _registers_table_model.Get_OOS(row);	}
	bool				RegistersGetBAD			(int row)	{	return _registers_table_model.Get_BAD(row);	}
	bool				RegistersGetERR			(int row)	{	return _registers_table_model.Get_ERR(row);	}
	bool				RegistersGetSEC			(int row)	{	return _registers_table_model.Get_SEC(row);	}

	bool				RegistersGetModified	(int row)	{	return _registers_table_model.Get_Modified(row);	}	
	std::string			RegistersGetParam		(int row)	{	return _registers_table_model.Get_Param(row);		}
	std::string			RegistersGetValue		(int row)	{	return _registers_table_model.Get_Value(row);		}
	std::string			RegistersGetConfig		(int row)	{	return _registers_table_model.Get_Config(row);		}


	void				RegistersToggleOOS		(int row);
	void				RegistersToggleBAD		(int row);
	void				RegistersToggleERR		(int row);
	void				RegistersToggleSEC		(int row);

	void				RegistersSetModified	(int row)					{	return _registers_table_model.Set_Modified(row, true);	}
	void				RegistersSetValue		(int row, std::string s)	{	return _registers_table_model.Set_Value(row, s);		}
	void				RegistersSetConfig		(int row, std::string s)	{	SetModifiedFlag(true);
																				return _registers_table_model.Set_Config(row, s);		}
	void				Timer_Fired				(void);
private:
	Emulator				_emulator;
	Machine					_machine;
	Stack_Table_Model		_stack_table_model;
	Registers_Table_Model	_registers_table_model;
	Steps_Table_Model		_steps_table_model;
	bool					m_init;
	bool					m_looping;
	int						m_loop_counter;

public:
	afx_msg void OnHelpLicense();
	afx_msg void OnOnlineSettings();
	afx_msg void OnControlsInit();
	afx_msg void OnUpdateControlsInit(CCmdUI *pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	
private:
	bool SaveFile(CString path);
	bool OpenFile(CString path);
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

	afx_msg void OnOnlineConnect();
	afx_msg void OnEditParameters();
	
public:
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
};



