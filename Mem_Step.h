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
	
	Name: 			Mem_Step.h
	Author:			Tod Baudais
					Copyright (C) 2000-2004. All rights reserved.
	
	Date Created:	9/13/2004
	Changes:		-none-
==============================================================================*/
#pragma once

#include	"Constants.h"
#include	"Exception_Illegal_Memory_Use.h"
#include	"Exception_Illegal_Symbol.h"
#include	"Mem_Status.h"

class	Instruction_Interface;


#include	<list>
#include	<ctype.h>
#include	<sstream>

/*==============================================================================
==============================================================================*/
 
class Mem_Step: public Mem_Status
{    
    public:
    
		Mem_Step ();
		void Reset ();
	        
		void Set_Break (bool break_point)		{	_break_point = break_point;		}
		bool Get_Break ()						{	return _break_point;			}
		
		void Set_Error (bool error)				{	_error = error;					} 
		bool Get_Error ()						{	return _error;					}

		void Set_Visited (bool visited)			{	_visited = visited;				} 
		bool Get_Visited ()						{	return _visited;				}   
	    
 
		void			Set_Configuration_Value	(std::string configuration);
		void			Set_Configuration		(std::string instruction);
		std::string		Get_Configuration		(void);

		// Bunch of stuff that a step can't do

		void		Set_Value_String	(std::string r)		{	throw Exception_Illegal_Memory_Use();	}
		void		Set_Value			(float r)			{	throw Exception_Illegal_Memory_Use();	}   
		std::string Get_Value_String	(void)				{	throw Exception_Illegal_Memory_Use();	}
		float		Get_Value			(void)				{	throw Exception_Illegal_Memory_Use();	}
		void		Set_Status			(int bit, bool bad)	{	throw Exception_Illegal_Memory_Use();	}   
		void		Set_Status			(int status)		{	throw Exception_Illegal_Memory_Use();	}
		bool		Get_Status			(int bit)			{	throw Exception_Illegal_Memory_Use();	}    
		int			Get_Status			(void)				{	throw Exception_Illegal_Memory_Use();	}
		void		Set_Other_Status	(int bit, bool bad)	{	throw Exception_Illegal_Memory_Use();	}
		void		Set_Other_Status	(int status)		{	throw Exception_Illegal_Memory_Use();	}
		bool		Get_Other_Status	(int bit)			{	throw Exception_Illegal_Memory_Use();	}

	    // Timer value
	    
		float		Get_Timer			(void)				{	return _timer;		}
		void		Set_Timer			(float timer)		{	_timer = timer;		}
	    
	    // Compiled instruction info
	    
		int			Get_Opcode			(void)				{	return _opcode;		}
		int			Get_Register1		(void)				{	return _register1;	}
		int			Get_Register2		(void)				{	return _register2;	}
		int			Get_Value1			(void)				{	return _value1;		}
		int			Get_Value2			(void)				{	return _value2;		}

		// Parsing routines
		std::string Trim_Comment(std::string s, int type);

		// Checks for a tilde
		bool Is_Inverted(std::string s);

		// Gets rid of the first character
		std::string Trim_Inverted(std::string s);
		
		bool Parse_Int (std::string s, int &ret);
		bool Parse_Hex (std::string s, int &ret);
		void Compile (Machine &machine);
	    
	    
		bool	Empty_1()	{	return 	_register1 == PARAM_EMPTY;				}
		bool	Empty_2()	{	return 	_register2 == PARAM_EMPTY;				}

		bool	Number_1()	{	return 	_register1 == PARAM_NUMBER;				}
		bool	Number_2()	{	return 	_register2 == PARAM_NUMBER;				}

		bool	Invert_1()	{	return 	_invert1;							}
		bool	Invert_2()	{	return 	_invert2;							}

		bool	Register_1()	{	return 	!Empty_1() && !Number_1();					}
		bool	Register_2()	{	return 	!Empty_2() && !Number_2();					}
	    
		bool	Mxx_1()		{	return Is_Memory(_register1) && !_invert1;			}
		bool	Mxx_2()		{	return Is_Memory(_register2) && !_invert2;			}
		bool	nMxx_1()	{	return Is_Memory(_register1) && _invert1;			}
		bool	nMxx_2()	{	return Is_Memory(_register2) && _invert2;			}
	    
		bool	RIxx_1()	{	return Is_Real_Input(_register1) && !_invert1;		}
		bool	RIxx_2()	{	return Is_Real_Input(_register2) && !_invert2;		}
		bool	nRIxx_1()	{	return Is_Real_Input(_register1) && _invert1;			}
		bool	nRIxx_2()	{	return Is_Real_Input(_register2) && _invert2;			}

