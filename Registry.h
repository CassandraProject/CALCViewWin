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
/*****************************************************************************
*           Change Log
*  Date     | Change
*-----------+-----------------------------------------------------------------
*  2-May-00 | [2.0.0.907] Created
*  2-May-00 | [2.0.0.907] Added Get/SetWindowPlacement
*****************************************************************************/
BOOL GetRegistryString(HKEY root, const CString &var, CString &val);
BOOL SetRegistryString(HKEY root, const CString &var, const CString &val);
BOOL GetRegistryString(HKEY root, const CString &path, const CString &var, CString &val);
BOOL SetRegistryString(HKEY root, const CString &path, const CString &var, const CString &val);
BOOL GetRegistryString(HKEY root, const CString &path, UINT var, CString &val);
BOOL SetRegistryString(HKEY root, const CString &path, UINT var, const CString &val);

BOOL GetRegistryInt(HKEY root, const CString &var, DWORD &val);
BOOL SetRegistryInt(HKEY root, const CString &var, DWORD val);
BOOL GetRegistryInt(HKEY root, const CString &path, const CString &var, DWORD & val);
BOOL SetRegistryInt(HKEY root, const CString &path, const CString &var, DWORD val);
BOOL GetRegistryInt(HKEY root, const CString &path, UINT var, DWORD &val);
BOOL SetRegistryInt(HKEY root, const CString &path, UINT var, DWORD val);

BOOL GetRegistryInt64(HKEY root, const CString &var, ULONGLONG &val);
BOOL SetRegistryInt64(HKEY root, const CString &var, ULONGLONG val);

BOOL GetRegistryGUID(HKEY root, const CString &var, GUID &val);
BOOL SetRegistryGUID(HKEY root, const CString &var, const GUID &val);

BOOL GetRegistryFont(HKEY root, const CString & var, LPLOGFONT f);
BOOL SetRegistryFont(HKEY root, const CString & var, const LOGFONT * f);

#ifdef IDS_KEY_PLACEMENT_FLAGS
BOOL GetRegistryWindowPlacement(HKEY root, const CString & var, WINDOWPLACEMENT * wp);
BOOL SetRegistryWindowPlacement(HKEY root, const CString & var, const WINDOWPLACEMENT * wp);
#endif

BOOL DeleteRegistryValue(HKEY root, const CString & var);
BOOL DeleteRegistryKey(HKEY root, const CString & keyname);

BOOL GetRegistryKey(HKEY root, const CString & name, HKEY & key);
BOOL FindRegistryKey(HKEY root, const CString & keyname, HKEY & key);

CStringArray * EnumRegistryKeys(HKEY root, const CString & group);
CStringArray * EnumRegistryValues(HKEY root, const CString & group);

CDWordArray * GetRegistryDWordArray(HKEY root, const CString &var);
BOOL SetRegistryDWordArray(HKEY root, const CString & var, CDWordArray & data);

