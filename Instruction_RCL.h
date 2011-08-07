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
//	File:	Instruction_RCL.java
//

/*
RCL {RIxx, ROxx, IIxx, IOxx, BIxx, BOxx, Ixx, Oxx, Mxx, ~RIxx, ~ROxx, ~IIxx, ~IOxx,
~BIxx, ~BOxx, ~Ixx, ~Oxx, ~Mxx} 
(Read and Clear)

RCL with RIxx, IIxx, BIxx, or Ixx operand unconditionally pushes onto the stack the contents of the 
operand. If the operand is unlinked (in the case of Ixx, if the LI01 operand is unlinked) a zero 
value is written to the operand. Otherwise the operand is unchanged.

RCL with ROxx, IOxx, BOxx, or Oxx operand unconditionally pushes onto the stack the operand contents, 
and, if the CALCA block is in Auto, writes a zero value to the operand. Otherwise the operand is 
unchanged.

RCL Mxx always pushes the Mxx value onto the stack and writes 0.0 to Mxx. 

For real type operands, a written zero value is 0.0; for integer types it is 0; and for Boolean 
types it is False. Values of integer operands are converted to real type when pushed on the stack. 
False Boolean operands are pushed as 0.0, and true Boolean operands are pushed as 1.0.

RCL with inverted operand follows the same rules as above, except that operand values are inverted 
before they are pushed on the stack. Inverted real or integer operands are pushed as 0.0 when they 
are non-zero and as 1.0 when they are zero.

In all cases, sptr(after) = sptr(before) + 1.

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_RCL: public Instruction_Interface
{
    Instruction_RCL() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();
        
        try {
            if (s->Register_1()) {
                float r = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
                reg.Index_To_Memory(s->Get_Register1())->Set_Value(0.0F);
                
                if (s->Invert_1())	r = (r != 0.0F) ? 0.0F : 1.0F; // Invert the param
    
                stack.Push(r);
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
        return (s.RIxx_1() || s.ROxx_1() || s.IIxx_1() || s.IOxx_1() ||
                s.BIxx_1() || s.BOxx_1() || s.IIxx_1() || s.LIxx_1() ||
                s.IOxx_1() || s.LOxx_1() || s.Mxx_1() || s.nRIxx_1() ||
                s.nROxx_1() || s.nIIxx_1() || s.nIOxx_1() || s.nBIxx_1() ||
                s.nBOxx_1() || s.nIIxx_1() || s.nIOxx_1() || s.nLIxx_1() || 
                s.nLOxx_1() ) && s.Empty_2();
    }

};