		bool	ROxx_1()	{	return Is_Real_Output(_register1) && !_invert1;		}
		bool	ROxx_2()	{	return Is_Real_Output(_register2) && !_invert2;		}
		bool	nROxx_1()	{	return Is_Real_Output(_register1) && _invert1;		}
		bool	nROxx_2()	{	return Is_Real_Output(_register2) && _invert2;		}

		bool	LIxx_1()	{	return Is_Long_Integer_Input(_register1) && !_invert1;	}
		bool	LIxx_2()	{	return Is_Long_Integer_Input(_register2) && !_invert2;	}
		bool	nLIxx_1()	{	return Is_Long_Integer_Input(_register1) && _invert1;		}
		bool	nLIxx_2()	{	return Is_Long_Integer_Input(_register2) && _invert2;		}

		bool	LOxx_1()	{	return Is_Long_Integer_Output(_register1) && !_invert1;	}
		bool	LOxx_2()	{	return Is_Long_Integer_Output(_register2) && !_invert2;	}
		bool	nLOxx_1()	{	return Is_Long_Integer_Output(_register1) && _invert1;	}
		bool	nLOxx_2()	{	return Is_Long_Integer_Output(_register2) && _invert2;	}
	    
		bool	BIxx_1()	{	return Is_Boolean_Input(_register1) && !_invert1;		}
		bool	BIxx_2()	{	return Is_Boolean_Input(_register2) && !_invert2;		}
		bool	nBIxx_1()	{	return Is_Boolean_Input(_register1) && _invert1;		}
		bool	nBIxx_2()	{	return Is_Boolean_Input(_register2) && _invert2;		}

		bool	BOxx_1()	{	return Is_Boolean_Output(_register1) && !_invert1;		}
		bool	BOxx_2()	{	return Is_Boolean_Output(_register2) && !_invert2;		}
		bool	nBOxx_1()	{	return Is_Boolean_Output(_register1) && _invert1;		}
		bool	nBOxx_2()	{	return Is_Boolean_Output(_register2) && _invert2;		}
	    
		bool	IIxx_1()	{	return Is_Integer_Input(_register1) && !_invert1;		}
		bool	IIxx_2()	{	return Is_Integer_Input(_register2) && !_invert2;		}
		bool	nIIxx_1()	{	return Is_Integer_Input(_register1) && _invert1;		}
		bool	nIIxx_2()	{	return Is_Integer_Input(_register2) && _invert2;		}

		bool	IOxx_1()	{	return Is_Integer_Output(_register1) && !_invert1;		}
		bool	IOxx_2()	{	return Is_Integer_Output(_register2) && !_invert2;		}
		bool	nIOxx_1()	{	return Is_Integer_Output(_register1) && _invert1;		}
		bool	nIOxx_2()	{	return Is_Integer_Output(_register2) && _invert2;		}

		bool	Ixx_1()		{	return Is_Pseudo_Input(_register1) && !_invert1;		}
		bool	Ixx_2()		{	return Is_Pseudo_Input(_register2) && !_invert2;		}
		bool	nIxx_1()	{	return Is_Pseudo_Input(_register1) && _invert1;		}
		bool	nIxx_2()	{	return Is_Pseudo_Input(_register2) && _invert2;		}

		bool	Oxx_1()		{	return Is_Pseudo_Output(_register1) && !_invert1;		}
		bool	Oxx_2()		{	return Is_Pseudo_Output(_register2) && !_invert2;		}
		bool	nOxx_1()	{	return Is_Pseudo_Output(_register1) && _invert1;		}
		bool	nOxx_2()	{	return Is_Pseudo_Output(_register2) && _invert2;		}
	    
		bool	Is_Branch()	{	return 	_opcode == BIF ||
										_opcode == BII ||
										_opcode == BIN ||
										_opcode == BIP ||
										_opcode == BIT ||
										_opcode == BIZ ||
										_opcode == CHN ||
										_opcode == GTO;					}
	    
		void		Blank_Jump (int step);
		void		Offset_Range (int first_step, int delta, int max);

	private:
	
		std::string	_instruction;

		int 		_opcode;
		int 		_register1;
		int			_register2;
		int 		_value1;
		int			_value2;
		bool 		_invert1;
		bool		_invert2;

		float 		_timer;

		bool		_break_point;
		bool		_error;
		bool		_visited;    
    
};

/*==============================================================================
==============================================================================*/

