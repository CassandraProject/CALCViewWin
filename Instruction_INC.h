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
//	File:	Instruction_INC.java
//

/*
INC
INC n
INC {ROxx, IOxx, LOxx, Mxx} 
(Increment)

INC increments the accumulator or the specified operand.

If blank is specified, the value in the accumulator is read, algebraically increased by 1.0, 
and returned to the accumulator, overwriting the original contents-> If an integer n is specified, 
the accumulator value is algebraically increased by n and returned to the accumulator. 

If Mxx is specified, the value in memory location xx is algebraically increased by 1.0. The 
stack is unaffected. 

If ROxx is specified and the block is in Auto, the indicated output is algebraically increased 
by 1.0. If the block is in Manual, the step is skipped.

If IOxx or LOxx is specified and the block is in Auto, the indicated output is incremented by 1 
as an integer addition without overflow. If the operand value before incrementation is equal 
to 32,767 (2,147,483,647 in the case of LOxx) or the block is in Manual, the step is skipped.

The stack is unaffected when the operand is other than blank or n. In all cases, 
sptr(after) = sptr(before).

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"
#include	<climits>

struct Instruction_INC: public Instruction_Interface
{
    Instruction_INC() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        if (s->Empty_1()) {
            float acc = stack.Top(0);
                        
            stack.Replace(acc + 1.0F);
            
        } else if (s->Number_1()) {
            float acc = stack.Top(0);
            int c = s->Get_Value1();
                            
            stack.Replace(acc + (float) c);

        } else if (s->IOxx_1() && s->Empty_2()) {
            float r = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
            
            if (r >= SHRT_MAX)			r = SHRT_MAX;
            else						r = r + 1.0F;
            
            reg.Index_To_Memory(s->Get_Register1())->Set_Value(r);
                
        } else if (s->LOxx_1() && s->Empty_2()) {
            float r = reg.Index_To_Memory(s->Get_Register1())->Get_Value();

            if (r >= LONG_MAX)	r = (float) LONG_MAX;
            else				r = r + 1.0F;

            reg.Index_To_Memory(s->Get_Register1())->Set_Value(r);
                
        } else if (s->Register_1() && s->Empty_2()) {
            float r = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
            reg.Index_To_Memory(s->Get_Register1())->Set_Value(r + 1.0F);
                
        }
        
		steps.Increment_Step();
	}
    
    void Update_Register_Use (Mem_Step &s, Registers_Table_Model &reg) {
        reg.Use_Register (PERROR);
        if (s.Register_1())	reg.Use_Register (s.Get_Register1());      
        if (s.Register_2())	reg.Use_Register (s.Get_Register2());      
    }

    bool Check (Mem_Step &s, Machine &mem) {
        return 	s.Empty_1() && s.Empty_2() ||						// INC
                s.Number_1() && s.Empty_2() ||						// INC c
                (s.ROxx_1() || s.IOxx_1() || s.LOxx_1() || s.Mxx_1()) && s.Empty_2(); 	// INC {RIxx, ROxx, Mxx}
    }

};
