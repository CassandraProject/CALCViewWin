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
	
	Name: 			Machine.h
	Author:			Tod Baudais
					Copyright (C) 2000-2004. All rights reserved.
	
	Date Created:	9/13/2004
	Changes:		-none-
==============================================================================*/
#pragma once

#include	<string>
#include	<map>

class Instruction_Interface;
class Mem_Status;

class Machine
{
		
	public:
		enum {
			CALCA = 1,
			CALC = 2,
			LOGIC = 3,
			MATH = 4
		};

   
								Machine					(void);
  
		void					Initialize				(void);
		void					FireChanged				(void);

		int						Name_To_Index_Memory	(std::string mem_name);
		Mem_Status *			Index_To_Memory			(int mem_index);
		std::string				Index_To_Name_Memory	(int mem_index);

		bool					Has_Memory				(int mem_index);

		int						Name_To_Index_Instruction	(std::string mem_name);
		Instruction_Interface*	Index_To_Instruction		(int mem_index);
		std::string				Index_To_Name_Instruction	(int mem_index);

		int						Get_Num_Steps			(void)	{	return _num_steps;	}
		int						Get_Num_Memory			(void)	{	return _num_memory;	}
		int						Get_Type				(void)	{	return _type;		}

		void					Set_Up_Memory_CALCA		(void);
		void					Set_Up_Memory_CALC		(void);
		void					Set_Up_Memory_MATH		(void);
		void					Set_Up_Memory_LOGIC		(void);
    
    private:
		void					Add_Instruction			(std::string mem_name, int mem_index, Instruction_Interface* instruction);
		void					Remove_Instruction		(std::string mem_name, int mem_index);
		void					Add_Memory_Location		(std::string mem_name, int mem_index, Mem_Status *mem_location, std::string default_value);   
		void					Remove_Memory_Location	(std::string mem_name, int mem_index);   
	
		std::map<std::string,int>				_map_string_to_index_memory;
		std::map<int,std::string>				_map_index_to_string_memory;
		std::map<int,Mem_Status*>				_map_index_to_memory;

		std::map<std::string,int>				_map_string_to_index_instruction;
		std::map<int,std::string>				_map_index_to_string_instruction;
		std::map<int,Instruction_Interface*>	_map_index_to_instruction;

		std::map<std::string,std::string>		_map_name_to_name;

		int	_type;
		int	_num_steps;
		int	_num_memory;
};

