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
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CALCViewWin.h"

#include "QuickListClip.h"

#include "MainFrm.h"

#include "TestView.h"

#include "StepsView.h"
#include "RegistersView.h"
#include "StackView.h"
#include ".\mainfrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_COMMAND(ID_EDIT_CLEAR, OnEditClear)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR, OnUpdateEditClear)
	ON_COMMAND(ID_EDIT_SELECTALL, OnEditSelectall)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SELECTALL, OnUpdateEditSelectall)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Add Default tool bar
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_FLOAT_MULTI) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// Add Tools bar
	if (!m_wndToolsToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC | CBRS_FLOAT_MULTI) ||
		!m_wndToolsToolBar.LoadToolBar(IDR_TOOLS))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	m_wndToolsToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolsToolBar);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpCreateStruct, CCreateContext* pContext)
{
	//if (CFrameWnd::OnCreateClient(lpCreateStruct,pContext) == FALSE)
	//	return FALSE;
	
		
	// Create Vertical Split Views
	if (!m_wndSplitterVert.CreateStatic(this,1,2,WS_CHILD | WS_VISIBLE | WS_BORDER))
		return FALSE;

	if (!m_wndSplitterVert.CreateView(0,0,RUNTIME_CLASS(CStepsView),CSize(400,100), pContext)) {
		m_wndSplitterVert.DestroyWindow();
		return FALSE;
	}

	// Create Horizontal Split Views
	if (!m_wndSplitterHorz.CreateStatic(&m_wndSplitterVert,2,1, WS_CHILD | WS_VISIBLE | WS_BORDER, m_wndSplitterVert.IdFromRowCol(0, 1)))
		return FALSE;

	if (!m_wndSplitterHorz.CreateView(0,0,RUNTIME_CLASS(CRegistersView),CSize(400,300), pContext) ||
		!m_wndSplitterHorz.CreateView(1,0,RUNTIME_CLASS(CStackView),CSize(400,300), pContext)) {
		m_wndSplitterHorz.DestroyWindow();
		return FALSE;
	}

	return TRUE;
}


BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers


void CMainFrame::OnEditCut()
{
	CWnd *pWnd = GetFocus();
	
	CQuickListClip *table = dynamic_cast<CQuickListClip*>(pWnd);
	CEdit *edit = dynamic_cast<CEdit*>(pWnd);
	
	if (table)		table->Cut();
	else if (edit)	edit->Cut();
}

void CMainFrame::OnUpdateEditCut(CCmdUI *pCmdUI)
{
	CWnd *pWnd = GetFocus();
	
	CQuickListClip *table = dynamic_cast<CQuickListClip*>(pWnd);
	CEdit *edit = dynamic_cast<CEdit*>(pWnd);
	
	if (table || edit)	pCmdUI->Enable(1);
	else				pCmdUI->Enable(0);
}

void CMainFrame::OnEditCopy()
{
	CWnd *pWnd = GetFocus();
	
	CQuickListClip *table = dynamic_cast<CQuickListClip*>(pWnd);
	CEdit *edit = dynamic_cast<CEdit*>(pWnd);
	
	if (table)		table->Copy();
	else if (edit)	edit->Copy();
}

void CMainFrame::OnUpdateEditCopy(CCmdUI *pCmdUI)
{
	CWnd *pWnd = GetFocus();
	
	CQuickListClip *table = dynamic_cast<CQuickListClip*>(pWnd);
	CEdit *edit = dynamic_cast<CEdit*>(pWnd);
	
	if (table || edit)	pCmdUI->Enable(1);
	else				pCmdUI->Enable(0);
}

void CMainFrame::OnEditPaste()
{
	CWnd *pWnd = GetFocus();
	
	CQuickListClip *table = dynamic_cast<CQuickListClip*>(pWnd);
	CEdit *edit = dynamic_cast<CEdit*>(pWnd);
	
	if (table)		table->Paste();
	else if (edit)	edit->Paste();
}

void CMainFrame::OnUpdateEditPaste(CCmdUI *pCmdUI)
{
	CWnd *pWnd = GetFocus();
	
	CQuickListClip *table = dynamic_cast<CQuickListClip*>(pWnd);
	CEdit *edit = dynamic_cast<CEdit*>(pWnd);
	
	if (table || edit)	pCmdUI->Enable(1);
	else				pCmdUI->Enable(0);
}

void CMainFrame::OnEditClear()
{
	CWnd *pWnd = GetFocus();
	
	CQuickListClip *table = dynamic_cast<CQuickListClip*>(pWnd);
	CEdit *edit = dynamic_cast<CEdit*>(pWnd);
	
	if (table)		table->Clear();
	else if (edit)	edit->Clear();
}

void CMainFrame::OnUpdateEditClear(CCmdUI *pCmdUI)
{
	CWnd *pWnd = GetFocus();
	
	CQuickListClip *table = dynamic_cast<CQuickListClip*>(pWnd);
	CEdit *edit = dynamic_cast<CEdit*>(pWnd);
	
	if (table || edit)	pCmdUI->Enable(1);
	else				pCmdUI->Enable(0);
}

void CMainFrame::OnEditSelectall()
{
	CWnd *pWnd = GetFocus();
	
	CQuickListClip *table = dynamic_cast<CQuickListClip*>(pWnd);
	//CEdit *edit = dynamic_cast<CEdit*>(pWnd);
	
	if (table)	table->SelectAll();
}

void CMainFrame::OnUpdateEditSelectall(CCmdUI *pCmdUI)
{
	CWnd *pWnd = GetFocus();
	
	CQuickListClip *table = dynamic_cast<CQuickListClip*>(pWnd);
	//CEdit *edit = dynamic_cast<CEdit*>(pWnd);
	
	if (table)	pCmdUI->Enable(1);
	else		pCmdUI->Enable(0);
}

BOOL CMainFrame::OnCmdMsg	(UINT nID, int nCode, void *pExtra, AFX_CMDHANDLERINFO *pHandlerInfo)
{
	/*for (UINT id = AFX_IDW_PANE_FIRST; id <= AFX_IDW_PANE_LAST; ++id) {
		CWnd *pWnd = GetDescendantWindow(id,TRUE);
		if (pWnd && pWnd != GetActiveView() && pWnd->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
			return true;
	}*/
	
	/*if (GetActiveView())
		GetActiveView()->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);*/
	
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

