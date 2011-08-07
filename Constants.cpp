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

#include	"Constants.h"

bool Is_Boolean_Input 		(int r)	{	return (r >= BI01) && (r <= BI16);	}
bool Is_Pseudo_Input 		(int r)	{	return (r >= I01) && (r <= I32);	}
bool Is_Real_Input 			(int r)	{	return (r >= RI01) && (r <= RI08);	}
bool Is_Integer_Input 		(int r)	{	return (r >= II01) && (r <= II02);	}
bool Is_Long_Integer_Input	(int r)	{	return (r >= LI01) && (r <= LI02);	}

bool Is_Boolean_Output 		(int r)	{	return (r >= BO01) && (r <= BO08);	}
bool Is_Pseudo_Output 		(int r)	{	return (r >= O01) && (r <= O32);	}
bool Is_Real_Output 		(int r)	{	return (r >= RO01) && (r <= RO04);	}
bool Is_Integer_Output 		(int r)	{	return (r >= IO01) && (r <= IO06);	}
bool Is_Long_Integer_Output	(int r)	{	return (r >= LO01) && (r <= LO02);	}

bool Is_Memory 				(int r)	{	return (r >= M01) && (r <= M24);	}

