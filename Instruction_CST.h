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
//	File:	Instruction_CST.java
//

/*
CST 
(Clear Stack)

CST resets the stack pointer one position below the bottom of the stack, waiting for the first value 
to be pushed onto the stack. Since the block implicitly executes a CST prior to execution, it is not 
required to start a program with a CST instruction. 

It is, however, recommended that you issue a CST instruction just before setting up operands for a 
polyadic instruction, since polyadic instructions operate on all values currently pushed onto the 
stack. sptr(after) = stackbottom -1.

CHECKED: March 14, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_CST: public Instruction_Interface
{
    Instruction_CST() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();
        
        stack.Clear();
        
        steps.Increment_Step();
    }

    void Update_Register_Use (Mem_Step &s, Registers_Table_Model &reg) {

    }

    bool Check (Mem_Step &s, Machine &mem) {
        return 	s.Empty_1() && s.Empty_2();
    }

};
