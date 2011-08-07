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
//	File:	Instruction_COO.java
//

/*
COO {ROxx, BOxx, IOxx, LOxx} 
(Clear Out-of-Service Status)

COO clears the Out-of-Service status bit of the specified output parameter to False. Its 
effect is identical in Auto or Manual mode. sptr(after) = sptr(before).

CHECKED: March 14, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_COO: public Instruction_Interface
{
    Instruction_COO() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        reg.Index_To_Memory(s->Get_Register1())->Set_Status(Mem_Status::OOS,false);
        
        steps.Increment_Step();
    }

    void Update_Register_Use (Mem_Step &s, Registers_Table_Model &reg) {
        if (s.Register_1())	reg.Use_Register (s.Get_Register1());      
        if (s.Register_2())	reg.Use_Register (s.Get_Register2());      
    }

    bool Check (Mem_Step &s, Machine &mem) {
        return 	s.BOxx_1() || s.ROxx_1() || s.IOxx_1() || s.LOxx_1();	// CBD {BOxx, ROxx, IOxx, LOxx} 
    }

};
