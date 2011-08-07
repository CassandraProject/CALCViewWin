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
//	File:	Instruction_IDIV.java
//

/*
IDIV
IDIV Mxx 
(Integer Division)

IDIV, with either blank or Mxx operand, reads the values from the top two stack locations, truncates 
each into integer form, integer divides the second truncated operand (divisor) into the first 
truncated operand (dividend), decrements the stack pointer, and writes the quotient into the new 
accumulator location (the new top of stack location). This overwrites the first operand and isolates 
the second operand from any further access->

The result of integer division is always truncated, so that the quotient returned to the stack is 
always a whole number. When an operand Mxx is specified, the quotient is handled as above, and the 
integer remainder (the integer modulus) is stored in memory location xx. The integer modulus is also 
always a whole number. In all cases, sptr(after) = sptr(before) - 1.

If the truncated value of the divisor is 0, the value 0.0 is written into the stack location where 
the quotient would have been placed. The value "4" (divide run-time error) is written to the PERROR 
parameter. The value of Mxx (if specified as an operand) is unchanged.

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_IDIV: public Instruction_Interface
{
    Instruction_IDIV() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        try {
            if (s->Empty_1()) {
                int acc1 = (int) stack.Top(0);
                int acc2 = (int) stack.Top(1);

                if (acc1 == 0.0F) {
                    stack.Pop(1);
                    stack.Replace(0.0F);
                    throw Exception_DIV_Error();	// error code
                } else {
                    // Kludge to implement weird behaviour when only 1 element on the stack
                    stack.Pop(1);
                    if (stack.Depth() > 0) 
                        stack.Replace((float) (acc2/acc1));
                }
                
            } else if (s->Register_1() && s->Empty_2()) {
                int acc1 = (int) stack.Top(0);
                int acc2 = (int) stack.Top(1);
                
                if (acc1 == 0.0F) {
                    stack.Pop(1);
                    stack.Replace(0.0F);
                    throw Exception_DIV_Error();	// error code
                } else {
                    // Kludge to implement weird behaviour when only 1 element on the stack
                    stack.Pop(1);
                    if (stack.Depth() > 0) 
                        stack.Replace((float) (acc2/acc1));
                    
                    // Store modulus
                    reg.Index_To_Memory(s->Get_Register1())->Set_Value((float) (acc2 % acc1));
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
        catch (Exception_DIV_Error &e) {
	    steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(4);	// error code
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
        return 	s.Empty_1() && s.Empty_2() ||	// IDIV
                s.Mxx_1() && s.Empty_2();	// IDIV Mxx
      }

};
