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
//	File:	Instruction_RON.java
//

/*
RON {RIxx, IIxx, LIxx, BIxx, ROxx, IOxx, LOxx, BOxx} 
(Read In-Service Status)

RON reads the Out-of-Service status bit of the specified parameter and writes the inverse of its 
value to the accumulator. (1 = True = OOS bit not set = parameter in service, 0 = False = OOS bit 
set = parameter out-of-service).

The result of the RON instruction is always the inverse of the result of the ROO instruction. 
sptr(after) = sptr(before) + 1.

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_RON: public Instruction_Interface
{
    Instruction_RON() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();
        
        try {
            bool error = reg.Index_To_Memory(s->Get_Register1())->Get_Status(Mem_Status::OOS);
            stack.Push(error ? 0.0F : 1.0F);
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
        return 	(s.RIxx_1() || s.IIxx_1() || s.LIxx_1() || s.BIxx_1() || 
                s.ROxx_1() || s.IOxx_1() || s.LOxx_1() || s.BOxx_1()) && s.Empty_2();
    }

};
