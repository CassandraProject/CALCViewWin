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
//	File:	Instruction_GTI.java
//

/*
GTI
GTI {RIxx, ROxx, IIxx, IOxx, Mxx} 
(Go To Indirect)

GTI branches to the step number contained in the accumulator. GTI RIxx branches to the step number 
specified by the current contents of RIxx. Similarly for the other operand types-> When the destination 
is in an operand of real data type (accumulator, RIxx, ROxx, or Mxx), the operand contents are 
truncated to an integer before use. sptr(after) = sptr(before).

An attempt to branch to a step number less than or equal to the current step number or greater than 
the step number containing the END statement (50 if there is no END step) writes a "10" (index 
run-time error) to the PERROR parameter. 

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_GTI: public Instruction_Interface
{
    Instruction_GTI() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();
        
        float acc = stack.Top(0);
        
        try {
            if (s->Empty_1()) {
                steps.Jump_Step((int) acc);
            } else {
                steps.Jump_Step((int) reg.Index_To_Memory(s->Get_Register1())->Get_Value());
            }
        }
        catch (Exception_Illegal_Branch &e) {
            steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(10);	// error code
            throw e;
        }
        
    }

    void Update_Register_Use (Mem_Step &s, Registers_Table_Model &reg) {
        reg.Use_Register (PERROR);
        if (s.Register_1())	reg.Use_Register (s.Get_Register1());      
        if (s.Register_2())	reg.Use_Register (s.Get_Register2());      
    }

    bool Check (Mem_Step &s, Machine &mem) {
        return 	s.Empty_1() && s.Empty_2() ||
                (s.RIxx_1() || s.ROxx_1() || s.IIxx_1() || s.IOxx_1() || s.Mxx_1()) && s.Empty_2();
    }

};
