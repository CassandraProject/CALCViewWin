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
//	File:	Instruction_MRS.java
//

/*
MRS 
(Master Reset Flip-Flop)

MRS emulates the function of a traditional reset-dominant flip-flop. It uses two operands on the 
stack as inputs-> The first operand is the "set" input and the second operand is the "reset" input.

This instruction writes the output, which corresponds to the "Q" output of a flip-flop, into the 
accumulator, overwriting the first operand (the "set" value) and making the second operand (the 
"reset" value) inaccessible.

Note that the MRS instruction provides the same functionality as the FF, except that the reset 
input has priority over the set input. If both inputs take on a logical one value, the flip-flop 
is forced to reset.



Truth Table for MRS Instruction 
S R Q  
0 0 No Change            
0 1 0 
1 0 1 
1 1 0 
 


The "No Change" condition causes the value of the MRS flip-flop after the previous execution 
cycle, which is retained in a block data variable, to be written to the accumulator. 
sptr(after) = sptr(before) - 1.

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"


struct Instruction_MRS: public Instruction_Interface
{
    Instruction_MRS() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        try {
            bool reset = stack.Top(0) != 0.0F;
            bool set = stack.Top(1) != 0.0F;
                            
            bool ff = s->Get_Timer() != 0.0F;
            
            if (set == true && reset == false)
                ff = true;
            else if (set == false && reset == true)
                ff = false;
            else if (set == true && reset == true)
                ff = false;
            
            s->Set_Timer(ff ? 1.0F : 0.0F);
            
            stack.Pop(1); 
            stack.Replace(s->Get_Timer()); 
        }
        catch (Exception_Stack_Overflow &e) {
	    steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(5);	// error code
            throw e;
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
