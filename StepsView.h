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
#include "QuickListClip.h"



// CStepsView form view

class CStepsView : public CFormView
{
	DECLARE_DYNCREATE(CStepsView)

protected:
	CStepsView();           // protected constructor used by dynamic creation
	virtual ~CStepsView();

public:
	enum { IDD = IDD_STEPS_VIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CQuickListClip	m_quicklist;
	CImageList	m_imagelist;

	int			m_dot_active_icon;
	int			m_dot_inactive_icon;
	int			m_dot_cur_active_icon;
	int			m_dot_error_icon;
	int			m_stop_active_icon;
	int			m_stop_inactive_icon;

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnGetListItem(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLvnEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListClick(WPARAM wParam, LPARAM lParam);

protected:
	virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
public:
	afx_msg void OnEditInsert();
	afx_msg void OnEditDelete();
	afx_msg void OnUpdateEditInsert(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditDelete(CCmdUI *pCmdUI);
//	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnTimer(UINT nIDEvent);
protected:
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

};



