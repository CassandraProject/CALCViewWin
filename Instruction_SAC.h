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
//
//	File:	Instruction_SAC.java
//

/*
SAC {ROxx, BOxx, IOxx, Mxx, ~BOxx, ~Mxx} 
(Store Accumulator in Output)

SAC is identical to OUT, except that outputs to the pseudo-parameters Oxx are not permitted. 
sptr(after) = sptr(before).

CHECKED: March 28, 2003
*/

#include    "Instruction_OUT.h"

struct Instruction_SAC: public Instruction_OUT
{

    bool Check (Mem_Step &s, Machine &mem) {
        return 	(s.ROxx_1() || s.BOxx_1() || s.IOxx_1() || s.Mxx_1() || 
                s.nBOxx_1() || s.nIOxx_1() || s.nLOxx_1() || s.nMxx_1()) && s.Empty_2();
    }

};
