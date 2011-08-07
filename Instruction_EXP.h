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
//	File:	Instruction_EXP.java
//

/*
EXP
EXP {RIxx, ROxx, Mxx}
EXP {RIxx, ROxx, Mxx} {n, RIxx, ROxx, Mxx} 
(Exponent)

EXP with blank operand reads the values from the top two stack locations, raises the first operand 
(base) to the power of the second operand (exponent), decrements the stack pointer, and writes the 
result into the new accumulator location (the new top of stack location). This overwrites the first 
operand and isolates the second operand from any further access-> sptr(after) = sptr(before) - 1.

EXP RIxx reads the value stored in RIxx, raises the value that it pops from the stack to the power of 
RIxx value, then pushes the result back onto the stack. EXP ROxx and EXP Mxx do the same for the values 
stored at ROxx and memory location xx, respectively. sptr(after) = sptr(before).

EXP RIxx ROxx reads the values stored in RIxx and ROxx, raises the RIxx value to the power of ROxx 
value, and pushes the result on the stack. Similarly for the cases where the first operand is of type 
ROxx or Mxx, or the second operand is of type RIxx or Mxx. sptr(after) = sptr(before) + 1.

EXP RIxx n reads the value stored in RIxx and the specified integer constant n, raises the RIxx value 
to the power of n, and pushes the result on the stack. If the first operand is ROxx or Mxx, the operation 
is similar. sptr(after) = sptr(before) + 1.

Whenever both operands are specified, the first operand is raised to the power of the second operand.

An attempt to EXP a negative value (base less than 0) causes the value of the specified exponent to be 
written to the stack location that would have contained the final result if the operation had been 
successful. A "9" (EXP run-time error) is written to the PERROR parameter.

If the base is 0 and the exponent is less than or equal to 0, the result of the operation is 0.0. 
PERROR is not set.

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"
#include	<cmath>

struct Instruction_EXP: public Instruction_Interface
{
    Instruction_EXP() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        try {
            if (s->Empty_1()) {
                float exp = stack.Top(0);
                float base = stack.Top(1);
                
                if (base < 0.0F) {
                    stack.Pop(1);
                    stack.Replace(exp);
                    throw Exception_EXP_Error();	// error code
                } else {
                    stack.Pop(1);
                    stack.Replace((float) std::pow(base,exp));
                }
                
            } else if (s->Register_1() && s->Empty_2()) {
                float base = stack.Top(0);
                float exp = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
    
                if (base < 0.0F) {
                    stack.Replace(exp);
                    throw Exception_EXP_Error();	// error code
                } else {
                    stack.Replace((float) std::pow(base,exp));
                }
                    
            } else if (s->Register_1() && s->Number_2()) {
                float base = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
                float exp = (float) s->Get_Value2();
                        
                if (base < 0.0F) {
                    stack.Push(exp);
                    throw Exception_EXP_Error();	// error code
                } else {
                    stack.Push((float) std::pow(base,exp));
                }
    
            } else if (s->Register_1() && s->Register_2()) {
                float base = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
                float exp = reg.Index_To_Memory(s->Get_Register2())->Get_Value();
            
                if (base < 0.0F) {
                    stack.Push(exp);
                    throw Exception_EXP_Error();	// error code
                } else {
                    stack.Push((float) std::pow(base,exp));
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
        catch (Exception_EXP_Error &e) {
	    steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(9);	// error code
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
        return 	s.Empty_1() && s.Empty_2() ||									// EXP
                (s.RIxx_1() || s.ROxx_1() || s.Mxx_1()) && s.Empty_2() || 					// EXP {RIxx, ROxx, Mxx}
                (s.RIxx_1() || s.ROxx_1() || s.Mxx_1()) && (s.Number_2() || s.RIxx_2() || s.ROxx_2() || s.Mxx_2()); // EXP {RIxx, ROxx, Mxx} {n,RIxx, ROxx, Mxx}
    }

};
