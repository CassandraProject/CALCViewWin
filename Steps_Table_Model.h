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
	
	Name: 			Steps_Table_Model.h
	Author:			Tod Baudais
					Copyright (C) 2000-2004. All rights reserved.
	
	Date Created:	9/13/2004
	Changes:		-none-
==============================================================================*/
#pragma once

#include "Exception_Illegal_Branch.h"

class Registers_Table_Model;
class Machine;
class Mem_Step;

class Steps_Table_Model
{
	private:
		int 					_current_step;
		bool					_initializing;
    
		Registers_Table_Model	*_registers;
		Machine					*_machine;
    
    public:
						Steps_Table_Model		(Machine *machine, Registers_Table_Model *reg); 
 
		void			Set_Initializing		(bool initializing);
		bool			Get_Initializing		(void);
		
		void			Increment_Step			(void);
		void			Jump_Step				(int step);
		void			Jump_Step_Backward		(int step);
		void			Jump_End				(void);
		int				Get_Num_Steps			(void);
		
		Mem_Step*		Get_Step				(int step);
		Mem_Step*		Get_Current_Step		(void);
		int				Get_Current_Step_Index	(void)		{	return _current_step;	}
		
		bool			Is_Done					(void);
		
		void			Begin_Running_Steps		(bool timini);
		void			Stop_Running_Steps		(void);
		
		void			Compile_Steps			(void);
		void			Compile_Step			(int step);
		
		bool			Has_Error				(void);
		
		void			Clear_Timers			(void);
		void			Clear_Timer				(int s);
		void			Init_Timer				(int s, bool timini);
		
		void			Insert_Line				(int s);
		void			Delete_Line				(int s);
    
/**
    Returns the number of rows in the table.
*/    
     
    /*public int getRowCount() {
        return _machine->Get_Num_Steps();
    }*/
    
/**
    Returns the number of columns in the table.
*/    

    /*public int getColumnCount() {
        return 3;
    }*/
        
/**
    Returns the name of the column.
    @param column	Column in the table.
*/    

    /*public String getColumnName(int column) {
       switch (column) {
            case BREAK_COLUMN:		return Get_String("StepTableBP");
            case NUMBER_COLUMN:		return Get_String("StepTableStep");			
            case INSTRUCTION_COLUMN:	return Get_String("StepTableStatement");	
            default:			return null;
        }
    }*/
    
/**
    Returns the class that represents the column type.
    @param column	Column in the table.
*/    

    /*public Class getColumnClass(int column) {
       switch (column) {
            case BREAK_COLUMN:		return bool.class;
            case NUMBER_COLUMN:		return Steps_Line_Number.class;			
            case INSTRUCTION_COLUMN:	return String.class;	
            default:			return null;
        }
    }*/

/**
    Is this cell in the table editable.
    @param row		Row in the table.
    @param column	Column in the table.
*/    

    /*public bool isCellEditable(int row, int column) {
       switch (column) {
            case BREAK_COLUMN:		return true;
            case NUMBER_COLUMN:		return false;			
            case INSTRUCTION_COLUMN:	return true;	
            default:			return false;
        }
    }*/
    
/**
    Sets the value of the cell.
    @param row		Row in the table.
    @param column	Column in the table.
*/    

    /*public void setValueAt(Object value, int row, int column) {
        Mem_Step step = Get_Step (row+1);
        
        switch (column) {
            case BREAK_COLUMN:		if (value instanceof bool) {
                                            step->Set_Break(((bool) value).boolValue());
                                            _machine->FireChanged ();
                                        }
                                        break;
            case INSTRUCTION_COLUMN:	if (value instanceof String) {
                                            step->Set_Configuration((String) value);
                                            step->Compile(_machine);
                                            _machine->FireChanged ();
                                        }
                                        break;	
        }
    }*/
    
/**
    Returns the value of the cell.
    @param row		Row in the table.
    @param column	Column in the table.
*/    

    /*public Object getValueAt(int row, int column) {    
        Mem_Step step = Get_Step (row+1);

        switch (column) {
            case BREAK_COLUMN:		return new bool(step->Get_Break());				
            case NUMBER_COLUMN:		return new Steps_Line_Number(step->Get_Visited(), ((_current_step-1) == row), step->Get_Error(), row+1);			
            case INSTRUCTION_COLUMN:	return step->Get_Configuration();	
            default:			return null;
        }
    }*/

    
};


