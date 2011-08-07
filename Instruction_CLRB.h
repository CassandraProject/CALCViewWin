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
//	File:	Instruction_CLRB.java
//

/*
CLRB
CLRB b 
(Clear Packed Boolean)

CLRB clears to 0 a specified bit in the accumulator. If blank is specified the bit number is 
expected to be at the top of the stack and this bit number is removed from the stack before 
the instruction is executed. sptr(after) = sptr(before) - 1.

If the bit number is less than 1 or greater than 16 an "11" (bit error run-time error) is written 
to the PERROR parameter.

If the operand b is specified it indicates a bit number between 1 and 16 in the accumulator. The 
stack is not popped in this case. sptr(after) = sptr(before).

Bit 1 is the most significant bit of the accumulator, and bit 16 the least significant.

CHECKED: March 14, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_CLRB: public Instruction_Interface
{
    Instruction_CLRB() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();
        
        try {
            if (s->Empty_1()) {
                int bit = (short) stack.Top(0);
                
                if (bit < 1 || bit > 16) {
                    throw Exception_Bit_Error();
                } else {
                    int acc = (int) stack.Top(1);
                    stack.Pop(1);
                    acc = acc & (~(1 << (16-bit)));
                    stack.Replace((float) acc);
                }
                
            } else if (s->Number_1()) {
                int bit = (short) s->Get_Value1();
                
                if (bit < 1 || bit > 16) {
                    throw Exception_Bit_Error();
                } else {
                    int acc = (int) stack.Top(0);
                    acc = acc & (~(1 << (16-bit)));
                    stack.Replace((float) acc);
                }
    
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
        catch (Exception_Bit_Error &e) {
	    steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(11);	// error code
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
        return 	s.Empty_1() && s.Empty_2() || 
        	s.Number_1() && s.Empty_2() && s.Get_Value1() >= 1 && s.Get_Value1() <= 16;									// ADD c
    }

};
