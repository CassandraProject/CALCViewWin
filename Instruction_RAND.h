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
//	File:	Instruction_RAND.java
//

/*
RAND 
(Generate Random Number)

RAND generates a random number with uniform distribution and places it in the accumulator. The 
following formula is used:
Seed = (Seed*125) modulo 2,796,203
Accumulator = Seed / 2,796,203

Seed is a CALCA block variable which is set to the value 100,001 when the block initializes, and 
is changed each time a RAND instruction is executed. You can re-initialize Seed with any desired 
value at any time, by using the SEED instruction. sptr(after) = sptr(before) + 1.

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_RAND: public Instruction_Interface
{
    Instruction_RAND() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();
        
        try {
            int randseed = (int) reg.Index_To_Memory(RNDSEED)->Get_Value();
            
            randseed = (randseed * 125) % 2796203;
            float acc = ((float) randseed / 2796203.0F);
            
            reg.Index_To_Memory(RNDSEED)->Set_Value((float) randseed);
            stack.Push(acc);
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
        reg.Use_Register (RNDSEED);
    }

    bool Check (Mem_Step &s, Machine &mem) {
        return 	s.Empty_1() && s.Empty_2();
    }

};
