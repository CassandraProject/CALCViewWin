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
//	File:	Instruction_MEDN.java
//

/*
MEDN 
(Median)

MEDN reads all the values from the stack, determines the median value, decrements the stack pointer, and writes this value into the new accumulator location (the new top of stack location). This overwrites the first operand and isolates the other operands from any further access->

The median value is determined as follows: The stack values are arranged conceptually in ascending order; then the median is the middle value if the number of operands is odd, or the average of the two middle values if the number of operands is even. sptr(after) = stackbottom. 

CHECKED: March 28, 2003
*/
#include    "Instruction_Interface.h"

struct Instruction_MEDN: public Instruction_Interface
{
    Instruction_MEDN() {
        
    }

    void Run (Steps_Table_Model &steps, Stack_Table_Model &stack, Machine &reg) {
        Mem_Step *s = steps.Get_Current_Step ();
        
        float *elements;
        
        try {

            int depth = stack.Depth();
            int c = depth;
            
            if (depth <= 0) 
                return;
            
            elements = new float[depth];
            
            for (; c > 0; --c) {
                elements[c-1] = stack.Top(0);
                if (c > 1) stack.Pop(1); // Don't pop last element
            }
            
            // Bubble sort the elements
            bool changed;
            do {
                changed = false;
                
                for (int i = 0; i < depth-1; ++i)
                    if (elements[i] > elements[i+1]) {
                        float temp = elements[i];
                        elements[i] = elements[i+1];
                        elements[i+1] = temp;
                        
                        changed = true;
                    }
                    
            } while (changed == true);
            
            // Even or odd
            if (depth % 2 == 0) {
                int i1 = (int) (depth / 2.0F);
                int i2 = (int) (depth / 2.0F) - 1;
                
                stack.Replace((elements[i1] + elements[i2]) / 2.0F);
            } else {
                int i = (int) (depth / 2.0F - 0.5F);
                stack.Replace(elements[i]);
            }
        }
        catch (Exception_Stack_Overflow &e) {
			steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(5);	// error code
            delete elements;
            throw e;
        }
        catch (Exception_Stack_Underflow &e) {
			steps.Increment_Step();
            if (reg.Index_To_Memory(PERROR)->Get_Value() == 0.0F)
                reg.Index_To_Memory(PERROR)->Set_Value(6);	// error code
            delete elements;
            throw e;
        }
        delete elements;
		steps.Increment_Step();
	}

    void Update_Register_Use (Mem_Step &s, Registers_Table_Model &reg) {
        reg.Use_Register (PERROR);
    }

    bool Check (Mem_Step &s, Machine &mem) {
        return 	s.Empty_1() && s.Empty_2();
    }

};
