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
	
	Name: 			Mem_Boolean.cpp
	Author:			Tod Baudais
					Copyright (C) 2000-2004. All rights reserved.
	
	Date Created:	9/13/2004
	Changes:		-none-
==============================================================================*/
#pragma once
#include "Mem_Status.h"

/*==============================================================================
==============================================================================*/

class Mem_Boolean: public Mem_Status
{
	public:
		Mem_Boolean (int flags)
			:	Mem_Status(flags)
		{
			_register = false;
			_configuration = "0";
		}
	    

		void Set_Configuration_Value (std::string configuration)
		{
			Set_Configuration(configuration);
			Set_Value_String(configuration);
		}

		void Set_Configuration (std::string configuration)
		{
			_configuration = configuration;
		}
	    
		std::string Get_Configuration ()
		{
			return _configuration;
		}
	    

		void Set_Value_String(std::string r) {
			try {
				std::stringstream s(r);
				float value;
				
				s >> value;
			
				_register = (int) value != 0;
	            
				Set_Other_Status (MODIFIED, true);
			}
			catch (...) {

			}
		}
	    
		void Set_Value(float r) {
			_register = (r != 0.0F) ? true : false;
	        
			Set_Other_Status (MODIFIED, true);
		}

		std::string Get_Value_String() {
			return (_register == true) ? "1" : "0";
		}
	    
		float Get_Value() {
			return (_register == true) ? 1.0F : 0.0F;
		}

	private:
		bool 			_register;
		std::string		_configuration;

};

