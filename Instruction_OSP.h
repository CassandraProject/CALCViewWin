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
//	File:	Instruction_OSP.java
//

/*
OSP
OSP t
OSP Mxx 
(One-Shot Pulse)

OSP with blank operand or with a 0 constant operand uses 0.5 seconds as its pulse width value. 
If the operand is integer t and greater than zero the value of t indicates the number of seconds 
in the pulse width. If the operand is Mxx, then the pulse width is determined by the contents of
memory location xx. A positive value in Mxx is used as the pulse width in seconds, and a negative 
value is used as the pulse width in block execution cycles-> 

The maximum pulse width with format OSP t is 32,767 seconds, or approximately 9.1 hours-> The maximum 
pulse width with format OSP Mxx can be obtained by loading the maximum positive floating point value 
into Mxx, before the OSP Mxx instruction is reached in the program. Since the maximum floating point 
value is 1038, the pulse width is 1038 seconds-> If the block period is longer than 1.0 second, an 
even longer pulse width can be obtained by loading Mxx with the value of -1038. (The pulse widths 
obtainable with this instruction are effectively unlimited.) 

OSP generates a non-retriggerable (triggering is disabled while the output is true) one-shot pulse 
when the accumulator value makes a zero-to-one transition. The output remains true for a period 
specified by the operand, after which the block resets the output to zero. 

The TIMINI parameter controls OSP behavior at block initialization time to avoid unexpected behavior 
of the OSP element and, possibly, unpredicted activation of the CALCA block's outputs-> If the TIMINI 
parameter is set (TIMINI = True), the pulse is considered to have been in progress just before 
initialization. A zero-to-one transition of the accumulator after initialization is required in 
order to start the pulse output. If the TIMINI parameter is not set (TIMINI =False), the pulse is 
considered to have been off just before initialization, and a logical one in the accumulator at 
block initialization begins to output the specified pulse. 

In all cases, sptr(after) = sptr(before).

CHECKED: March 14, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_OSP: public Instruction_Interface
{
    Instruction_OSP() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        try {
            float acc = stack.Top(0);
            float n;
            
            if (s->Number_1())			n = (float) s->Get_Value1();
            else if (s->Register_1()) 	n = reg.Index_To_Memory(s->Get_Register1())->Get_Value();
            else 			n = 1.0F;
                
            float timer = s->Get_Timer();
                                    
            // Is timer timing a pulse
            if (timer > 0.0F && timer < n) {
                timer += 1.0F;
                stack.Replace(1.0F);
            
            // Is timer resetting
            } else if (acc == 0.0F) {
                timer = 0.0F;
                stack.Replace(0.0F);
                
            // Is timer restarting
            } else if (acc != 0.0F && timer == 0.0F) {
                timer += 1.0F;
                stack.Replace(1.0F);
                
            // Pulse has timed out
            } else {
                stack.Replace(0.0F);
            }
        
            s->Set_Timer(timer);
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
