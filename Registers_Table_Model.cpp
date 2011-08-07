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
	
	Name: 			Registers_Table_Model.h
	Author:			Tod Baudais
					Copyright (C) 2000-2004. All rights reserved.
	
	Date Created:	9/13/2004
	Changes:		-none-
==============================================================================*/

#include	"Registers_Table_Model.h"
#include	"Machine.h"
#include	"Mem_Step.h"
#include	"Instruction_Interface.h"
#include	<algorithm>

Registers_Table_Model::Registers_Table_Model (Machine *machine) {        
	_machine = machine;
}
	
void Registers_Table_Model::Use_Register (int r) {
	std::vector<int>::iterator reg = std::find(_register_use.begin(), _register_use.end(), r);
	if (reg == _register_use.end())
		_register_use.push_back(r);
}

void Registers_Table_Model::Refresh_Registers() {
	_register_use.clear();
	
	for (short s = 1; s <= _machine->Get_Num_Steps(); ++s) {
		Mem_Step *step = dynamic_cast<Mem_Step*>(_machine->Index_To_Memory(STEP01 + s - 1));
		if (!step)	continue;
	            
		// Mem_Float Update
		Instruction_Interface *inst_impl = _machine->Index_To_Instruction(step->Get_Opcode());
		inst_impl->Update_Register_Use (*step, *this);
	}

	std::sort(_register_use.begin(), _register_use.end()/*, std::less<int>*/);
	
	Clear_Modified ();
}

void Registers_Table_Model::Clear_Modified () {
	for (unsigned int r = 0; r < _register_use.size(); ++r) {
		_machine->Index_To_Memory(_register_use[r])->Set_Other_Status(Mem_Status::MODIFIED, false);     
	}
}

void Registers_Table_Model::Clear_All_Status () {
	for (unsigned int r = 0; r < _register_use.size(); ++r) {
		_machine->Index_To_Memory(_register_use[r])->Set_Other_Status(0);     
		_machine->Index_To_Memory(_register_use[r])->Set_Status(0);     
	}      
}
	    
bool Registers_Table_Model::Get_OOS		(int r)		
{	
	return _machine->Index_To_Memory(_register_use[r])->Get_Status(Mem_Status::OOS);
}

bool Registers_Table_Model::Get_BAD		(int r)		
{	
	return _machine->Index_To_Memory(_register_use[r])->Get_Status(Mem_Status::BAD);	
}

bool Registers_Table_Model::Get_ERR		(int r)		
{	
	return _machine->Index_To_Memory(_register_use[r])->Get_Status(Mem_Status::ERROR_);	
}

bool Registers_Table_Model::Get_SEC		(int r)		
{	
	return _machine->Index_To_Memory(_register_use[r])->Get_Status(Mem_Status::SECURE);	
}

bool Registers_Table_Model::Get_Modified	(int r)		
{	
	return _machine->Index_To_Memory(_register_use[r])->Get_Other_Status(Mem_Status::MODIFIED);	
}

std::string		 Registers_Table_Model::Get_Param				(int r)
{
	return _machine->Index_To_Name_Memory(_register_use[r]);
}

std::string Registers_Table_Model::Get_Value		(int r)		
{	
	return _machine->Index_To_Memory(_register_use[r])->Get_Value_String();				
}

std::string Registers_Table_Model::Get_Config		(int r)		
{	
	return _machine->Index_To_Memory(_register_use[r])->Get_Configuration();			
}


void Registers_Table_Model::Set_OOS		(int r, bool s)			
{	
	return _machine->Index_To_Memory(_register_use[r])->Set_Status(Mem_Status::OOS,s);		
}

void Registers_Table_Model::Set_BAD		(int r, bool s)			
{	
	return _machine->Index_To_Memory(_register_use[r])->Set_Status(Mem_Status::BAD,s);		
}

void Registers_Table_Model::Set_ERR		(int r, bool s)			
{	
	return _machine->Index_To_Memory(_register_use[r])->Set_Status(Mem_Status::ERROR_,s);	
}

void Registers_Table_Model::Set_SEC		(int r, bool s)			
{	
	return _machine->Index_To_Memory(_register_use[r])->Set_Status(Mem_Status::SECURE,s);			
}

void Registers_Table_Model::Set_Modified	(int r, bool s)			
{	
	return _machine->Index_To_Memory(_register_use[r])->Set_Other_Status(Mem_Status::MODIFIED,s);	
}

void Registers_Table_Model::Set_Value		(int r, std::string s)	
{	
	return _machine->Index_To_Memory(_register_use[r])->Set_Value_String(s);						
}

void Registers_Table_Model::Set_Config		(int r, std::string s)	
{	
	return _machine->Index_To_Memory(_register_use[r])->Set_Configuration(s);						
}

