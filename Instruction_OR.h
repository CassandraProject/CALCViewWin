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
//	File:	Instruction_OR.java
//

/*
OR
OR c
OR {BIxx, BOxx, Ixx, Oxx, Mxx, ~BIxx, ~BOxx, ~Ixx, ~Oxx, ~Mxx} 
OR {BIxx, BOxx, Ixx, Oxx, Mxx, ~BIxx, ~BOxx, ~Ixx, ~Oxx, ~Mxx} {BIxx, BOxx,
Ixx, Oxx, Mxx, ~BIxx, ~BOxx, ~Ixx, ~Oxx, ~Mxx} 
(Logical Or)

OR performs a logical "or" of the contents of the specified operands and stack locations->

If blank is specified, all stack locations are ORed together, and the result is written into the 
accumulator (the new top of stack location). This overwrites the first operand and isolates the 
other operands from any further access-> sptr(after) = stackbottom.

If c is specified, the last c stack locations are ORed together, removed from the stack, and the 
result stored in the accumulator. sptr(after) = sptr(before) - c + 1.

If c exceeds the current number of stack operands, a "6" (stack underflow run-time error) is written 
to the PERROR parameter.

If only one operand is specified, the operand is ORed with the accumulator, and the result replaces 
the accumulator. sptr(after) = sptr(before).

If both operands are specified, the operands are ORed together and the result is stored in the 
accumulator. sptr(after) = sptr(before) + 1.

If ~ is specified, the operand value is inverted prior to being used.

Before the operand Mxx is used in the calculation, its value is truncated to an integer; if the 
result is 0 the operand value is considered False, otherwise it is True.

CHECKED: March 14, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_OR: public Instruction_Interface
{
    Instruction_OR() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        try {
            if (s->Empty_1()) {
                bool result = false;
                int depth = stack.Depth();
                
                for (; depth > 0; --depth) {
                    result = result || (int) stack.Top(0) != 0;
                    if (depth > 1) stack.Pop(1); // Don't pop last element
                }
                
                stack.Replace(result ? 1.0F : 0.0F);
                
            } else if (s->Number_1()) {
                bool result = false;
                int c = s->Get_Value1();
                
                for (; c > 0; --c) {
                    result = result || (int) stack.Top(0) != 0;
                    stack.Pop(1); // Don't pop last element
                }
                
                stack.Push(result ? 1.0F : 0.0F);
            } else if (s->Register_1() && s->Empty_2()) {
                bool acc = (int) stack.Top(0) != 0;
                bool r = (int) reg.Index_To_Memory(s->Get_Register1())->Get_Value() != 0;
                
                if (s->Invert_1())	r = !r; // Invert the param
                
                stack.Replace((acc || r) ? 1.0F : 0.0F);
                    
            } else if (s->Register_1() && s->Register_2()) {
                bool r1 = (int) reg.Index_To_Memory(s->Get_Register1())->Get_Value() != 0;
                bool r2 = (int) reg.Index_To_Memory(s->Get_Register2())->Get_Value() != 0;
                
                if (s->Invert_1())	r1 = !r1; // Invert the param
                if (s->Invert_2())	r2 = !r2; // Invert the param
    
                stack.Push((r1 || r2) ? 1.0F : 0.0F);
        
            }
        }
        catch (Exception_Stack_Overflow &e) {
	    steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(5);	// error code
            throw e;
        }   
        catch (Exception_Stack_Underflow &e) {
	    steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(6);	// error code
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
        return 	s.Empty_1() && s.Empty_2() ||									// AND
                s.Number_1() && s.Empty_2() && s.Get_Value1() >= 1 && s.Get_Value1() <= mem.Get_Num_Memory() ||									// ADD c

                (s.BIxx_1() || s.BOxx_1() || s.IIxx_1() || s.IOxx_1() || s.LIxx_1() || s.LOxx_1() || s.Mxx_1() ||
                s.nBIxx_1() || s.nBOxx_1() || s.nIIxx_1() || s.nIOxx_1() || s.nLIxx_1() || s.nLOxx_1() || s.nMxx_1()) && s.Empty_2() ||	// AND {BIxx, BOxx, Ixx, Oxx, Mxx, ~BIxx, ~BOxx, ~Ixx, ~Oxx, ~Mxx}

                (s.BIxx_1() || s.BOxx_1() || s.IIxx_1() || s.IOxx_1() || s.LIxx_1() || s.LOxx_1() || s.Mxx_1() ||
                s.nBIxx_1() || s.nBOxx_1() || s.nIIxx_1() || s.nIOxx_1() || s.nLIxx_1() || s.nLOxx_1() || s.nMxx_1()) && 	
                (s.BIxx_2() || s.BOxx_2() || s.IIxx_2() || s.IOxx_2() || s.LIxx_2() || s.LOxx_2() || s.Mxx_2() ||
                s.nBIxx_2() || s.nBOxx_2() || s.nIIxx_2() || s.nIOxx_2() || s.nLIxx_2() || s.nLOxx_2() || s.nMxx_2());	// AND {BIxx, BOxx, Ixx, Oxx, Mxx, ~BIxx, ~BOxx, ~Ixx, ~Oxx, ~Mxx} {BIxx, BOxx, Ixx, Oxx, Mxx, ~BIxx, ~BOxx, ~Ixx, ~Oxx, ~Mxx}
    }

};
