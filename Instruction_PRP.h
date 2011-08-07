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
//	File:	Instruction_PRP.java
//

/*
PRP ROxx 
(Propagate Errors)

PRP allows you to propagate certain types of RIxx input errors to the specified ROxx output. The RIxx 
inputs whose error conditions are to be propagated are specified by a mask on the stack, loaded by a 
prior instruction. The mask is popped from the stack after it is used. sptr(after) = sptr(before) - 1.

This instruction sets the Error status bit of the specified output operand if any of the specified RIxx 
inputs are in error. An RIxx input is considered to be in error when:

its Bad status bit is True, or

its Out-of-Service status bit is True, or

its Error status bit is True, or

its OM status field does not have the value "ON_SCAN".

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_PRP: public Instruction_Interface
{
    Instruction_PRP() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        try {
            int r = s->Get_Register1() - RO01;

            if (r < 0 || r > 7) {
                throw Exception_Index_Error();	// error code
            } 

            int status = reg.Index_To_Memory(RI01 + r)->Get_Status();
            int mask = (int) stack.Top(0);

            reg.Index_To_Memory(RO01 + r)->Set_Status(status & mask);
        }
        catch (Exception_Index_Error &e) {
			steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(10);	// error code
            throw e;
        }
        catch (Exception_Stack_Overflow &e) {
			steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(5);	// error code
            throw e;
        }
		steps.Increment_Step();
	}

    void Update_Register_Use (Mem_Step &s, Registers_Table_Model &reg) {
        reg.Use_Register (PERROR);
        if (s.Register_1())	reg.Use_Register (s.Get_Register1());      
        if (s.Register_2())	reg.Use_Register (s.Get_Register2());      
    }

    bool Check (Mem_Step &s, Machine &mem) {
        return 	s.ROxx_1() && s.Empty_2();
    }

};
