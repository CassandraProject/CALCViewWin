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
//	File:	Instruction_MAX.java
//

/*
MAX
MAX c
MAX {RIxx, ROxx, Mxx} 
MAX {RIxx, ROxx, Mxx} {n, RIxx, ROxx, Mxx} 
(Maximum)

MAX reads all the values from the stack, selects the maximum algebraic value, decrements the stack 
pointer, and writes this value into the new accumulator location (the new top of stack location). 
This overwrites the first operand and isolates other operands from any further access-> 
sptr(after) = stackbottom.

MAX c reads the top c values from the stack, selects the maximum algebraic value from the c topmost 
values on the stack, writes this value into the c'th position from the top of the stack, and sets the 
new top of the stack (the accumulator) to this position. sptr(after) = sptr(before) - c + 1.

If c exceeds the current number of stack operands, a "6" (stack underflow run-time error) is written 
to the PERROR parameter.

MAX RIxx reads the value stored in RIxx (the CALCA's Real Input parameter xx), selects the maximum 
algebraic value from the RIxx value and the value that it pops from the stack, and then pushes the 
result onto the stack. MAX ROxx and MAX Mxx do the same for the values stored at ROxx and memory 
location xx, respectively. sptr(after) = sptr(before).

MAX RIxx Mxx reads the values stored in RIxx and Mxx, selects the maximum algebraic value from the 
RIxx and Mxx values, and stores the result on the stack. Whenever both operands are specified, the 
maximum algebraic value is selected from the first and second operands-> 
sptr(after) = sptr(before) + 1.

MAX RIxx n reads the value stored in RIxx and a specified integer constant (n), selects the maximum 
algebraic value from RIxx and n, and stores the result on the stack. If the first operand is ROxx or 
Mxx, the operation is similar. sptr(after) = sptr(before) + 1.

CHECKED: March 28, 2003
*/
#pragma once

#include    "Instruction_Interface.h"
#include	<cmath>

struct Instruction_MAX: public Instruction_Interface
{
    Instruction_MAX() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        try {
            if (s->Empty_1()) {
                int c = stack.Depth();
                float max = stack.Top(0);
                
                for (; c > 0; --c) {
                    max = std::max(stack.Top(0), max);
                    if (c > 1) stack.Pop(1); // Don't pop last element
               }
                
                stack.Replace(max);
                
            } else if (s->Number_1()) {
                int c = s->Get_Value1();
                float max = stack.Top(0);
     
                for (; c > 0; --c) {
                    max = std::max(stack.Top(0), max);
                    stack.Pop(1);
                }
                
                stack.Push(max);
                
            } else if (s->Register_1() && s->Empty_2()) {
                float acc = stack.Top(0);
                float r = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
                            
                if (stack.Depth() > 0)
                    stack.Replace(std::max(acc, r));
                    
            } else if (s->Register_1() && s->Number_2()) {
                float r = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
                float n = (float) s->Get_Value2();
                            
                stack.Push(std::max(n, r));
        
            } else if (s->Register_1() && s->Register_2()) {
                float reg1 = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
                float reg2 = reg.Index_To_Memory(s->Get_Register2())->Get_Value();
                
                stack.Push(std::max(reg1, reg2));
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
        return 	s.Empty_1() && s.Empty_2() ||									// MAX
                s.Number_1() && s.Empty_2() ||									// MAX c
                (s.RIxx_1() || s.ROxx_1() || s.Mxx_1()) && s.Empty_2() || 					// MAX {RIxx, ROxx, Mxx}
                (s.RIxx_1() || s.ROxx_1() || s.Mxx_1()) && (s.Number_2() || s.RIxx_2() || s.ROxx_2() || s.Mxx_2()); // MAX {RIxx, ROxx, Mxx} {n,RIxx, ROxx, Mxx}
    }

};
