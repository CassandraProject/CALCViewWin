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
	
	Name: 			Emulator.cpp
	Author:			Tod Baudais
				Copyright (C) 2000-2004. All rights reserved.
	
	Date Created:		9/13/2004
	Changes:		-none-
==============================================================================*/
#pragma once

#include "stdafx.h"

#include "Emulator.h"
#include "Preferences.h"
#include "Mem_Step.h"
#include "Instruction_Interface.h"

#include "Exception_Syntax_Error.h"

/*==============================================================================
==============================================================================*/

Emulator::Emulator()
{
	_state = false;
	_begin = true;	
}
    
void Emulator::Run (	Machine &machine, 
						Registers_Table_Model &registers,
						Stack_Table_Model &stack,
						Steps_Table_Model &steps,
						bool init) {        
	try {
	
		if (_begin) {
			// Set initialize flags
			steps.Set_Initializing(init);

			// Get Timini if there is one
			bool timini = false;
			if (machine.Has_Memory (TIMINI))
				timini = machine.Index_To_Memory(TIMINI)->Get_Value() != 0.0F;
	                            
			stack.Clear ();
			steps.Begin_Running_Steps (timini);

			// Clear errors
			machine.Index_To_Memory(PERROR)->Set_Value(0.0F);

			// Done beginning
			_begin = false;
		}

		// Clear all modified flags
		registers.Clear_Modified ();
	
		while (_state == PLAY || _state == STEP) {
			Mem_Step *step = steps.Get_Current_Step();
	        
			// First check for error
			if (step->Get_Error()) {
				_state = ERROR_;
				//Set_Stop ();
				break;
			}
	        
			// Check for break point
			if (_state == PLAY && step->Get_Break() && !step->Get_Visited()) {
				step->Set_Visited(true);
				break;                    
			}
	        
			step->Set_Visited(true);
	        
			Instruction_Interface *step_impl = machine.Index_To_Instruction(step->Get_Opcode());
			step_impl->Run(steps, stack, machine);
	            
			// If we are done
			if (steps.Is_Done()) {
				_begin = true;
				steps.Jump_Step_Backward(1);
				_state = STOP;            
			}
	        
			// If we were just stepping
			if(_state == STEP)
				break;
	        
		}
	}

	catch (Exception_Index_Error &) {
		if (Prefs_Get_Show_Errors())
			AfxMessageBox(_T("ExceptionIndexMsg"));
		_state = ERROR_;
	}
	catch (Exception_LN_Error &) {
		if (Prefs_Get_Show_Errors())
			AfxMessageBox(_T("ExceptionLNMsg"));
		_state = ERROR_;
	}
	catch (Exception_Stack_Overflow &) {
		if (Prefs_Get_Show_Errors())
			AfxMessageBox(_T("ExceptionStackOverflowMsg"));
		_state = ERROR_;
	}
	catch (Exception_LOG_Error &) {
		if (Prefs_Get_Show_Errors())
			AfxMessageBox(_T("ExceptionLOGMsg"));
		_state = ERROR_;
	}
	catch (Exception_Bit_Error &) {
		if (Prefs_Get_Show_Errors())
			AfxMessageBox(_T("ExceptionBitMsg"));
		_state = ERROR_;
	}
	catch (Exception_SQRT_Error &) {
		if (Prefs_Get_Show_Errors())
			AfxMessageBox(_T("ExceptionSQRTMsg"));
		_state = ERROR_;
	}
	catch (Exception_EXP_Error &) {
		if (Prefs_Get_Show_Errors())
			AfxMessageBox(_T("ExceptionEXPMsg"));
		_state = ERROR_;
	}
	catch (Exception_ACOS_Error &) {
		if (Prefs_Get_Show_Errors())
			AfxMessageBox(_T("ExceptionACOSMsg"));
		_state = ERROR_;
	}
	catch (Exception_ASIN_Error &) {
		if (Prefs_Get_Show_Errors())
			AfxMessageBox(_T("ExceptionASINMsg"));
		_state = ERROR_;
	}
	catch (Exception_Syntax_Error &) {
		if (Prefs_Get_Show_Errors())
			AfxMessageBox(_T("ExceptionSyntaxErrMsg"));
		_state = ERROR_;
	}
	catch (Exception_Stack_Underflow &) {
		if (Prefs_Get_Show_Errors())
			AfxMessageBox(_T("ExceptionStackUnderflowMsg"));
		_state = ERROR_;
	}
	catch (Exception_Illegal_Symbol &) {
		if (Prefs_Get_Show_Errors())
			AfxMessageBox(_T("ExceptionIllegalSymbolMsg"));
		_state = ERROR_;
	}
	catch (Exception_Illegal_Branch &) {
		if (Prefs_Get_Show_Errors())
			AfxMessageBox(_T("ExceptionIllegalBranchMsg"));
		_state = ERROR_;
	}
	catch (Exception_DIV_Error &) {
		if (Prefs_Get_Show_Errors())
			AfxMessageBox(_T("ExceptionDIVMsg"));
		_state = ERROR_;
	}
}


/*==============================================================================
==============================================================================*/


