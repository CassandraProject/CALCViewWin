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
//	File:	Instruction_ABS.java
//

/*
ABS 
(Absolute Value)

ABS reads the contents of the accumulator and returns the absolute value to the accumulator, overwriting 
the original contents-> sptr(after) = sptr(before).

CHECKED: March 14, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_ABS: public Instruction_Interface
{
    Instruction_ABS() {

    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();
        
        //try {
            float acc = stack.Top(0);
            acc = (acc < 0.0F ? -acc : acc);
            stack.Replace(acc);
        //}
		steps.Increment_Step();
	}

    void Update_Register_Use (Mem_Step &s, Registers_Table_Model &reg) {

    }

    bool Check (Mem_Step &s, Machine &mem) {
        return 	s.Empty_1() && s.Empty_2();
    }

};


