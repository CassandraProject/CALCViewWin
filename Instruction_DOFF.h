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
//	File:	Instruction_DOFF.java
//

/*
DOFF
DOFF t
DOFF Mxx 
(Delayed OFF}

DOFF produces a logical zero output if the input has been in the logical zero state for the consecutive 
length of time specified by the operand. Once the timer has been activated by a zero value of the input, 
the output is forced to logical one; and remains in this state during the time-out. Once the delay has 
been completed and the output is at logical zero, the output returns to one as soon as the instruction 
reads a logical one followed by a zero at the input. On each cycle, the input is found in the accumulator, 
and the output is then written to the accumulator, overwriting the input after it has been used.

DOFF with blank operand or with a 0 constant operand uses 0.5 seconds as its delay value. If the operand 
is the integer t and greater than zero the value of t indicates the number of seconds in the delay. If 
the operand is Mxx, then the delay is determined by the contents of memory location xx. A positive value 
in Mxx is used as the delay in seconds, and a negative value is used as the delay in block execution cycles->

The maximum time delay with format DOFF t is 32,767 seconds, or approximately 9.1 hours-> The maximum delay 
with format DOFF Mxx can be obtained by loading the maximum positive floating point value into Mxx, before 
the DOFF Mxx instruction is reached in the program. Since the maximum floating point value is 1038, the 
delay is 1038 seconds-> If the block period is longer than 1.0 second, an even longer delay can be obtained 
by loading Mxx with the value of -1038. (The delay times obtainable with this instruction are effectively 
unlimited.) 

The TIMINI parameter controls DOFF behavior at block initialization time to avoid unexpected behavior of 
the DOFF timer and, possibly, unpredicted activation of the CALCA block's outputs-> If the TIMINI parameter 
is set (TIMINI = True), the timer is considered to have been in the expired state just before initialization. 
A one-to-zero transition of the accumulator after initialization is required in order to start the timer. 
If the TIMINI parameter is not set (TIMINI =False), the timer is considered to have been in the inactive 
state just before initialization, and a zero in the accumulator at block initialization begins to time the 
specified delay. The timer, therefore, begins to run.

In all cases, sptr(after) = sptr(before).

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_DOFF: public Instruction_Interface
{
    Instruction_DOFF() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        try {
            float acc = stack.Top(0);
            float n;
            
            if (s->Number_1())			n = (float) std::abs(s->Get_Value1());
            else if (s->Register_1()) 	n = (float) std::abs(reg.Index_To_Memory(s->Get_Register1())->Get_Value());
            else 			n = 1.0F;
                
            float data = s->Get_Timer();
            
            // Increment the timer
            if (acc == 0.0F) data = data + 1.0F;
            else 		 data = 0.0F;
            
            // Set the output
            if (data > n) {
                stack.Replace(0.0F);
            } else {
                stack.Replace(1.0F);
            }
        
            s->Set_Timer(data);
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
        reg.Use_Register (TIMINI);
        if (s.Register_1())	reg.Use_Register (s.Get_Register1());      
        if (s.Register_2())	reg.Use_Register (s.Get_Register2());      
    }

    bool Check (Mem_Step &s, Machine &mem) {
        return 	(s.Empty_1() && s.Empty_2()) ||
                (s.Number_1() && s.Empty_2()) ||
                (s.Mxx_1() && s.Empty_2());
    }

};
