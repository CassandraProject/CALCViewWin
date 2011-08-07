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
//	File:	Instruction_CLA.java
//

/*
CLA 
(Clear All Machine Registers)

CLA clears all 24 memory registers-> sptr(after) = sptr(before).

CHECKED: March 14, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_CLA: public Instruction_Interface
{
    Instruction_CLA() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();
        
        reg.Index_To_Memory(M01)->Set_Value(0.0F);
        reg.Index_To_Memory(M02)->Set_Value(0.0F);
        reg.Index_To_Memory(M03)->Set_Value(0.0F);
        reg.Index_To_Memory(M04)->Set_Value(0.0F);
        reg.Index_To_Memory(M05)->Set_Value(0.0F);
        reg.Index_To_Memory(M06)->Set_Value(0.0F);
        reg.Index_To_Memory(M07)->Set_Value(0.0F);
        reg.Index_To_Memory(M08)->Set_Value(0.0F);
        reg.Index_To_Memory(M09)->Set_Value(0.0F);
        reg.Index_To_Memory(M10)->Set_Value(0.0F);
        reg.Index_To_Memory(M11)->Set_Value(0.0F);
        reg.Index_To_Memory(M12)->Set_Value(0.0F);
        reg.Index_To_Memory(M13)->Set_Value(0.0F);
        reg.Index_To_Memory(M14)->Set_Value(0.0F);
        reg.Index_To_Memory(M15)->Set_Value(0.0F);
        reg.Index_To_Memory(M16)->Set_Value(0.0F);
        reg.Index_To_Memory(M17)->Set_Value(0.0F);
        reg.Index_To_Memory(M18)->Set_Value(0.0F);
        reg.Index_To_Memory(M19)->Set_Value(0.0F);
        reg.Index_To_Memory(M20)->Set_Value(0.0F);
        reg.Index_To_Memory(M21)->Set_Value(0.0F);
        reg.Index_To_Memory(M22)->Set_Value(0.0F);
        reg.Index_To_Memory(M23)->Set_Value(0.0F);
        reg.Index_To_Memory(M24)->Set_Value(0.0F);
         
        steps.Increment_Step();
    }

    void Update_Register_Use (Mem_Step &s, Registers_Table_Model &reg) {

    }

    bool Check (Mem_Step &s, Machine &mem) {
        return 	s.Empty_1() && s.Empty_2();
    }

};
