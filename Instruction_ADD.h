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
//	File:	Instruction_ADD.java
//

/*
ADD
ADD c
ADD {RIxx, ROxx, Mxx}
ADD {RIxx, ROxx, Mxx} {n, RIxx, ROxx, Mxx} 
(Add)

ADD (or ADD c) reads the values from the top two stack locations (or a specified number c of stack
values), adds them, decrements the stack pointer, and writes the sum into the new accumulator location 
(the new top of stack location). This overwrites the first operand and isolates the second and any 
later operands from any further access-> For ADD, sptr(after) = sptr(before) - 1. For ADD c, 
sptr(after) = sptr(before) - c + 1.

If c exceeds the current number of stack operands, a "6" (stack underflow run-time error) is written 
to the PERROR parameter. The same action occurs if ADD has no operand and there is only one value on 
the stack.

ADD RIxx reads the value stored in RIxx (the CALCA's Real Input parameter xx), adds it to the value 
that it pops from the stack, then pushes the result back onto the stack. ADD ROxx and ADD Mxx do the 
same for the values stored at ROxx and memory location xx, respectively. sptr(after) = sptr(before).

ADD RIxx n reads the RIxx value and a specified integer constant (n), adds them and stores the result 
on the stack. If the first operand is ROxx or Mxx, the operation is similar. 
sptr(after) = sptr(before) + 1.

The range of n is -64 to 63.

NOTE Values outside of this range will be accepted but the results are based on the rightmost 6 bits of n. 

ADD RIxx Mxx reads the values stored in RIxx and Mxx, adds them, and stores the result on the stack. 
Similarly when ROxx or Mxx is the first operand or RIxx or ROxx is the second operand. 
sptr(after) = sptr(before) + 1.

CHECKED: March 14, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_ADD: public Instruction_Interface
{
    Instruction_ADD() {
       
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        try {
            if (s->Empty_1()) {
                float acc1 = stack.Top(0);
                float acc2 = stack.Top(1);
                
                stack.Pop(1);
                
                stack.Replace(acc1+acc2);
                
            } else if (s->Number_1()) {
                float sum = 0.0F;
                int c = s->Get_Value1();
                
                for (; c > 0; --c) {
                    sum += stack.Top(0);
                    stack.Pop(1); // Don't pop last element
                }
                
                stack.Push(sum);
            } else if (s->Register_1() && s->Empty_2()) {
                float acc = stack.Top(0);
                float r = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
                            
                stack.Replace(acc+r);
                    
            } else if (s->Register_1() && s->Number_2()) {
                float r = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
                float n = (float) s->Get_Value2();
                        
                stack.Push(r+n);
        
            } else if (s->Register_1() && s->Register_2()) {
                float reg1 = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
                float reg2 = reg.Index_To_Memory(s->Get_Register2())->Get_Value();
                
                stack.Push(reg1+reg2);
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
        return 	s.Empty_1() && s.Empty_2() ||									// ADD
                s.Number_1() && s.Empty_2() && s.Get_Value1() >= 1 && s.Get_Value1() <= mem.Get_Num_Memory() ||			// ADD c
                (s.RIxx_1() || s.ROxx_1() || s.Mxx_1()) && s.Empty_2() || 					// ADD {RIxx, ROxx, Mxx}
                (s.RIxx_1() || s.ROxx_1() || s.Mxx_1()) && (s.Number_2() && s.Get_Value1() >= -64 && s.Get_Value1() <= 63) ||  // ADD {RIxx, ROxx, Mxx} {n}
                (s.RIxx_1() || s.ROxx_1() || s.Mxx_1()) && (s.RIxx_2() || s.ROxx_2() || s.Mxx_2()); 		// ADD {RIxx, ROxx, Mxx} {n,RIxx, ROxx, Mxx}
    }

};
