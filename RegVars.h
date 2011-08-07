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
* 30-Aug-99 | [1.384] Created change log
* 30-Aug-99 | [1.384] Added RegistryGUID
*****************************************************************************/
class RegistryVar {
    public:
	static void setInt(UINT id, int value, HKEY root = HKEY_CURRENT_USER);
	static int getInt(UINT id, int def = 0, HKEY root = HKEY_CURRENT_USER);
	static void setString(UINT id, LPCTSTR s, HKEY root = HKEY_CURRENT_USER);
	static CString getString(UINT id, LPCTSTR def = NULL, HKEY root = HKEY_CURRENT_USER);
	static void setGUID(UINT id, const GUID & value, HKEY root = HKEY_CURRENT_USER);
	static GUID getGUID(UINT id, GUID * def, HKEY root = HKEY_CURRENT_USER);
    protected:
	UINT id;
	HKEY root;
	       };
/*lint -e1735 */
class RegistryInt : public RegistryVar {
     public:
         RegistryInt(UINT n, HKEY r = HKEY_CURRENT_USER ) {id = n; root = r; value = 0; } 
	int value; // the value
	virtual int load(int def = 0);
	virtual void store();
				       };
	    

class RegistryString : public RegistryVar {
     public:
	RegistryString(UINT n, HKEY r = HKEY_CURRENT_USER) {id = n; root = r; }
	CString value; // the value
	virtual CString load(LPCTSTR def = NULL);
	virtual void store();
				       };
#ifdef GUID_DEFINED
class RegistryGUID : public RegistryVar {
    public:
       RegistryGUID(UINT n, HKEY r = HKEY_CURRENT_USER) {id = n; root = r; }
       GUID value; // the value
       virtual GUID * load(GUID * def = NULL);
       virtual void store();
};
#endif

