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
//	File:	Instruction_RQE.java
//

/*
RQE {RIxx, IIxx, LIxx, BIxx} 
(Read Quality Including Error) 

RQE reads the Bad, Out-of-Service, and Error status bits and the OM status field of the specified 
input. It writes the value 1 to the accumulator if any of the three bits is set or if the OM status 
field does not have the value "ON_SCAN". In any other case, it writes the value 0 to the accumulator. 
sptr(after) = sptr(before) + 1.

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_RQE: public Instruction_Interface
{
    Instruction_RQE() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        try {
            bool bad = reg.Index_To_Memory(s->Get_Register1())->Get_Status(Mem_Status::BAD);
            bool oos = reg.Index_To_Memory(s->Get_Register1())->Get_Status(Mem_Status::OOS);
            bool err = reg.Index_To_Memory(s->Get_Register1())->Get_Status(Mem_Status::ERROR_);
            
            stack.Push(bad || oos || err ? 1.0F : 0.0F);
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
        return 	(s.RIxx_1() || s.IIxx_1() || s.LIxx_1() || s.BIxx_1()) && s.Empty_2();
    }

};
