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
//	File:	Instruction_IN.java
//

/*
IN
IN {n, RIxx, ROxx, IIxx, IOxx, BIxx, BOxx, Ixx, Oxx, Mxx, ~RIxx, ~ROxx, ~IIxx, 
~IOxx, ~BIxx, ~BOxx, ~Ixx, ~Oxx, ~Mxx} 
(Input)

IN with no operands pushes the value 0.0 onto the stack. IN with operand of type n (any integer), 
RIxx, ROxx, or Mxx pushes the operand value onto the stack. IN IIxx and IN IOxx convert the operand 
value to real data type and then push the result onto the stack. IN with one of the Boolean operands 
(BIxx, BOxx, Ixx, or Oxx) pushes the value 1.0 when the operand value is True and 0.0 when it is False.

IN with inverted Boolean operand (~BIxx, ~BOxx, ~Ixx, or ~Oxx) pushes the value 0.0 onto the stack 
when the parameter value is True and 1.0 when it is false. IN with inverted real or integer operand 
(~RIxx, ~ROxx, ~IIxx, ~IOxx, or ~Mxx) pushes the value 0.0 when the parameter is non-zero, and 1.0 
when the parameter value is 0.0. In all cases, sptr(after) = sptr(before) + 1.

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_IN: public Instruction_Interface
{
    Instruction_IN() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();
        
        try {
            if (s->Empty_1()) {
                stack.Push(0.0F);
            } else if (s->Register_1()) {
                float r = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
                
                if (s->Invert_1())	r = (r != 0.0F) ? 0.0F : 1.0F; // Invert the param
    
                stack.Push(r);
            } else if (s->Number_1()) {
                float c = (float) s->Get_Value1();

                stack.Push((float) c);
            }
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
        return 	true; // Input anything
    }

};
