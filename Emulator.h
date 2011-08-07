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
	
	Name: 			Emulator.h
	Author:			Tod Baudais
				Copyright (C) 2000-2004. All rights reserved.
	
	Date Created:		9/13/2004
	Changes:		-none-
==============================================================================*/
#pragma once

#include "Machine.h"
#include "Registers_Table_Model.h"
#include "Stack_Table_Model.h"
#include "Steps_Table_Model.h"

/*==============================================================================
==============================================================================*/

class Emulator
{
	private:
		enum {
			PLAY = 1,
			STOP = 2,
			STEP = 3,
			ERROR_ = 4
		};
		
		int			_state;
		bool		_begin;
    
    public:
				Emulator		(void);
    
		void	Set_Play		(void)	{	_state = PLAY;					}
		void	Set_Stop		(void)	{	_state = STOP;	_begin = true;	}
		void	Set_Step		(void)	{	_state = STEP;					}
            
        bool	Is_Error		(void)	{	return _state == ERROR_;		}
            
		void Run (	Machine &machine, 
					Registers_Table_Model &registers,
					Stack_Table_Model &stack,
					Steps_Table_Model &steps,
					bool init);
};

/*==============================================================================
==============================================================================*/


