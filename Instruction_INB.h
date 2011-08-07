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
//	File:	Instruction_INB.java
//

/*
INB
INB {IIxx, Mxx} 
(Input Indexed Boolean)

INB with blank operand reads the value in the accumulator, truncates it to an integer, and uses the 
result as the index of the BIxx parameter to be read. The value of BIxx is then pushed onto the stack. 
INB IIxx and INB Mxx operate similarly, except that the index of the BIxx parameter is found in IIxx 
or Mxx. (If in Mxx, the real contents of Mxx are first truncated to an integer.)

A value of True in the indexed Boolean is written to the stack as 1.0 and a value of False is written 
as 0.0. sptr(after) = sptr(before) + 1.

If the value of the index (the contents of the accumulator, IIxx, or Mxx) is less than 1 or greater 
than 16, a "10" (index run-time error) is written to the PERROR parameter.

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_INB: public Instruction_Interface
{
    Instruction_INB() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();

        try {
            if (s->Empty_1()) {
                int i = (int) stack.Top(0);
            
                if (i < 1 || i > 16) 
                    throw Exception_Index_Error();
                
                stack.Replace(reg.Index_To_Memory(BI01 + i - 1)->Get_Value());
                
            } else if (s->Register_1()) {
                int r = (int) reg.Index_To_Memory(s->Get_Register1())->Get_Value();
    
                if (r < 1 || r > 16)
                    throw Exception_Index_Error();

                stack.Push(reg.Index_To_Memory(BI01 + r - 1)->Get_Value());
            }
        }
        catch (Exception_Index_Error &e) {
	    steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(10);	// error code
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

        reg.Use_Register (BI01);
        reg.Use_Register (BI02);
        reg.Use_Register (BI03);
        reg.Use_Register (BI04);
        reg.Use_Register (BI05);
        reg.Use_Register (BI06);
        reg.Use_Register (BI07);
        reg.Use_Register (BI08);
        reg.Use_Register (BI09);
        reg.Use_Register (BI10);
        reg.Use_Register (BI11);
        reg.Use_Register (BI12);
        reg.Use_Register (BI13);
        reg.Use_Register (BI14);
        reg.Use_Register (BI15);
        reg.Use_Register (BI16);

        if (s.Register_1())	reg.Use_Register (s.Get_Register1());      
        if (s.Register_2())	reg.Use_Register (s.Get_Register2());      
    }

    bool Check (Mem_Step &s, Machine &mem) {
        return 	s.Empty_1() && s.Empty_2() ||
                (s.IIxx_1() || s.Mxx_1()) && s.Empty_2();
    }

};
