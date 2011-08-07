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
	
	Name: 			Steps_Table_Model.cpp
	Author:			Tod Baudais
					Copyright (C) 2000-2004. All rights reserved.
	
	Date Created:	9/13/2004
	Changes:		-none-
==============================================================================*/

#include "Steps_Table_Model.h"

#include "Exception_Illegal_Branch.h"
#include "Registers_Table_Model.h"
#include "Machine.h"
#include "Mem_Step.h"


/**
Constructor for the class.
@param reg	The registers table list so we can update it when the steps change.
*/

Steps_Table_Model::Steps_Table_Model (Machine *machine, Registers_Table_Model *reg) 
{            
    // Remember the registers table so we can update it
    _registers = reg;    
    _machine = machine;
    
    _current_step = 1;
            
    _initializing = false;
    
    //_machine->addObserver(this);
}

/**
Initializing flag. If initializing, a different code path may be taken.
@param initializing	Set or clear the initializing flag.
*/    

void Steps_Table_Model::Set_Initializing (bool initializing) 
{
    _initializing = initializing;
}

/**
Returns the Initializing flag.
*/    

bool Steps_Table_Model::Get_Initializing () 
{
    return _initializing;
}

    
/**
Go to the next program step->
*/    

void Steps_Table_Model::Increment_Step () 
{
    ++_current_step;
}

/**
Jump to the program step->
@param step		Mem_Step 1 thru 50.
*/    

void Steps_Table_Model::Jump_Step (int step) 
{        
    if (step <= _current_step)
        throw Exception_Illegal_Branch();

    _current_step = step;
}

/**
Jump to the program step without throwing an exception.
@param step		Mem_Step 1 thru 50.
*/    

void Steps_Table_Model::Jump_Step_Backward (int step) 
{
    _current_step = step;
}

/**
Jump to the end step->
*/    

void Steps_Table_Model::Jump_End () 
{
    _current_step = _machine->Get_Num_Steps() + 1;
}

/**
Returns a step->
@param step		Mem_Step 1 thru 50.
*/    

Mem_Step* Steps_Table_Model::Get_Step (int step) 
{
    return (Mem_Step*) _machine->Index_To_Memory(STEP01 + step - 1);
}

/**
What step are we presently on.
*/    

Mem_Step* Steps_Table_Model::Get_Current_Step () 
{
    return Get_Step(_current_step);
}


int Steps_Table_Model::Get_Num_Steps()			
{	
	return _machine->Get_Num_Steps();
}

/**
Are we done executing the steps.
*/    

bool Steps_Table_Model::Is_Done () 
{
    return _current_step > _machine->Get_Num_Steps();
}

/**
Takes all of the necessary steps to start executing the block. Timers are also initialized.
@param init		Block is initializing.
@param timini	The value of the timini register since timers are also initialized.
*/    

void Steps_Table_Model::Begin_Running_Steps (bool timini) 
{
    Stop_Running_Steps();

    for (short s = 1; s <= _machine->Get_Num_Steps(); ++s) {
        Mem_Step *step = Get_Step (s);
                    
        if (_initializing) {
            Init_Timer (s, timini);   
            _machine->Initialize();	// error code
        }
    }
}

/**
Takes all of the necessary steps to stop the block.
*/    

void Steps_Table_Model::Stop_Running_Steps () 
{
    for (short s = 1; s <= _machine->Get_Num_Steps(); ++s) {
        Mem_Step *step = Get_Step (s);
        step->Set_Visited(false);
    }

    // Start on step 1
    Jump_Step_Backward(1);
}

/**
Compiles all of the steps.
*/    

void Steps_Table_Model::Compile_Steps () 
{
    for (short s = 1; s <= _machine->Get_Num_Steps(); ++s) {
        Mem_Step *step = Get_Step (s);
        step->Compile(*_machine);      
    }
    
    _registers->Refresh_Registers();
}

void Steps_Table_Model::Compile_Step (int step)
{
	Mem_Step *s = Get_Step (step);
	s->Compile(*_machine);   
	
	_registers->Refresh_Registers();   
}

