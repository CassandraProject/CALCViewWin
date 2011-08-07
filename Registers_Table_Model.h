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
#pragma once

#include	<vector>

class Machine;

class Registers_Table_Model
{
	private:
		std::vector<int>	_register_use;
		Machine				*_machine;

	public:

		Registers_Table_Model (Machine *machine);
		
		bool			Get_OOS					(int r);
		bool			Get_BAD					(int r);
		bool			Get_ERR					(int r);
		bool			Get_SEC					(int r);

		bool			Get_Modified			(int r);
		std::string		Get_Param				(int r);
		std::string		Get_Value				(int r);
		std::string		Get_Config				(int r);


		void			Set_OOS					(int r, bool s);
		void			Set_BAD					(int r, bool s);
		void			Set_ERR					(int r, bool s);
		void			Set_SEC					(int r, bool s);

		void			Set_Modified			(int r, bool s);
		void			Set_Value				(int r, std::string s);
		void			Set_Config				(int r, std::string s);
	        
		void			Use_Register			(int r);
		void			Refresh_Registers		(void);
		void			Clear_Modified			(void);
		void			Clear_All_Status		(void);
		
		unsigned int	GetRowCount				(void)		{	return _register_use.size();	}
	    
		/**
			Observer message
		*/
	    
		//void update (Observable o, Object arg) {
			// Called when the memory changed
	        
			//Refresh_Registers();
			//fireTableDataChanged();
		//}
	    
		/*----Table Model------------------------------------------------------------------*/
	/*
		public int getRowCount() {
			return _register_use.size();
		}
	    
		public int getColumnCount() {
			return 9;
		}
	        
		public Object getValueAt(int row, int column) {    
			int index = ((Integer) _register_use.get(row)).intValue();
	    
			switch (column) {
				case GET_COLUMN:		return new Boolean (_machine.Index_To_Memory(index).Get_Other_Status(Mem_Status.ONLINE_GET));				
				case PUT_COLUMN:		return new Boolean (_machine.Index_To_Memory(index).Get_Other_Status(Mem_Status.ONLINE_PUT));				
				case OUT_OF_SERVICE_COLUMN:	return new Boolean (_machine.Index_To_Memory(index).Get_Status(Mem_Status.OOS));				
				case BAD_COLUMN:		return new Boolean (_machine.Index_To_Memory(index).Get_Status(Mem_Status.BAD));				
				case ERROR_COLUMN:		return new Boolean (_machine.Index_To_Memory(index).Get_Status(Mem_Status.ERROR));				
				case SECURE_COLUMN:		return new Boolean (_machine.Index_To_Memory(index).Get_Status(Mem_Status.SECURE));				 
				case REGISTER_COLUMN:	return new Registers_Name(_machine.Index_To_Memory(index).Get_Other_Status(Mem_Status.MODIFIED), _machine.Index_To_Name_Memory(index));				
				case NUMBER_COLUMN:		return _machine.Index_To_Memory(index).Get_Value_String();			
				case CONFIG_COLUMN:		return _machine.Index_To_Memory(index).Get_Configuration();			
				default:			return null;
			}
		}
	    
		void setValueAt(Object value, int row, int column) {
			int index = ((Integer) _register_use.get(row)).intValue();

			switch (column) {
				case GET_COLUMN:		if (value instanceof Boolean) {
												_machine.Index_To_Memory(index).Set_Other_Status(Mem_Status.ONLINE_GET,((Boolean) value).booleanValue());
											}
											break;			
				case PUT_COLUMN:		if (value instanceof Boolean) {
												_machine.Index_To_Memory(index).Set_Other_Status(Mem_Status.ONLINE_PUT,((Boolean) value).booleanValue());
											}
											break;			
				case OUT_OF_SERVICE_COLUMN:	if (value instanceof Boolean) {
												_machine.Index_To_Memory(index).Set_Status(Mem_Status.OOS,((Boolean) value).booleanValue());		
											}
											break;			
				case BAD_COLUMN:		if (value instanceof Boolean) {
												_machine.Index_To_Memory(index).Set_Status(Mem_Status.BAD,((Boolean) value).booleanValue());		
											}
											break;			
				case ERROR_COLUMN:		if (value instanceof Boolean) {
												_machine.Index_To_Memory(index).Set_Status(Mem_Status.ERROR,((Boolean) value).booleanValue());	
											}
											break;			
				case SECURE_COLUMN:		if (value instanceof Boolean) {
												_machine.Index_To_Memory(index).Set_Status(Mem_Status.SECURE,((Boolean) value).booleanValue());
											}
											break;			
				case NUMBER_COLUMN:		if (value instanceof String) {
												_machine.Index_To_Memory(index).Set_Value_String((String) value);
											}
											break;
				case CONFIG_COLUMN:		if (value instanceof String) {
												_machine.Index_To_Memory(index).Set_Configuration((String) value);
											}
											break;
			}
		}

		public String getColumnName(int column) {
		switch (column) {
				case GET_COLUMN:		return Get_String("RegTableGET");			
				case PUT_COLUMN:		return Get_String("RegTablePUT");			
				case OUT_OF_SERVICE_COLUMN:	return Get_String("RegTableOOS");			
				case BAD_COLUMN:		return Get_String("RegTableBAD");			
				case ERROR_COLUMN:		return Get_String("RegTableERR");			
				case SECURE_COLUMN:		return Get_String("RegTableSEC");			
				case REGISTER_COLUMN:	return Get_String("RegTableParam");			
				case NUMBER_COLUMN:		return Get_String("RegTableValue");			
				case CONFIG_COLUMN:		return Get_String("RegTableConfig");			
				default:			return null;
			}
		}
	    
		public Class getColumnClass(int column) {
		switch (column) {
				case GET_COLUMN:		return Boolean.class;
				case PUT_COLUMN:		return Boolean.class;
				case OUT_OF_SERVICE_COLUMN:	return Boolean.class;
				case BAD_COLUMN:		return Boolean.class;
				case ERROR_COLUMN:		return Boolean.class;
				case SECURE_COLUMN:		return Boolean.class;
				case REGISTER_COLUMN:	return Registers_Name.class;
				case NUMBER_COLUMN:		return String.class;			
				case CONFIG_COLUMN:		return String.class;			
				default:			return null;
			}
		}

		bool isCellEditable(int row, int column) {
			int index = ((Integer) _register_use.get(row)).intValue();

			switch (column) {
				case GET_COLUMN:		return _machine.Index_To_Memory(index).Can_Online();
				case PUT_COLUMN:		return _machine.Index_To_Memory(index).Can_Online();
				case OUT_OF_SERVICE_COLUMN:	return _machine.Index_To_Memory(index).Can_Change_Status();
				case BAD_COLUMN:		return _machine.Index_To_Memory(index).Can_Change_Status();
				case ERROR_COLUMN:		return _machine.Index_To_Memory(index).Can_Change_Status();
				case SECURE_COLUMN:		return _machine.Index_To_Memory(index).Can_Change_Status();
				case REGISTER_COLUMN:	return false;
				case NUMBER_COLUMN:		return true;			
				case CONFIG_COLUMN:		return true;			
				default:			return false;
			}
		}*/
};

