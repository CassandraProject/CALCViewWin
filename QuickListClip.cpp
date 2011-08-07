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
// CQuickListClip.cpp : implementation file
//

#include "stdafx.h"
#include "afxadv.h"
#include "afxole.h"
#include "QuickListClip.h"

#include "StringUtils.h"

#include <list>
#include <sstream>

// CCQuickListClip

//IMPLEMENT_DYNCREATE(CQuickListClip, CQuickList)

CQuickListClip::CQuickListClip()
{

}

void	CQuickListClip::Set_Edit_Columns	(int *columns, int count)
{
	for (int i = 0; i < count; ++i)
		mcols.push_back(columns[i]);
}

void	CQuickListClip::Cut	(void)
{
	Copy();
	Clear();
}

void	CQuickListClip::Copy	(void)
{
	std::stringstream ss;
	
	// Assumes m_quicklist is in focus.
	POSITION pos = GetFirstSelectedItemPosition();

	while (pos) {
		int nItem = GetNextSelectedItem(pos);
		
		for (int i = 0; i < mcols.size(); ++i) {
			int col = mcols[i];
			
			CQuickList::CListItemData data = GetItemData(nItem, col);

			ss << LPCTSTR(data.m_text);
			
			// Either a tab or a newline
			if (i == mcols.size()-1)	ss << "\r\n";
			else						ss << "\t";
			
		}
	}
	
	// Write the clipboard
	
	if (OpenClipboard()) {
		EmptyClipboard();
		
		HGLOBAL hClipboardData;
		hClipboardData = GlobalAlloc(GMEM_DDESHARE, ss.str().size()+1);
		
		char *pchardata = (char*) GlobalLock(hClipboardData);
		strcpy(pchardata, ss.str().c_str());
		
		GlobalUnlock(hClipboardData);
		
		SetClipboardData(CF_TEXT,hClipboardData);
		CloseClipboard();
	} else {
		AfxMessageBox(_T("Unable to Copy data to the clipboard."));
	}
	
}

void	CQuickListClip::Paste	(void)
{
	// load clipboard
	std::stringstream ss;
	int col_index = 0;
	int row_index;
	
	if (OpenClipboard()) {
		HANDLE hClipboardData = GetClipboardData(CF_TEXT);
		
		char *pchardata = (char*) GlobalLock(hClipboardData);
		ss << pchardata;
		
		GlobalUnlock(hClipboardData);
		
		CloseClipboard();
	} else {
		AfxMessageBox(_T("Unable to Paste data from the clipboard."));	
	}
	
	std::list<std::string>	data = Tokenize (ss.str(), false);
	std::list<std::string>::iterator dataiter = data.begin();

	// Fill in the selected cells first.
	POSITION pos = GetFirstSelectedItemPosition();
	
	// See if there is a selection
	if (!pos)
		return;

	// For each of the selected positions
	while (pos) {
		// Iterate through the selections
		if (col_index == 0)
			row_index = GetNextSelectedItem(pos);
		
		// Check for the end of the list
		if (dataiter == data.end())
			return;
		
		int col = mcols[col_index];
				
		OnEndEdit(row_index, col, _T(dataiter->c_str()), false, 0);
		++dataiter;
		
		col_index = (++col_index) % mcols.size();
	}
	
	// Continue filling in the remaining items below the last selected row
	
	// Check for the end of the list
	while (1) {
		// Iterate through the rows
		if (col_index == 0)
			++row_index;
		
		if (row_index >= GetItemCount())
			return;
	
		// Check for the end of the list
		if (dataiter == data.end())
			return;		
		
		int col = mcols[col_index];
		
		OnEndEdit(row_index, col, _T(dataiter->c_str()), false, 0);
		++dataiter;
		
		col_index = (++col_index) % mcols.size();
	}
	
	return;
}

void	CQuickListClip::Clear	(void)
{
	// Assumes m_quicklist is in focus.
	POSITION pos = GetFirstSelectedItemPosition();

	while (pos) {
		int nItem = GetNextSelectedItem(pos);
		
		for (int i = 0; i < mcols.size(); ++i) {
			int col = mcols[i];
			
			OnEndEdit(nItem, col, _T(""), false, 0);
		}
	}
}

void	CQuickListClip::SelectAll	(void)
{
	for (int i = 0; i < GetItemCount(); ++i) {
		SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);
	}
}


/*BEGIN_MESSAGE_MAP(CQuickListClip, CCtrlView)
//{{AFX_MSG_MAP(CCQuickListClip)
ON_WM_NCDESTROY()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()*/

