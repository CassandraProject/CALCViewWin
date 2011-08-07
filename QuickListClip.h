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
#include "quicklist.h"
#include <vector>

// CQuickListClip

class CQuickListClip : public CQuickList
{
	//DECLARE_DYNCREATE(CQuickListClip)

public:
	CQuickListClip();           // protected constructor used by dynamic creation
	
	void	Set_Edit_Columns	(int *columns, int count);
	void	Cut		(void);
	void	Copy	(void);
	void	Paste	(void);
	void	Clear	(void);
	void	SelectAll	(void);
	
	std::vector<int>	mcols;
	
	//{{AFX_MSG(CQuickListView)

	//}}AFX_MSG
	
	//DECLARE_MESSAGE_MAP()
};


