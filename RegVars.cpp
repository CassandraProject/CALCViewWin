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
#include "stdafx.h"
#include "registry.h"
#include "regvars.h"

/****************************************************************************
*                             RegistryVar::getInt
* Inputs:
*       UINT id: ID value
*	int def: Default value (= 0)
*	HKEY root: default HKEY_CURRENT_USER
* Result: int
*       
* Effect: 
*       Retrieves the integer value
****************************************************************************/

int RegistryVar::getInt(UINT id, int def, HKEY root)
    {
     DWORD value;
     CString pathname;
     pathname.LoadString(id);
     if(!GetRegistryInt(root, pathname, value))
	return def;
     return (int)value;
    }


/****************************************************************************
*                             RegistryVar::setInt
* Inputs:
*       UINT id: ID to set
*	int value: Value to set
*	HKEY root: default HKEY_CURRENT_USER
* Result: void
*       
* Effect: 
*       Sets the registry key
****************************************************************************/

void RegistryVar::setInt(UINT id, int value, HKEY root)
    {
     CString pathname;

     pathname.LoadString(id);
     SetRegistryInt(root, pathname, value);
    }

/****************************************************************************
*                           RegistryVar::setString
* Inputs:
*       UINT id: ID of variable in 
*	LPCTSTR value: String value to save
*	HKEY root: default HKEY_CURRENT_USER
* Result: void
*       
* Effect: 
*       Stores the string value in the registry
****************************************************************************/

void RegistryVar::setString(UINT id, LPCTSTR value, HKEY root)
    {
     CString pathname;

     pathname.LoadString(id);

     SetRegistryString(root, pathname, value);
    }

/****************************************************************************
*                           RegistryVar::getString
* Inputs:
*       UINT id: ID of subkey
*	LPCTSTR def: Default value, or NULL
*	HKEY root: default HKEY_CURRENT_USER
* Result: CString
*       The string value loaded
* Effect: 
*       Loads the string from the registry
****************************************************************************/

CString RegistryVar::getString(UINT id, LPCTSTR def, HKEY root)
    {
     CString pathname;
     CString value;

     pathname.LoadString(id);

     if(!GetRegistryString(root, pathname, value))
        { /* failed */
	 if(def != NULL)
	    value = def;
         else
	    value = _T("");
	} /* failed */
     return value;
    }

/****************************************************************************
*                            RegistryVar::getGUID
* Inputs:
*       UINT id: ID of variable to load
*	GUID * def: Default, or NULL if no default
*	HKEY root: Root of key
* Result: GUID 
*       The GUID value loaded
* Effect: 
*       Loads a GUID from the Registry
****************************************************************************/

GUID RegistryVar::getGUID(UINT id, GUID * def, HKEY root)
    {
     CString pathname;
     GUID guid;

     pathname.LoadString(id);
     if(!GetRegistryGUID(root, pathname, guid))
	{ /* failed */
	 if(def != NULL)
	    guid = *def;
	 else
	    ::ZeroMemory(&guid, sizeof(GUID));
	} /* failed */
     return guid;
    } // RegistryVar::getGUID

/****************************************************************************
*                            RegistryVar::setGUID
* Inputs:
*       UINT id: String ID of var
*	const GUID &value: GUID to store
*	HKEY root: Root of key (default HKEY_CURRENT_USER)
* Result: void
*       
* Effect: 
*       Stores the GUID
****************************************************************************/

void RegistryVar::setGUID(UINT id, const GUID &value, HKEY root)
    {
     CString pathname;
     pathname.LoadString(id);
     SetRegistryGUID(root, pathname, value);
    } // RegistryVar::setGUID

/****************************************************************************
*                              RegistryInt::load
* Result: BOOL
*       TRUE if loaded successfully
*	FALSE if no load occurred
* Effect: 
*       Loads the 'value'
****************************************************************************/

int RegistryInt::load(int def)
    {
     value = getInt(id, def, root);
     return value;
    }

/****************************************************************************
*                             RegistryInt::store
* Result: void
*       
* Effect: 
*       Stores the 'value'
****************************************************************************/

void RegistryInt::store()
    {
     setInt(id, value, root);
    }

/****************************************************************************
*                            RegistryString::load
* Inputs:
*       LPCTSTR def: Default value, or NULL if no default
* Result: CString
*       The string that was loaded
* Effect: 
*       Loads the string
****************************************************************************/

CString RegistryString::load(LPCTSTR def)
    {
     value = getString(id, def, root);
     return value;
    }

/****************************************************************************
*                            RegistryString::store
* Result: void
*       
* Effect: 
*       Stores the variable
****************************************************************************/

void RegistryString::store()
    {
     setString(id, (LPCTSTR)value, root);
    }

/****************************************************************************
*                             RegistryGUID::load
* Inputs:
*       GUID * def: Default value
* Result: GUID *
*       Pointer to value member
* Effect: 
*       Loads the GUID
****************************************************************************/

GUID * RegistryGUID::load(GUID * def)
    {
     value = getGUID(id, def, root);
     return &value;
    } // RegistryGUID::load

/****************************************************************************
*                             RegistryGUID::store
* Result: void
*       
* Effect: 
*       Stores the GUID in the Registry
****************************************************************************/

void RegistryGUID::store()
    {
     setGUID(id, value, root);
    } // RegistryGUID::store

