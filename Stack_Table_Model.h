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
	
	Name: 			Stack_Table_Model.h
	Author:			Tod Baudais
				Copyright (C) 2000-2004. All rights reserved.
	
	Date Created:		9/13/2004
	Changes:		-none-
==============================================================================*/
#pragma once

#include	"Constants.h"

#include	<string>
#include	<sstream>

/*==============================================================================
==============================================================================*/

class Stack_Table_Model
{
    public:
        // Constructors 
							Stack_Table_Model	(void);
							Stack_Table_Model	(const Stack_Table_Model &rhs);
				
        // Assignment operator
        Stack_Table_Model&	operator =			(const Stack_Table_Model &rhs);		

        // Destructor
		virtual				~Stack_Table_Model	(void);
		     
    public:
		void				Clear				(void);
		void				Push				(float v);// throws Exception_Stack_Overflow;	
		void				Pop					(int n);// throws Exception_Stack_Underflow;       
		float				Top					(int n);
		int					Depth				(void);
		void				Replace				(float v);	

		int					GetRowCount			(void);
		std::string			GetValueAt			(int row);   
   
    private:
		float				_stack[NUM_STACK_SIZE];
		int					_stack_pointer;

};

/*==============================================================================
==============================================================================*/


