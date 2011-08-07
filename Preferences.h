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

/*==============================================================================
==============================================================================*/

void	Load_Prefs (void);
void	Save_Prefs (void);

/*==============================================================================
==============================================================================*/

CString	Prefs_Get_Name			(void);
void	Prefs_Set_Name			(CString s);

CString	Prefs_Get_Activation	(void);
void	Prefs_Set_Activation	(CString s);

CString	Prefs_Get_ICC_Path		(void);
void	Prefs_Set_ICC_Path		(CString s);

CString	Prefs_Get_CPLNS_Path	(void);
void	Prefs_Set_CPLNS_Path	(CString s);

CString	Prefs_Get_Last_CB		(void);
void	Prefs_Set_Last_CB		(CString s);

CString	Prefs_Get_Last_CP		(void);
void	Prefs_Set_Last_CP		(CString s);

int		Prefs_Get_Accept		(void);
void	Prefs_Set_Accept		(int i);

int		Prefs_Get_Period		(void);
void	Prefs_Set_Period		(int i);

int		Prefs_Get_Show_Errors	(void);
void	Prefs_Set_Show_Errors	(int i);

int		Prefs_Get_ICC_Errors	(void);
void	Prefs_Set_ICC_Errors	(int i);

/*==============================================================================
==============================================================================*/
