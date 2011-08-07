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
//	File:	Instruction_NOP.java
//

/*
NOP 
(No Operation)

This instruction causes the step to be ignored. The program continues with the next instruction. 
sptr(after) = sptr(before).

CHECKED: March 28, 2003
*/
#pragma once

#include    "Instruction_Interface.h"

struct Instruction_NOP: public Instruction_Interface
{
    Instruction_NOP() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        steps.Increment_Step();
    }

    void Update_Register_Use (Mem_Step &s, Registers_Table_Model &reg) {

    }

    bool Check (Mem_Step &s, Machine &mem) {
        return (s.Empty_1() && s.Empty_2());
    }

};
