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
//	File:	Instruction_POP.java
//

/*
POP 
(Pop Stack)

POP removes the value at the top of the stack and discards it. The stack pointer is decremented to 
point to the next value on the stack. sptr(after) = sptr(before) - 1.

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_POP: public Instruction_Interface
{
    Instruction_POP() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        //Mem_Step *s = steps.Get_Current_Step ();
        
        try {
            stack.Pop(1); 
        }
        catch (Exception_Stack_Underflow &e) {
	    steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(6);	// error code
            throw e;
        }         
		steps.Increment_Step();
	}

    void Update_Register_Use (Mem_Step &s, Registers_Table_Model &reg) {
        reg.Use_Register (PERROR);
    }

    bool Check (Mem_Step &s, Machine &mem) {
        return 	s.Empty_1() && s.Empty_2();
    }

};
