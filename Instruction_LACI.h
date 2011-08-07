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
//	File:	Instruction_LACI.java
//

/*
LACI Mxx 
(Load Accumulator Indirect)

LACI Mxx loads the accumulator with the contents of a memory register indexed by the contents 
of the memory register specified by the Mxx operand, where xx is an index from 01 to 24 (or 1 
to 24). Neither memory register is affected.

If Mxx does not contain an integer value, the truncated contents are used as the index. 

If the specified memory contents are less than 1 or greater than 24, a "10" (index run-time 
error) is written to the PERROR parameter at run-time. sptr(after) = sptr(before) + 1.

CHECKED: March 28, 2003
*/ 
#include    "Instruction_Interface.h"

struct Instruction_LACI: public Instruction_Interface
{
    Instruction_LACI() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        try {
            int i = (int) reg.Index_To_Memory(s->Get_Register1())->Get_Value();
            if (i < 1 || i > reg.Get_Num_Memory()) 
                throw Exception_Index_Error();	// error code

            float r = (float) reg.Index_To_Memory(M01 + i - 1)->Get_Value();
            stack.Push(r);
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
        return 	s.Mxx_1() && s.Empty_2();
    }

};
