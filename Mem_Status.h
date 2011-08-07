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
	
	Name: 			Mem_Status.cpp
	Author:			Tod Baudais
					Copyright (C) 2000-2004. All rights reserved.
	
	Date Created:	9/13/2004
	Changes:		-none-
==============================================================================*/
#pragma once

#include	<string>
#include	"Constants.h"
//#include "Machine.h"

/*==============================================================================
==============================================================================*/

class Mem_Status
{
	public:
		enum {
			CAN_CONFIG_VALUE = 1 << 5,
			CAN_CONFIG_CONNECT = 1 << 6,
			CAN_CHANGE_STATUS = 1 << 8,
			
			MODIFIED = 1 << 2,
			BAD = 1 << 8,
			SECURE = 1 << 9,
			ACK = 1 << 10,
			OOS = 1 << 11,
			SHADOW = 1 << 12,
			LIM_HI = 1 << 13,
			LIM_LO = 1 << 14,
			ERROR_ = 1 << 15
		};

	public:

        // Constructors 
							Mem_Status	(void);
							Mem_Status	(int f);
							Mem_Status	(const Mem_Status &rhs);
				
        // Assignment operator
        Mem_Status&			operator =	(const Mem_Status &rhs);		

        // Destructor
		virtual				~Mem_Status	(void);

	private:

		int				_other_status;
		int				_status;
		int				_flags;
       

	public:
	    
		bool		Can_Config()					{	return Get_Flag(CAN_CONFIG_VALUE) || Get_Flag(CAN_CONFIG_CONNECT);	}
		bool		Can_Change_Status ()			{	return Get_Flag(CAN_CHANGE_STATUS);									}
		//bool		Can_Online ()					{	return Get_Flag(CAN_ONLINE);										}


		virtual void			Set_Other_Status			(int bit, bool s);
		virtual void			Set_Other_Status			(int status)	{	_other_status = status;					}
		virtual bool			Get_Other_Status			(int bit)		{	return (_other_status & bit) != 0;		}
		virtual int				Get_Other_Status			(void)					{	return _other_status;					}

	    
		virtual void			Set_Status					(int bit, bool s);
		virtual void			Set_Status					(int status)			{	_status = status;						}
		virtual bool			Get_Status					(int bit)			{	return (_status & bit) != 0;			}
		virtual int				Get_Status					(void)					{	return _status;							}

		virtual bool			Get_Flag					(int bit)		{	return (_flags & bit) != 0;				}
		virtual void			Set_Flag					(int bit, bool s);
		
		
 
		virtual void			Set_Configuration_Value		(std::string s)	{								}
		virtual void			Set_Configuration			(std::string s)	{								}
		virtual std::string		Get_Configuration			(void)			{	return std::string("");		}
    
		virtual void			Set_Value_String			(std::string v)	{								}
		virtual std::string		Get_Value_String			(void)			{	return std::string("");		}

		virtual void			Set_Value					(float v)		{								}    
		virtual float			Get_Value					(void)			{	return 0.0F;				}
        
		//virtual void			Compile						(Machine &machine) = 0;
};

/*==============================================================================
==============================================================================*/
