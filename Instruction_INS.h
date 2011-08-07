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
//	File:	Instruction_INS.java
//

/*
INS {RIxx, BIxx, IIxx, LIxx, ROxx, BOxx, IOxx, LOxx} 
(Input Status)

INS pushes the 16-bit status of the specified input or output parameter onto the stack. The status 
value is not modified. sptr(after) = sptr(before) + 1.

The status integer is made up of the following fields: (Bit 0 is the least significant bit; Bit 15 
is most significant.)

 Bit  Definition  
0 to 4 Data Type 
5 to 7 OM Status 
8 Bad 
9 Secure/Release 
10 Acknowledge 
11 Out of Service 
12 Shadow Parameter 
13 Limited High 
14 Limited Low 
15 Propagated Error 

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_INS: public Instruction_Interface
{
    Instruction_INS() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();
        
        try {
            float r = (float) reg.Index_To_Memory(s->Get_Register1())->Get_Status();
            stack.Push(r);
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
        return 	(s.RIxx_1() || s.BIxx_1() || s.IIxx_1() || s.LIxx_1() || 
                s.ROxx_1() || s.BOxx_1() || s.IOxx_1() || s.LOxx_1()) && s.Empty_2();
    }

};
