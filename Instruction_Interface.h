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
//
//	File:	Instruction_Interface.java
//
#pragma once

#include "Steps_Table_Model.h"
#include "Stack_Table_Model.h"
#include "Registers_Table_Model.h"
#include "Machine.h"
#include "Mem_Step.h"

#include "Exception_ACOS_Error.h"
#include "Exception_ASIN_Error.h"
#include "Exception_Bit_Error.h"
#include "Exception_DIV_Error.h"
#include "Exception_EXP_Error.h"
#include "Exception_Illegal_Branch.h"
#include "Exception_Illegal_Memory_Use.h"
#include "Exception_Index_Error.h"
#include "Exception_LN_Error.h"
#include "Exception_LOG_Error.h"
#include "Exception_SQRT_Error.h"
#include "Exception_Stack_Underflow.h"
#include "Exception_Stack_Overflow.h"

class Instruction_Interface
{
	public:
		virtual void Run					(Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {};
		virtual void Update_Register_Use	(Mem_Step &s, Registers_Table_Model &reg) {};
		virtual bool Check					(Mem_Step &s, Machine &mem) {	return false;	};
};
