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
//	File:	Instruction_DIV.java
//

/*
DIV
DIV {RIxx, ROxx, Mxx}
DIV {RIxx, ROxx, Mxx} {n, RIxx, ROxx, Mxx} 
(Divide)

DIV reads the values from the top two stack locations, divides the second operand (divisor) 
into the first operand (dividend), decrements the stack pointer, and writes the quotient into 
the new accumulator location (the new top of stack location). This overwrites the first operand 
and isolates the second operand from any further access-> sptr(after) = sptr(before) - 1.

DIV RIxx reads the value stored in RIxx (the CALCA's Real Input parameter xx) as the divisor, 
divides the value (the dividend) that it pops from the stack, then pushes the result back onto 
the stack. DIV ROxx and DIV Mxx do the same for the values stored at ROxx and memory location xx, 
respectively. sptr(after) = sptr(before).

DIV RIxx Mxx reads the values stored in RIxx and Mxx, divides the RIxx value by the Mxx value, 
and stores the result on the stack. Whenever both operands are specified, the first operand is 
divided by the second operand. sptr(after) = sptr(before) + 1.

DIV RIxx n reads the value stored in RIxx and a specified integer constant n, divides the RIxx 
value by n, and stores the result on the stack. If the first operand is ROxx or Mxx, the operation 
is similar. sptr(after) = sptr(before) + 1.

The range of n is -64 to 63.

NOTE
Values outside of this range will be accepted but the results are based on the rightmost 6 bits of n. 

An attempt to DIV by zero (divisor = 0) causes the value of 0 to be written to the stack location at which the result would ordinarily be stored. A "4" (DIV run-time error) is written to the PERROR parameter.

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_DIV: public Instruction_Interface
{
    Instruction_DIV() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        try {
            if (s->Empty_1()) {
                float acc1 = stack.Top(0);
                float acc2 = stack.Top(1);
                
                if (acc1 == 0.0F) {
                    stack.Pop(1);
                    stack.Replace(0.0F);
                    throw Exception_DIV_Error();	// error code
                } else {
                    stack.Pop(1);
                    stack.Replace(acc2/acc1);
                }
                
            } else if (s->Register_1() && s->Empty_2()) {
                float acc = stack.Top(0);
                float r = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
    
                if (r == 0.0F) {
                    stack.Replace(0.0F);
                    throw Exception_DIV_Error();	// error code
                } else {
                    stack.Replace(acc/r);
                }
                    
            } else if (s->Register_1() && s->Number_2()) {
                float r = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
                float n = (float) s->Get_Value2();
                        
                if (n == 0.0F) {
                    stack.Push(0.0F);
                    throw Exception_DIV_Error();	// error code
                } else {
                    stack.Push(r/n);
                }
    
            } else if (s->Register_1() && s->Register_2()) {
                float reg1 = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
                float reg2 = reg.Index_To_Memory(s->Get_Register2())->Get_Value();
            
                if (reg2 == 0.0F) {
                    stack.Push(0.0F);
                    throw Exception_DIV_Error();	// error code
                } else {
                    stack.Push(reg1/reg2);
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
        return 	s.Empty_1() && s.Empty_2() ||									// DIV
                (s.RIxx_1() || s.ROxx_1() || s.Mxx_1()) && s.Empty_2() || 					// DIV {RIxx, ROxx, Mxx}
                (s.RIxx_1() || s.ROxx_1() || s.Mxx_1()) && (s.RIxx_2() || s.ROxx_2() || s.Mxx_2()) || 					// DIV {RIxx, ROxx, Mxx} {RIxx, ROxx, Mxx}
                (s.RIxx_1() || s.ROxx_1() || s.Mxx_1()) && (s.Number_2() && s.Get_Value2() >= -64 && s.Get_Value2() <= 63); // DIV {RIxx, ROxx, Mxx} {n}
    }

};
