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
//	File:	Instruction_OUT.java
//

/*
OUT {ROxx, BOxx, IOxx, Oxx, Mxx, ~BOxx, ~Oxx, ~Mxx} 
(Output)

OUT writes the accumulator contents to the output parameter specified by the operand. When operating 
in the Manual state, this instruction is skipped without changing the value of the output, unless 
the operand is Mxx or ~Mxx, in which case the operation is executed.

The value of the accumulator is first clamped between 32,767 and -32,768 and then truncated to an 
integer value before writing it to an operand of type IOxx.

Any non-zero value in the accumulator is converted to True in an operand of type BOxx or Oxx, and 
a 0.0 value is converted to False. If the operand is of type ~BOxx or ~Oxx a non-zero value in the 
accumulator causes a False to be written and a 0.0 causes a True to be written.

If the operand is of type Mxx, the accumulator value is simply copied to the specified memory 
location. If it is of type ~Mxx, any non-zero accumulator value is written to Mxx as the real 
value 0.0 and an accumulator value of 0.0 is written to Mxx as real value 1.0.

OUT does not alter the contents or location of the accumulator. sptr(after) = sptr(before).

CHECKED: March 28, 2003
*/
#pragma once

#include    "Instruction_Interface.h"

struct Instruction_OUT: public Instruction_Interface
{
    Instruction_OUT() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

    
        float acc = stack.Top(0);

        if (s->Invert_1())	acc = (acc != 0.0F) ? 0.0F : 1.0F; // Invert the param

        reg.Index_To_Memory(s->Get_Register1())->Set_Value(acc);
        
		steps.Increment_Step();
	}

    void Update_Register_Use (Mem_Step &s, Registers_Table_Model &reg) {
        reg.Use_Register (PERROR);
        if (s.Register_1())	reg.Use_Register (s.Get_Register1());      
        if (s.Register_2())	reg.Use_Register (s.Get_Register2());      
    }

    bool Check (Mem_Step &s, Machine &mem) {
        return 	(s.ROxx_1() || s.BOxx_1() || s.IOxx_1() || s.Mxx_1() || 
                s.nBOxx_1() || s.nIOxx_1() || s.nLOxx_1() || s.nMxx_1() || 
                s.Oxx_1() || s.nOxx_1()) && s.Empty_2();
    }

};
