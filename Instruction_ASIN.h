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
//	File:	Instruction_ASIN.java
//

/*
ASIN 
(Arc Sine)

ASIN reads the value (sine function value) in the accumulator and computes the value, in radians, 
of the first or fourth quadrant angle that has this sine value. It then stores the angle value into 
the accumulator, overwriting the original sine value, which must have an absolute value equal to or 
less than one. sptr(after) = sptr(before).

An attempt to ASIN a value > 1 or < -1 causes the instruction to be skipped and writes a "2" 
(ASIN run-time error) to the PERROR parameter.

CHECKED: March 14, 2003
*/
#include    "Instruction_Interface.h"
#include	<cmath>

struct Instruction_ASIN: public Instruction_Interface
{
    Instruction_ASIN() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();
 
        try {

            float acc = stack.Top(0);
            
            if (acc > 1.0F || acc < -1.0F) {
                throw Exception_ASIN_Error();
            } else {
                acc = (float) std::asin(acc);
                stack.Replace(acc);
            }
        }
        catch (Exception_ASIN_Error &e) {
			steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(2);	// error code
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
