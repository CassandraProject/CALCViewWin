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
//	File:	Instruction_PRO.java
//

/*
PRO ROxx 
(Propagate Downstream)

PRO allows you to propagate the cascade acknowledgment from a specified RIxx input to the specified 
ROxx output. The RIxx input index is loaded onto the stack by a prior instruction, and popped from 
the stack when it is used. sptr(after) = sptr(before) - 1.

This instruction copies the PRIBLK initialization acknowledgment bit (the Acknowledge status bit) from 
RIxx to ROxx.

If the popped index value for RIxx is less than 1 or greater than 8, a "-3" (out-of-range syntax error 
value, treated as a run-time error) is written to the PERROR parameter.

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_PRO: public Instruction_Interface
{
    Instruction_PRO() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        try {
            int r = (int) stack.Top(0);

            if (r < 1 || r > 8) {
                throw Exception_Index_Error();	// error code
            } 
            
            int status = reg.Index_To_Memory(RI01 + r - 1)->Get_Status();
            reg.Index_To_Memory(s->Get_Register1())->Set_Status(status);
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
