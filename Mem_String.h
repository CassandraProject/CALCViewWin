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
/*==============================================================================
	
	Name: 			Mem_String.cpp
	Author:			Tod Baudais
					Copyright (C) 2000-2004. All rights reserved.
	
	Date Created:	9/13/2004
	Changes:		-none-
==============================================================================*/
#pragma once
#include "Mem_Status.h"
#include "StringUtils.h"
/*==============================================================================
==============================================================================*/

class Mem_String: public Mem_Status
{
    
    public:
		Mem_String (int flags) 
			:	Mem_Status(flags)
		{
			_value = "";
		}

		void Set_Configuration_Value (std::string configuration)
		{
			Set_Configuration(configuration);
		}

		void Set_Configuration (std::string configuration)
		{
			// TODO: Convert to uppercase
			_value = ToUppercase(configuration);
		}
	    
		std::string Get_Configuration ()
		{
			return _value;
		}
	    

		void Set_Value_String(std::string r) {
			throw Exception_Illegal_Memory_Use();
		}
	    
		void Set_Value(float r) {
			throw Exception_Illegal_Memory_Use();
		}
	    
		std::string Get_Value_String() {
			throw Exception_Illegal_Memory_Use();
		}

		float Get_Value() {
			throw Exception_Illegal_Memory_Use();
		}
		
    private:
		std::string 	_value;

};


