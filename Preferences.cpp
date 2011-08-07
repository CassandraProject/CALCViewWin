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
#include	"afxwin.h"
#include	"resource.h"

#include	"RegVars.h"
#include	"Preferences.h"

RegistryString		g_Name(IDS_KEY_NAME);
RegistryString		g_Activation(IDS_KEY_ACTIVATION);

RegistryString		g_iccdriver(IDS_KEY_ICCDRVR);
RegistryString		g_cplns(IDS_KEY_CPLNS);

RegistryString		g_last_cp(IDS_KEY_LASTCP);
RegistryString		g_last_cb(IDS_KEY_LASTCB);

RegistryInt			g_accept(IDS_KEY_ACCEPT);
RegistryInt			g_loopperiod(IDS_KEY_LOOP_PERIOD);
RegistryInt			g_showerrors(IDS_KEY_SHOWERRORS);
RegistryInt			g_iccmessages(IDS_KEY_ICCMESSAGES);

/*==============================================================================
==============================================================================*/

void Load_Prefs (void)
{
	g_Name.load();
	g_Activation.load();
	g_iccdriver.load();
	g_cplns.load();
	
	g_last_cp.load();
	g_last_cb.load();

	g_loopperiod.load();
	g_showerrors.load();
	g_iccmessages.load();
	g_accept.load();
	
	// Set defaults
	if (g_iccdriver.value.IsEmpty()) 
		g_iccdriver.value = _T("D:\\opt\\fox\\ciocfg\\api\\iccdrvr.tsk.exe");
	
	if (g_cplns.value.IsEmpty())
		g_cplns.value = _T("D:\\etc\\cplns");
		
	if (g_loopperiod.value <= 0)
		g_loopperiod.value = 1;
}

void Save_Prefs (void)
{
	g_Name.store();
	g_Activation.store();
	g_iccdriver.store();
	g_cplns.store();
	
	g_last_cp.store();
	g_last_cb.store();

	g_loopperiod.store();
	g_showerrors.store();
	g_iccmessages.store();
	g_accept.store();
}

/*==============================================================================
==============================================================================*/

CString	Prefs_Get_Name			(void)			{	return g_Name.value;		}
void	Prefs_Set_Name			(CString s)		{	g_Name.value = s;			}

CString	Prefs_Get_Activation	(void)			{	return g_Activation.value;	}
void	Prefs_Set_Activation	(CString s)		{	g_Activation.value = s;		}

CString	Prefs_Get_ICC_Path		(void)			{	return g_iccdriver.value;	}
void	Prefs_Set_ICC_Path		(CString s)		{	g_iccdriver.value = s;		}

CString	Prefs_Get_Last_CB		(void)			{	return g_last_cb.value;		}
void	Prefs_Set_Last_CB		(CString s)		{	g_last_cb.value = s;		}

CString	Prefs_Get_Last_CP		(void)			{	return g_last_cp.value;		}
void	Prefs_Set_Last_CP		(CString s)		{	g_last_cp.value = s;		}

CString	Prefs_Get_CPLNS_Path	(void)			{	return g_cplns.value;		}
void	Prefs_Set_CPLNS_Path	(CString s)		{	g_cplns.value = s;			}

int		Prefs_Get_Accept		(void)			{	return g_accept.value;		}
void	Prefs_Set_Accept		(int i)			{	g_accept.value = i;			}

int		Prefs_Get_Period		(void)			{	return g_loopperiod.value;	}
void	Prefs_Set_Period		(int i)			{	g_loopperiod.value = i;		}

int		Prefs_Get_Show_Errors	(void)			{	return g_showerrors.value;	}
void	Prefs_Set_Show_Errors	(int i)			{	g_showerrors.value = i;		}

int		Prefs_Get_ICC_Errors	(void)			{	return g_iccmessages.value;	}
void	Prefs_Set_ICC_Errors	(int i)			{	g_iccmessages.value = i;	}

/*==============================================================================
==============================================================================*/