/**
Compiles all of the steps.
*/    

bool Steps_Table_Model::Has_Error () 
{
    for (short s = 1; s <= _machine->Get_Num_Steps(); ++s) {
        Mem_Step *step = Get_Step (s);
        if (step->Get_Error())	
            return true;     
    }
    
    return false;
}

/**
Clears the timer of all steps.
*/    

void Steps_Table_Model::Clear_Timers () 
{
    for (short step = 1; step <= _machine->Get_Num_Steps(); ++step)
        Clear_Timer (step);        
}

/**
Clears the timer of a certain step->
@param s		The step that has to be cleared.
*/    

void Steps_Table_Model::Clear_Timer (int s) {
    Mem_Step *step = Get_Step (s);
    step->Set_Timer(0.0F);
}

/**
Clears the timer of a certain step->
@param step		The step that has to be cleared.
@param timini	The value of the timini register.
*/    

void Steps_Table_Model::Init_Timer (int s, bool timini) 
{
    Mem_Step *step = Get_Step (s);
    
    if (timini) {
        switch (step->Get_Opcode()) {
            case OSP:		step->Set_Timer((float) step->Get_Value1() + 1.0F);	break;
            case DON:		step->Set_Timer((float) step->Get_Value1() + 1.0F);	break;
            case DOFF:		step->Set_Timer((float) step->Get_Value1() + 1.0F);	break;	
            default:		step->Set_Timer((float) step->Get_Value1() + 1.0F);	break;        
        }
    } else {
        switch (step->Get_Opcode()) {
            case OSP:		step->Set_Timer(0.0F);	break;
            case DON:		step->Set_Timer(0.0F);	break;
            case DOFF:		step->Set_Timer(0.0F);	break;	
            default:		step->Set_Timer(0.0F);	break;        
        }
    }
}


void Steps_Table_Model::Delete_Line	(int s)
{	
	Mem_Step *dst_step;

    // Blank jumps going to row
    for (int r = 0; r < Get_Num_Steps(); ++r) {
        dst_step = Get_Step(r+1);
        dst_step->Blank_Jump(s+1);
    }

    Compile_Steps();

    // Copy last rows up
    for (int r = s; r < Get_Num_Steps() - 1; ++r) {
        dst_step = Get_Step(r+1);
        Mem_Step *src_step = Get_Step(r+1+1);
        dst_step->Set_Configuration(src_step->Get_Configuration());
    }

    // Clear Rows
    dst_step = Get_Step(Get_Num_Steps());
    dst_step->Set_Configuration("");  
                        
    Compile_Steps();

    // Offset indices
    for (int r = 0; r < Get_Num_Steps(); ++r) {
        dst_step = Get_Step(r+1);
        dst_step->Offset_Range(s+1,-1, Get_Num_Steps());
    }
    
    Compile_Steps();
}


void Steps_Table_Model::Insert_Line	(int s)
{
    Mem_Step *dst_step;

    // Blank jumps going to step 50 because it'll disappear
    for (int r = 0; r < Get_Num_Steps(); ++r) {
        dst_step = Get_Step(r+1);
        dst_step->Blank_Jump(Get_Num_Steps());
    }
    
    Compile_Steps();

    // Copy last rows down
    for (int r = Get_Num_Steps() - 1; r > s; --r) {
        dst_step = Get_Step(r+1);
        Mem_Step *src_step = Get_Step(r+1-1);
        dst_step->Set_Configuration(src_step->Get_Configuration());
    }

    // Clear Rows
    dst_step = Get_Step(s+1);
    dst_step->Set_Configuration("");  
    
    Compile_Steps();

    // Offset indices
    for (int r = 0; r < Get_Num_Steps(); ++r) {
        dst_step = Get_Step(r+1);
        dst_step->Offset_Range(s+1, 1, Get_Num_Steps());
    }

    Compile_Steps();
}

/**
Observer message
*/

/*void Steps_Table_Model::update (Observable o, Object arg) {
    // Called when the memory changed
    Compile_Steps ();
    //fireTableDataChanged();
}*/

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


