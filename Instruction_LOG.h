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
//	File:	Instruction_LOG.java
//

/*
LOG 
(Common Logarithm)

LOG reads the value (which must be positive) in the accumulator, computes the common (base 10) 
logarithm of the value, and writes the result to the accumulator, overwriting the original contents-> 
sptr(after) = sptr(before).

An attempt to execute this instruction with a zero or negative value in the accumulator causes the 
instruction to be skipped and writes a "7" (LOG run-time error) to the PERROR parameter.

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_LOG: public Instruction_Interface
{
    Instruction_LOG() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();
 
        try {
            float acc = stack.Top(0);
            
            if (acc <= 0.0F) {
                throw Exception_LOG_Error();	// error code
            } else {                
                // Since log10(x) = loge(x) / loge(10)
                acc = (float) (std::log(acc) / std::log(10.0));
                
                stack.Replace(acc);
            }
        }
        catch (Exception_LOG_Error &e) {
	    steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(7);	// error code
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
