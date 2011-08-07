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
//	File:	Instruction_REL.java
//

/*
REL {ROxx, BOxx, IOxx, LOxx} 
(Clear Secure Status)

REL clears the Secure status bit of the specified output parameter to False, thereby releasing it 
and making it settable. Its effect is identical in Auto or Manual mode. The effect of REL overrides 
that of MA. sptr(after) = sptr(before). 

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_REL: public Instruction_Interface
{
    Instruction_REL() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        reg.Index_To_Memory(s->Get_Register1())->Set_Status(Mem_Status::SECURE,false);

        steps.Increment_Step();
    }

    void Update_Register_Use (Mem_Step &s, Registers_Table_Model &reg) {
        reg.Use_Register (PERROR);
        if (s.Register_1())	reg.Use_Register (s.Get_Register1());      
        if (s.Register_2())	reg.Use_Register (s.Get_Register2());      
    }

    bool Check (Mem_Step &s, Machine &mem) {
        return 	(s.ROxx_1() || s.BOxx_1() || s.IOxx_1() || s.LOxx_1()) && s.Empty_2();
    }

};
