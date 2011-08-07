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
//	File:	Instruction_RND.java
//

/*
RND 
(Round)

RND reads the number in the accumulator, rounds off the value to the nearest integer, and writes the 
result into the accumulator, overwriting the original value.

If the decimal portion is less than 0.5, the value is rounded down to the integer portion of the 
number. If the decimal portion is 0.5 or greater, the value is rounded up to the next higher integer. 
In the case of negative accumulator values, if the absolute value of the decimal portion is greater 
than 0.5, the value is rounded down to the next lower integer.  
sptr(after) = sptr(before). 

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"
#include	<cmath>

struct Instruction_RND: public Instruction_Interface
{
    Instruction_RND() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        float acc = stack.Top(0);
                    
        stack.Replace(std::floor(acc + 0.5F));
        
		steps.Increment_Step();
	}

    void Update_Register_Use (Mem_Step &s, Registers_Table_Model &reg) {
    }

    bool Check (Mem_Step &s, Machine &mem) {
        return 	s.Empty_1() && s.Empty_2();
    }

};

