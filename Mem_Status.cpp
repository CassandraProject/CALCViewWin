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

#include "Mem_Status.h"

/*==============================================================================
==============================================================================*/

Mem_Status::Mem_Status(void) 
	:	_other_status	(0),
		_status			(0),
		_flags			(0)	
{

}

Mem_Status::Mem_Status(int f) 
	:	_other_status	(0),
		_status			(0),
		_flags			(f)
{
 
}

Mem_Status::Mem_Status (const Mem_Status &rhs)
{

}
				
Mem_Status& Mem_Status::operator = (const Mem_Status &rhs)
{
	return *this;
}		

Mem_Status::~Mem_Status() 
{
    
}
/*==============================================================================
==============================================================================*/

void Mem_Status::Set_Other_Status (int bit, bool s)
{
	if (s)		_other_status = _other_status | bit;
	else		_other_status = _other_status & ~bit;
}

void Mem_Status::Set_Status (int bit, bool s)
{
	if (s)		_status = _status | bit;
	else		_status = _status & ~bit;
}
	
void Mem_Status::Set_Flag (int bit, bool s)	
{
	if (s)		_flags = _flags |  bit;
	else		_flags = _flags & ~bit;
}

/*==============================================================================
==============================================================================*/

