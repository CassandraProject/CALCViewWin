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
//	File:	Instruction_CHN.java
//

/*
CHN s 
(Clear Mem_Step History)

CHN s causes the history of any timer in Mem_Step s to be cleared, setting any DON, DOFF, or OSP element in
Mem_Step s to the inactive state with accumulated time of zero. sptr(after) = sptr(before).

If s is less than 1 or greater than 50, a "-3" (out-of-range syntax error value, treated as a run-time
error) is written to the PERROR parameter.

CHECKED: March 14, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_CHN: public Instruction_Interface
{
    Instruction_CHN() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();
        
        try {
            int n = s->Get_Value1();
                
            if (n < 1 || n > reg.Get_Num_Steps()) {
                throw Exception_Index_Error();	// error code
            } 
            
            steps.Clear_Timer(n);
        }
        catch (Exception_Index_Error &e) {
			steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(-3);	// error code
            throw e;
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
    }

    bool Check (Mem_Step &s, Machine &mem) {
        return s.Number_1() && s.Empty_2() && s.Get_Value1() >= 1 && s.Get_Value1() <= mem.Get_Num_Steps();									// ADD c
    }

};
