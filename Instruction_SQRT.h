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
//	File:	Instruction_SQRT.java
//

/*
SQRT 
(Square Root)

SQRT reads the value (which must be  0) in the accumulator, computes the square root of the value, 
and writes the positive root to the accumulator, writing over the original contents-> 
sptr(after) = sptr(before).

An attempt to SQRT a value < 0 causes the instruction to be skipped and writes a "1" (SQRT run-time error) 
to the PERROR parameter.

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"
#include	<cmath>

struct Instruction_SQRT: public Instruction_Interface
{
    Instruction_SQRT() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        try {
            float acc = stack.Top(0);
            
            if (acc < 0.0F) {
                throw Exception_SQRT_Error();	// error code
            } else {
                acc = (float) std::sqrt(acc);
                stack.Replace(acc);
            }
        }
        catch (Exception_SQRT_Error &e) {
	    steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(1);	// error code
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
