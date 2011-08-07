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
/*==============================================================================
	
	Name: 			Stack_Table_Model.cpp
	Author:			Tod Baudais
					Copyright (C) 2000-2004. All rights reserved.
	
	Date Created:	9/13/2004
	Changes:		-none-
==============================================================================*/

#include "Stack_Table_Model.h"
#include "Exception_Stack_Overflow.h"
#include "Exception_Stack_Underflow.h"

/*==============================================================================
==============================================================================*/

Stack_Table_Model::Stack_Table_Model(void) 
{
    Clear();
}

Stack_Table_Model::Stack_Table_Model (const Stack_Table_Model &rhs)
{

}
				
Stack_Table_Model& Stack_Table_Model::operator = (const Stack_Table_Model &rhs)
{
	return *this;
}		

Stack_Table_Model::~Stack_Table_Model() 
{
    
}

/*==============================================================================
==============================================================================*/
    
void Stack_Table_Model::Clear() {
    _stack_pointer = -1;
}

void Stack_Table_Model::Push(float v) //throws Exception_Stack_Overflow
{
	if (_stack_pointer >= NUM_STACK_SIZE - 1)
		throw Exception_Stack_Overflow();

    ++_stack_pointer;
    
    _stack[_stack_pointer] = v;
}

void Stack_Table_Model::Pop(int n) //throws Exception_Stack_Underflow
{
	if (_stack_pointer - n < -1) 
		throw Exception_Stack_Underflow();
	
	_stack_pointer -= n;
}

float Stack_Table_Model::Top (int n) 
{
    int sp = _stack_pointer - n;

    if (sp < 0) 
		return 0.0F;

    return _stack[sp];
}

int Stack_Table_Model::Depth () 
{
     return _stack_pointer+1;
}

void Stack_Table_Model::Replace (float v) 
{
    if (_stack_pointer >= 0 && _stack_pointer < NUM_STACK_SIZE)
		_stack[_stack_pointer] = v;
    else if (_stack_pointer < 0) 
		Push(v);
}

/*==============================================================================
    Table model stuff
==============================================================================*/
    
int Stack_Table_Model::GetRowCount() {
    return NUM_STACK_SIZE;
}

std::string Stack_Table_Model::GetValueAt(int row) {
	std::stringstream s;
	s << Top(row);
	return s.str();
}
    
/*==============================================================================
==============================================================================*/


