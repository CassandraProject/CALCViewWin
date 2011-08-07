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
	
	Name: 			Mem_Step.cpp
	Author:			Tod Baudais
					Copyright (C) 2000-2004. All rights reserved.
	
	Date Created:	9/13/2004
	Changes:		-none-
==============================================================================*/

#include	"Mem_Status.h"
#include	"Instruction_Interface.h"
#include	"StringUtils.h"

#include	<list>
#include	<ctype.h>
#include	<sstream>

/*==============================================================================
==============================================================================*/

Mem_Step::Mem_Step ()
	:	Mem_Status(Mem_Status::CAN_CONFIG_VALUE | Mem_Status::CAN_CONFIG_CONNECT | Mem_Status::CAN_CHANGE_STATUS)
{
	_instruction = "";
	_timer = 0.0F;
	
	_break_point = false;
	_visited = false;

	Reset();
}

void Mem_Step::Reset () {
	// Clear this instruction
	_opcode = NOP;
	_register1 = PARAM_EMPTY;
	_register2 = PARAM_EMPTY;
	_value1 = 0;
	_value2 = 0;
	_invert1 = false;
	_invert2 = false;
	
	Set_Error(false);
}
	  
void Mem_Step::Set_Configuration_Value (std::string configuration)
{
	Set_Configuration(configuration);
}

void Mem_Step::Set_Configuration (std::string instruction) {
	// TODO: ToUppercase
	_instruction = ToUppercase(instruction);
}

std::string Mem_Step::Get_Configuration () {
	return _instruction;
}

// Parsing routines

std::string Mem_Step::Trim_Comment(std::string s, int type) {
	if (type == Machine::CALCA || type == Machine::MATH || type == Machine::LOGIC) {
		std::string sixteen_chars = s.substr (0,std::min((std::string::size_type) 16, s.size()));
		std::string::size_type comment_start = sixteen_chars.find_first_of(";");
	    
		if (comment_start == std::string::npos)
			return sixteen_chars;
		else
			return sixteen_chars.substr (0,comment_start);
	} else if (type == Machine::CALC) {
		std::string ten_chars = s.substr (0,std::min((std::string::size_type) 10, s.size()));
		std::string::size_type comment_start = ten_chars.find_first_of(";");
	    
		if (comment_start == std::string::npos)
			return ten_chars;
		else
			return ten_chars.substr (0,comment_start);
	} else 
		return s;
}

// Checks for a tilde
bool Mem_Step::Is_Inverted(std::string s) {
	std::string::size_type tilde = s.find_first_of("~");
	
	if (tilde != 0)
		return false;
	
	return true;
}

// Gets rid of the first character
std::string Mem_Step::Trim_Inverted(std::string s) {
	return s.substr(1);
}

bool Mem_Step::Parse_Int (std::string s, int &ret)
{
	if (s.size() == 0)
		return false;
		
	for (unsigned int i = 0; i < s.size(); ++i)
		if (!isdigit(s[i]) && s[i] != '+' && s[i] != '-')
			return false;
	
	std::stringstream ss(s);
	ss >> ret;
	return true;
}

bool Mem_Step::Parse_Hex (std::string s, int &ret)
{
	if (s.size() < 2)
		return false;
		
	if (s[0] != 'H')
		return false;
		
	for (unsigned int i = 1; i < s.size(); ++i)
		if (!isxdigit(s[i]))
			return false;
			
	s.erase(0,1);
	
	std::stringstream ss(s);
	ss >> std::hex >> ret;
	return true;		
}

void Mem_Step::Compile (Machine &machine) { 
	Reset();

	// Tokenize the step
	std::string instruction_no_comment = Trim_Comment(_instruction, machine.Get_Type());
	std::list<std::string> tokens = Tokenize(instruction_no_comment);
	
	// Check first token
	if (tokens.size() > 0) {
		std::string token = tokens.front();
	    
		// Get the Opcode
		try {
			_opcode = machine.Name_To_Index_Instruction(token);
		}
		catch(Exception_Illegal_Symbol &) {
			_error = true;
			return;
		}
	    
	    tokens.pop_front();
	} 


	// Check second token
	if (tokens.size() > 0) {
		std::string token = tokens.front();
		bool isNumber = false;
		bool isRegister = true;
	            
		// Try to read a number
		if (Parse_Int (token, _value1)) {
			_register1 = PARAM_NUMBER;
			isNumber = true;
		}
		
		// Try to read a number as hex
		if (Parse_Hex (token, _value1)) {
			_register1 = PARAM_NUMBER;
			isNumber = true;
		}
	    
		// Try to read a register
		try {
			_invert1 = Is_Inverted(token);
			if (_invert1)	token = Trim_Inverted(token);
           	_register1 = machine.Name_To_Index_Memory(token);
		}
		catch(Exception_Illegal_Symbol &) {
			isRegister = false;
		}
	    
		// Have we found a value
		if (!isNumber && !isRegister) {
			_error = true;
			return;
		}
	    
	    tokens.pop_front();
	} 


	// Check third token
	if (tokens.size() > 0) {
		std::string token = tokens.front();
		bool isNumber = false;
		bool isRegister = true;
	    
		// Try to read a number
		if (Parse_Int (token, _value2)) {
			_register2 = PARAM_NUMBER;
			isNumber = true;
		}
		
		// Try to read a number as hex
		if (Parse_Hex (token, _value2)) {
			_register2 = PARAM_NUMBER;
			isNumber = true;
		}
	    
		// Try to read a register
		try {
			_invert2 = Is_Inverted(token);
			if (_invert2)	token = Trim_Inverted(token);
           	_register2 = machine.Name_To_Index_Memory(token);
		}
		catch(Exception_Illegal_Symbol &) {
			isRegister = false;
		}
	    
		// Have we found a value
		if (!isNumber && !isRegister) {
			_error = true;
			return;
		}
	    
	    tokens.pop_front();
	}
	
	// Syntax Check
	Instruction_Interface *inst_impl = machine.Index_To_Instruction(_opcode);
	/*if (inst_impl == NULL || !inst_impl->Check(*this, *machine)) {
		_error = true;
		return;
	}*/
	        
} // Compile

void		Mem_Step::Blank_Jump (int step) {
	// If it's a branch, update the line numbers
	if (Is_Branch() && !Get_Error()) {
		int src_step = Get_Value1();
	    
		if (src_step == step) {

			// Extract and insert new line number
			std::stringstream src_line_string;
			src_line_string << step;

			std::string::size_type index = _instruction.find(src_line_string.str());
	                                
			std::string new_instruction = _instruction.substr(0,index) + "??" +
									_instruction.substr(index + src_line_string.str().size());
	                                
			Set_Configuration(new_instruction);
		}
	}

}

void		Mem_Step::Offset_Range (int first_step, int delta, int max) {
	// If it's a branch, update the line numbers
	if (Is_Branch() && !Get_Error()) {
		int src_step = Get_Value1();
	    
		if (src_step >= first_step) {

			// Extract and insert new line number			
			std::stringstream src_line_string;
			src_line_string << src_step;
			std::string::size_type index = _instruction.find(src_line_string.str());
	        
			int dst_step = src_step + delta;
			std::stringstream dst_line_string;
			dst_line_string << dst_step;
	                
			std::string new_instruction = _instruction.substr(0,index) + dst_line_string.str() +
									_instruction.substr(index + src_line_string.str().size());
	                                
			Set_Configuration(new_instruction);
		}
	}

}



/*==============================================================================
==============================================================================*/

