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
	
	Name: 			Machine.cpp
	Author:			Tod Baudais
					Copyright (C) 2000-2004. All rights reserved.
	
	Date Created:	9/13/2004
	Changes:		-none-
==============================================================================*/

#include "Machine.h"

#include "Mem_Bit_Reference.h"
#include "Mem_Boolean.h"
#include "Mem_Float.h"
#include "Mem_Int.h"
#include "Mem_Step.h"
#include "Mem_String.h"
#include "Mem_Type.h"

#include	<algorithm>

#include "Instruction_Interface.h"
#include "Instruction_ABS.h"
#include "Instruction_ACOS.h"
#include "Instruction_ADD.h"
#include "Instruction_ALN.h"
#include "Instruction_ALOG.h"
#include "Instruction_AND.h"
#include "Instruction_ANDX.h"
#include "Instruction_ASIN.h"
#include "Instruction_ATAN.h"
#include "Instruction_AVE.h"
#include "Instruction_BIF.h"
#include "Instruction_BII.h"
#include "Instruction_BIN.h"
#include "Instruction_BIP.h"
#include "Instruction_BIT.h"
#include "Instruction_BIZ.h"
#include "Instruction_CBD.h"
#include "Instruction_CE.h"
#include "Instruction_CHI.h"
#include "Instruction_CHN.h"
#include "Instruction_CHS.h"
#include "Instruction_CLA.h"
#include "Instruction_CLE.h"
#include "Instruction_CLL.h"
#include "Instruction_CLM.h"
#include "Instruction_CLR.h"
#include "Instruction_CLRB.h"
#include "Instruction_COO.h"
#include "Instruction_COS.h"
#include "Instruction_CST.h"
#include "Instruction_DEC.h"
#include "Instruction_DIV.h"
#include "Instruction_DOFF.h"
#include "Instruction_DON.h"
#include "Instruction_DUP.h"
#include "Instruction_END.h"
#include "Instruction_EXIT.h"
#include "Instruction_EXP.h"
#include "Instruction_FF.h"
#include "Instruction_GTI.h"
#include "Instruction_GTO.h"
#include "Instruction_IDIV.h"
#include "Instruction_IMOD.h"
#include "Instruction_IN.h"
#include "Instruction_INB.h"
#include "Instruction_INC.h"
#include "Instruction_INH.h"
#include "Instruction_INL.h"
#include "Instruction_INR.h"
#include "Instruction_INS.h"
#include "Instruction_LAC.h"
#include "Instruction_LACI.h"
#include "Instruction_LN.h"
#include "Instruction_LOG.h"
#include "Instruction_MAX.h"
#include "Instruction_MAXO.h"
#include "Instruction_MEDN.h"
#include "Instruction_MIN.h"
#include "Instruction_MRS.h"
#include "Instruction_MUL.h"
#include "Instruction_NAN.h"
#include "Instruction_NAND.h"
#include "Instruction_NANX.h"
#include "Instruction_NOP.h"
#include "Instruction_NOR.h"
#include "Instruction_NORX.h"
#include "Instruction_NOT.h"
#include "Instruction_NOTX.h"
#include "Instruction_NXO.h"
#include "Instruction_NXOR.h"
#include "Instruction_NXOX.h"
#include "Instruction_OR.h"
#include "Instruction_ORX.h"
#include "Instruction_OSP.h"
#include "Instruction_OUT.h"
#include "Instruction_POP.h"
#include "Instruction_PRI.h"
#include "Instruction_PRO.h"
#include "Instruction_PRP.h"
#include "Instruction_RAND.h"
#include "Instruction_RANG.h"
#include "Instruction_RBD.h"
#include "Instruction_RCL.h"
#include "Instruction_RCN.h"
#include "Instruction_RE.h"
#include "Instruction_REL.h"
#include "Instruction_RER.h"
#include "Instruction_RND.h"
#include "Instruction_RON.h"
#include "Instruction_ROO.h"
#include "Instruction_RQE.h"
#include "Instruction_RQL.h"
#include "Instruction_SAC.h"
#include "Instruction_SBD.h"
#include "Instruction_SE.h"
#include "Instruction_SEC.h"
#include "Instruction_SEED.h"
#include "Instruction_SET.h"
#include "Instruction_SETB.h"
#include "Instruction_SIEC.h"
#include "Instruction_SIN.h"
#include "Instruction_SOO.h"
#include "Instruction_SQR.h"
#include "Instruction_SQRT.h"
#include "Instruction_SSF.h"
#include "Instruction_SSI.h"
#include "Instruction_SSN.h"
#include "Instruction_SSP.h"
#include "Instruction_SST.h"
#include "Instruction_SSZ.h"
#include "Instruction_STH.h"
#include "Instruction_STL.h"
#include "Instruction_STM.h"
#include "Instruction_STMI.h"
#include "Instruction_SUB.h"
#include "Instruction_SWP.h"
#include "Instruction_TAN.h"
#include "Instruction_TIM.h"
#include "Instruction_TRC.h"
#include "Instruction_TSTB.h"
#include "Instruction_XOR.h"
#include "Instruction_XORX.h"

Machine::Machine (void)
{
	_type = 0;
	_num_steps = 0;
	_num_memory = 0;
	
    Set_Up_Memory_CALCA();
    Initialize();
    
    // Alternate names for registers
    _map_name_to_name["M1"] = "M01";
    _map_name_to_name["M2"] = "M02";
    _map_name_to_name["M3"] = "M03";
    _map_name_to_name["M4"] = "M04";
    _map_name_to_name["M5"] = "M05";
    _map_name_to_name["M6"] = "M06";
    _map_name_to_name["M7"] = "M07";
    _map_name_to_name["M8"] = "M08";
    _map_name_to_name["M9"] = "M09";

    _map_name_to_name["BI1"] = "BI01";
    _map_name_to_name["BI2"] = "BI02";
    _map_name_to_name["BI3"] = "BI03";
    _map_name_to_name["BI4"] = "BI04";
    _map_name_to_name["BI5"] = "BI05";
    _map_name_to_name["BI6"] = "BI06";
    _map_name_to_name["BI7"] = "BI07";
    _map_name_to_name["BI8"] = "BI08";
    _map_name_to_name["BI9"] = "BI09";

    _map_name_to_name["BO1"] = "BO01";
    _map_name_to_name["BO2"] = "BO02";
    _map_name_to_name["BO3"] = "BO03";
    _map_name_to_name["BO4"] = "BO04";
    _map_name_to_name["BO5"] = "BO05";
    _map_name_to_name["BO6"] = "BO06";
    _map_name_to_name["BO7"] = "BO07";
    _map_name_to_name["BO8"] = "BO08";

    _map_name_to_name["II1"] = "II01";
    _map_name_to_name["II2"] = "II02";

    _map_name_to_name["IO1"] = "IO01";
    _map_name_to_name["IO2"] = "IO02";
    _map_name_to_name["IO3"] = "IO03";
    _map_name_to_name["IO4"] = "IO04";
    _map_name_to_name["IO5"] = "IO05";
    _map_name_to_name["IO6"] = "IO06";

    _map_name_to_name["LI1"] = "LI01";
    _map_name_to_name["LI2"] = "LI02";

    _map_name_to_name["LO1"] = "LO01";
    _map_name_to_name["LO2"] = "LO02";

    _map_name_to_name["RI1"] = "RI01";
    _map_name_to_name["RI2"] = "RI02";
    _map_name_to_name["RI3"] = "RI03";
    _map_name_to_name["RI4"] = "RI04";
    _map_name_to_name["RI5"] = "RI05";
    _map_name_to_name["RI6"] = "RI06";
    _map_name_to_name["RI7"] = "RI07";
    _map_name_to_name["RI8"] = "RI08";

    _map_name_to_name["RO1"] = "RO01";
    _map_name_to_name["RO2"] = "RO02";
    _map_name_to_name["RO3"] = "RO03";
    _map_name_to_name["RO4"] = "RO04";

    _map_name_to_name["I1"] = "I01";
    _map_name_to_name["I2"] = "I02";
    _map_name_to_name["I3"] = "I03";
    _map_name_to_name["I4"] = "I04";
    _map_name_to_name["I5"] = "I05";
    _map_name_to_name["I6"] = "I06";
    _map_name_to_name["I7"] = "I07";
    _map_name_to_name["I8"] = "I08";
    _map_name_to_name["I9"] = "I09";

    _map_name_to_name["O1"] = "O01";
    _map_name_to_name["O2"] = "O02";
    _map_name_to_name["O3"] = "O03";
    _map_name_to_name["O4"] = "O04";
    _map_name_to_name["O5"] = "O05";
    _map_name_to_name["O6"] = "O06";
    _map_name_to_name["O7"] = "O07";
    _map_name_to_name["O8"] = "O08";
    _map_name_to_name["O9"] = "O09";
}

void Machine::Initialize () {
    try {
        Index_To_Memory(RNDSEED)->Set_Value(100001.0F);	// error code
    }
    catch(Exception_Illegal_Symbol &) {
    
    }
}  

/*void Machine::FireChanged () {
    setChanged();
    //notifyObservers();
}*/

/**
*/    

int Machine::Name_To_Index_Memory (std::string mem_name)
{
    std::string real_name;
    
    // Check for aliases
    std::map<std::string,std::string>::iterator alias = _map_name_to_name.find(mem_name);
    
    if (alias != _map_name_to_name.end())
        real_name = alias->second;
    else
        real_name = mem_name;

    // Get register
    std::map<std::string,int>::iterator reg = _map_string_to_index_memory.find(real_name);
    
    if (reg != _map_string_to_index_memory.end())
        return reg->second;
    else
        throw Exception_Illegal_Symbol();
}

/**
*/    

Mem_Status * Machine::Index_To_Memory (int mem_index)
{
    std::map<int,Mem_Status*>::iterator mem = _map_index_to_memory.find(mem_index);

    if (mem != _map_index_to_memory.end())
        return mem->second;
    else
        throw Exception_Illegal_Symbol();
}

/**
*/    

std::string Machine::Index_To_Name_Memory (int mem_index)
{
    std::map<int,std::string>::iterator mem = _map_index_to_string_memory.find(mem_index);

    if (mem != _map_index_to_string_memory.end())
        return mem->second;
    else
        throw Exception_Illegal_Symbol();
}

/**
*/    

bool Machine::Has_Memory (int mem_index)
{
    std::map<int,std::string>::iterator mem = _map_index_to_string_memory.find(mem_index);
    return mem != _map_index_to_string_memory.end();
}


/**
*/    

int Machine::Name_To_Index_Instruction (std::string mem_name)
{
        // Get register
    std::map<std::string,int>::iterator reg = _map_string_to_index_instruction.find(mem_name);
    
    if (reg != _map_string_to_index_instruction.end())
        return reg->second;
    else
        throw Exception_Illegal_Symbol();
}

/**
*/    

Instruction_Interface* Machine::Index_To_Instruction (int mem_index)
{
    std::map<int,Instruction_Interface*>::iterator mem = _map_index_to_instruction.find(mem_index);

    if (mem != _map_index_to_instruction.end())
        return mem->second;
    else
        throw Exception_Illegal_Symbol();
}

/**
*/    

std::string Machine::Index_To_Name_Instruction (int mem_index)
{
    std::map<int,std::string>::iterator mem = _map_index_to_string_instruction.find(mem_index);

    if (mem != _map_index_to_string_instruction.end())
        return mem->second;
    else
        throw Exception_Illegal_Symbol();
}

/**
*/    

void Machine::Add_Instruction (std::string mem_name, int mem_index, Instruction_Interface* instruction)
{
    std::map<std::string,int>::iterator				reg1 = _map_string_to_index_instruction.find(mem_name);
	std::map<int,Instruction_Interface*>::iterator	reg2 = _map_index_to_instruction.find(mem_index);
	std::map<int,std::string>::iterator 			reg3 = _map_index_to_string_instruction.find(mem_index);

    _map_string_to_index_instruction[mem_name] = mem_index;
    _map_index_to_string_instruction[mem_index] = mem_name;
        
    if (reg2 == _map_index_to_instruction.end())
        _map_index_to_instruction[mem_index] = instruction;
    else
		delete instruction;
    }

void Machine::Remove_Instruction (std::string mem_name, int mem_index)
{
    std::map<std::string,int>::iterator				reg1 = _map_string_to_index_instruction.find(mem_name);
	std::map<int,Instruction_Interface*>::iterator	reg2 = _map_index_to_instruction.find(mem_index);
	std::map<int,std::string>::iterator 			reg3 = _map_index_to_string_instruction.find(mem_index);

	if (reg1 != _map_string_to_index_instruction.end())
		_map_string_to_index_instruction.erase(reg1);
	if (reg3 != _map_index_to_string_instruction.end())
		_map_index_to_string_instruction.erase(reg3);
		
	if (reg2 != _map_index_to_instruction.end()) {
		delete reg2->second;
		_map_index_to_instruction.erase(reg2);
	}

}

/**
*/    

void Machine::Add_Memory_Location (std::string mem_name, int mem_index, Mem_Status *mem_location, std::string default_value)
{
	mem_location->Set_Configuration_Value(default_value);

    std::map<std::string,int>::iterator			reg1 = _map_string_to_index_memory.find(mem_name);
	std::map<int,Mem_Status*>::iterator			reg2 = _map_index_to_memory.find(mem_index);
	std::map<int,std::string>::iterator 		reg3 = _map_index_to_string_memory.find(mem_index);

    _map_string_to_index_memory[mem_name] = mem_index;
    _map_index_to_string_memory[mem_index] = mem_name;
        
    if (reg2 == _map_index_to_memory.end())
        _map_index_to_memory[mem_index] = mem_location;
    else
		delete mem_location;            
}

/**
*/    

void Machine::Remove_Memory_Location (std::string mem_name, int mem_index)
{
    std::map<std::string,int>::iterator				reg1 = _map_string_to_index_memory.find(mem_name);
	std::map<int,Mem_Status*>::iterator				reg2 = _map_index_to_memory.find(mem_index);
	std::map<int,std::string>::iterator 			reg3 = _map_index_to_string_memory.find(mem_index);

	if (reg1 != _map_string_to_index_memory.end())
		_map_string_to_index_memory.erase(reg1);
	if (reg3 != _map_index_to_string_memory.end())
		_map_index_to_string_memory.erase(reg3);
		
	if (reg2 != _map_index_to_memory.end()) {
		delete reg2->second;
		_map_index_to_memory.erase(reg2);
	}
}

/**
*/    

void Machine::Set_Up_Memory_CALCA ()
{
    _type = CALCA;
    
    Add_Instruction("NOP", NOP, new Instruction_NOP());
    
    Add_Instruction("ABS", ABS, new Instruction_ABS());
    Add_Instruction("ACOS", ACOS, new Instruction_ACOS());
    Add_Instruction("ADD", ADD, new Instruction_ADD());
    Add_Instruction("ALN", ALN, new Instruction_ALN());
    Add_Instruction("ALOG", ALOG, new Instruction_ALOG());
    Add_Instruction("ASIN", ASIN, new Instruction_ASIN());
    Add_Instruction("ATAN", ATAN, new Instruction_ATAN());
    Add_Instruction("AVE", AVE, new Instruction_AVE());
    Add_Instruction("CHS", CHS, new Instruction_CHS());
    Add_Instruction("COS", COS, new Instruction_COS());
    Add_Instruction("DEC", DEC, new Instruction_DEC());
    Add_Instruction("DIV", DIV, new Instruction_DIV());
    Add_Instruction("EXP", EXP, new Instruction_EXP());
    Add_Instruction("IDIV", IDIV, new Instruction_IDIV());
    Add_Instruction("IMOD", IMOD, new Instruction_IMOD());
    Add_Instruction("INC", INC, new Instruction_INC());
    Add_Instruction("LN", LN, new Instruction_LN());
    Add_Instruction("LOG", LOG, new Instruction_LOG());
    Add_Instruction("MAX", MAX, new Instruction_MAX());
    Add_Instruction("MAXO", MAXO, new Instruction_MAXO());
    Add_Instruction("MIN", MIN, new Instruction_MIN());
    Add_Instruction("MEDN", MEDN, new Instruction_MEDN());
    Add_Instruction("MUL", MUL, new Instruction_MUL());
    Add_Instruction("RAND", RAND, new Instruction_RAND());
    Add_Instruction("RANG", RANG, new Instruction_RANG());
    Add_Instruction("RND", RND, new Instruction_RND());
    Add_Instruction("SEED", SEED, new Instruction_SEED());
    Add_Instruction("SIN", SIN, new Instruction_SIN());
    Add_Instruction("SQR", SQR, new Instruction_SQR());
    Add_Instruction("SQRT", SQRT, new Instruction_SQRT());
    Add_Instruction("SUB", SUB, new Instruction_SUB());
    Add_Instruction("TAN", TAN, new Instruction_TAN());
    Add_Instruction("TRC", TRC, new Instruction_TRC());

    Add_Instruction("AND", AND, new Instruction_AND());
    Add_Instruction("ANDX", ANDX, new Instruction_ANDX());
    Add_Instruction("NAN", NAN, new Instruction_NAN());
    Add_Instruction("NANX", NANX, new Instruction_NANX());
    Add_Instruction("NAND", NAND, new Instruction_NAND());
    Add_Instruction("NOR", NOR, new Instruction_NOR());
    Add_Instruction("NORX", NORX, new Instruction_NORX());
    Add_Instruction("NOT", NOT, new Instruction_NOT());
    Add_Instruction("NOTX", NOTX, new Instruction_NOTX());
    Add_Instruction("NXO", NXO, new Instruction_NXO());
    Add_Instruction("NXOR", NXOR, new Instruction_NXOR());
    Add_Instruction("NXOX", NXOX, new Instruction_NXOX());
    Add_Instruction("OR", OR, new Instruction_OR());
    Add_Instruction("ORX", ORX, new Instruction_ORX());
    Add_Instruction("XOR", XOR, new Instruction_XOR());
    Add_Instruction("XORX", XORX, new Instruction_XORX());

    Add_Instruction("CBD", CBD, new Instruction_CBD());
    Add_Instruction("CE", CE, new Instruction_CE());
    Add_Instruction("COO", COO, new Instruction_COO());
    Add_Instruction("IN", IN_, new Instruction_IN());
    Add_Instruction("INB", INB, new Instruction_INB());
    Add_Instruction("INH", INH, new Instruction_INH());
    Add_Instruction("INL", INL, new Instruction_INL());
    Add_Instruction("INR", INR, new Instruction_INR());
    Add_Instruction("INS", INS, new Instruction_INS());
    Add_Instruction("OUT", OUT_, new Instruction_OUT());
    Add_Instruction("RBD", RBD, new Instruction_RBD());
    Add_Instruction("RCL", RCL, new Instruction_RCL());
    Add_Instruction("RCN", RCN, new Instruction_RCN());
    Add_Instruction("RE", RE, new Instruction_RE());
    Add_Instruction("REL", REL, new Instruction_REL());
    Add_Instruction("RON", RON, new Instruction_RON());
    Add_Instruction("ROO", ROO, new Instruction_ROO());
    Add_Instruction("RQE", RQE, new Instruction_RQE());
    Add_Instruction("RQL", RQL, new Instruction_RQL());
    Add_Instruction("SAC", SAC, new Instruction_SAC());
    Add_Instruction("SBD", SBD, new Instruction_SBD());
    Add_Instruction("SE", SE, new Instruction_SE());
    Add_Instruction("SEC", SEC, new Instruction_SEC());
    Add_Instruction("SOO", SOO, new Instruction_SOO());
    Add_Instruction("STH", STH, new Instruction_STH());
    Add_Instruction("STL", STL, new Instruction_STL());
    Add_Instruction("SWP", SWP, new Instruction_SWP());

    Add_Instruction("PRI", PRI, new Instruction_PRI());
    Add_Instruction("PRO", PRO, new Instruction_PRO());
    Add_Instruction("PRP", PRP, new Instruction_PRP());

    Add_Instruction("CLA", CLA, new Instruction_CLA());
    Add_Instruction("CLM", CLM, new Instruction_CLM());
    Add_Instruction("CST", CST, new Instruction_CST());
    Add_Instruction("DUP", DUP, new Instruction_DUP());
    Add_Instruction("LAC", LAC, new Instruction_LAC());
    Add_Instruction("LACI", LACI, new Instruction_LACI());
    Add_Instruction("POP", POP, new Instruction_POP());
    Add_Instruction("STM", STM, new Instruction_STM());
    Add_Instruction("STMI", STMI, new Instruction_STMI());
    Add_Instruction("TSTB", TSTB, new Instruction_TSTB());

    Add_Instruction("BIF", BIF, new Instruction_BIF());
    Add_Instruction("BII", BII, new Instruction_BII());
    Add_Instruction("BIN", BIN, new Instruction_BIN());
    Add_Instruction("BIP", BIP, new Instruction_BIP());
    Add_Instruction("BIT", BIT, new Instruction_BIT());
    Add_Instruction("BIZ", BIZ, new Instruction_BIZ());
    Add_Instruction("END", END, new Instruction_END());
    Add_Instruction("EXIT", EXIT, new Instruction_EXIT());
    Add_Instruction("GTI", GTI, new Instruction_GTI());
    Add_Instruction("GTO", GTO, new Instruction_GTO());

    Add_Instruction("CLL", CLL, new Instruction_CLL());
    Add_Instruction("CLR", CLR, new Instruction_CLR());
    Add_Instruction("CLRB", CLRB, new Instruction_CLRB());
    Add_Instruction("SET", SET, new Instruction_SET());
    Add_Instruction("SETB", SETB, new Instruction_SETB());
    Add_Instruction("SSF", SSF, new Instruction_SSF());
    Add_Instruction("SSI", SSI, new Instruction_SSI());
    Add_Instruction("SSN", SSN, new Instruction_SSN());
    Add_Instruction("SSP", SSP, new Instruction_SSP());
    Add_Instruction("SST", SST, new Instruction_SST());
    Add_Instruction("SSZ", SSZ, new Instruction_SSZ());

    Add_Instruction("CHI", CHI, new Instruction_CHI());
    Add_Instruction("CHN", CHN, new Instruction_CHN());
    Add_Instruction("DOFF", DOFF, new Instruction_DOFF());
    Add_Instruction("DON", DON, new Instruction_DON());
    Add_Instruction("OSP", OSP, new Instruction_OSP());
    Add_Instruction("TIM", TIM, new Instruction_TIM());

    Add_Instruction("FF", FF, new Instruction_FF());
    Add_Instruction("MRS", MRS, new Instruction_MRS());

    Add_Instruction("CLE", CLE, new Instruction_CLE());
    Add_Instruction("RER", RER, new Instruction_RER());
    Add_Instruction("SIEC", SIEC, new Instruction_SIEC()); 


    Add_Memory_Location("NAME", NAME, 	new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "");
    Add_Memory_Location("TYPE", TYPE, 	new Mem_Type(Mem_Status::CAN_CONFIG_VALUE), "");
    Add_Memory_Location("DESCRP", DESCRP, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "");
    Add_Memory_Location("PERIOD", PERIOD, new Mem_Int(Mem_Status::CAN_CONFIG_VALUE), "1");
    Add_Memory_Location("PHASE", PHASE,	new Mem_Int(Mem_Status::CAN_CONFIG_VALUE), "0");
    Add_Memory_Location("LOOPID", LOOPID, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "");

    Add_Memory_Location("BI01", BI01, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI02", BI02, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI03", BI03, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI04", BI04, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI05", BI05, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI06", BI06, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI07", BI07, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI08", BI08, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI09", BI09, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI10", BI10, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI11", BI11, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI12", BI12, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI13", BI13, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI14", BI14, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI15", BI15, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI16", BI16, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");

    Add_Memory_Location("RI01", RI01, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI02", RI02, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI03", RI03, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI04", RI04, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI05", RI05, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI06", RI06, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI07", RI07, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI08", RI08, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");

    Remove_Memory_Location("HSCI1", HSCI1);
    Remove_Memory_Location("HSCI2", HSCI2);
    Remove_Memory_Location("HSCI3", HSCI3);
    Remove_Memory_Location("HSCI4", HSCI4);
    Remove_Memory_Location("HSCI5", HSCI5);
    Remove_Memory_Location("HSCI6", HSCI6);
    Remove_Memory_Location("HSCI7", HSCI7);
    Remove_Memory_Location("HSCI8", HSCI8);

    Remove_Memory_Location("LSCI1", LSCI1);
    Remove_Memory_Location("LSCI2", LSCI2);
    Remove_Memory_Location("LSCI3", LSCI3);
    Remove_Memory_Location("LSCI4", LSCI4);
    Remove_Memory_Location("LSCI5", LSCI5);
    Remove_Memory_Location("LSCI6", LSCI6);
    Remove_Memory_Location("LSCI7", LSCI7);
    Remove_Memory_Location("LSCI8", LSCI8);

    Remove_Memory_Location("DELTI1", DELTI1);
    Remove_Memory_Location("DELTI2", DELTI2);
    Remove_Memory_Location("DELTI3", DELTI3);
    Remove_Memory_Location("DELTI4", DELTI4);
    Remove_Memory_Location("DELTI5", DELTI5);
    Remove_Memory_Location("DELTI6", DELTI6);
    Remove_Memory_Location("DELTI7", DELTI7);
    Remove_Memory_Location("DELTI8", DELTI8);

    Remove_Memory_Location("EI1", EI1);
    Remove_Memory_Location("EI2", EI2);
    Remove_Memory_Location("EI3", EI3);
    Remove_Memory_Location("EI4", EI4);
    Remove_Memory_Location("EI5", EI5);
    Remove_Memory_Location("EI6", EI6);
    Remove_Memory_Location("EI7", EI7);
    Remove_Memory_Location("EI8", EI8);


    Add_Memory_Location("II01", II01, new Mem_Int(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("II02", II02, new Mem_Int(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");

    Add_Memory_Location("LI01", LI01, new Mem_Int(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("LI02", LI02, new Mem_Int(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");

    Add_Memory_Location("BO01", BO01, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO02", BO02, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO03", BO03, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO04", BO04, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO05", BO05, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO06", BO06, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO07", BO07, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO08", BO08, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");

    Add_Memory_Location("RO01", RO01, new Mem_Float(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RO02", RO02, new Mem_Float(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RO03", RO03, new Mem_Float(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RO04", RO04, new Mem_Float(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0.0");

    Remove_Memory_Location("HSCO1", HSCO1);
    Remove_Memory_Location("HSCO2", HSCO2);
    Remove_Memory_Location("HSCO3", HSCO3);
    Remove_Memory_Location("HSCO4", HSCO4);

    Remove_Memory_Location("LSCO1", LSCO1);
    Remove_Memory_Location("LSCO2", LSCO2);
    Remove_Memory_Location("LSCO3", LSCO3);
    Remove_Memory_Location("LSCO4", LSCO4);

    Remove_Memory_Location("EO1", EO1);
    Remove_Memory_Location("EO2", EO2);
    Remove_Memory_Location("EO3", EO3);
    Remove_Memory_Location("EO4", EO4);


    Add_Memory_Location("IO01", IO01, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("IO02", IO02, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("IO03", IO03, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("IO04", IO04, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("IO05", IO05, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("IO06", IO06, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");

    Add_Memory_Location("LO01", LO01, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("LO02", LO02, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");

    Add_Memory_Location("M01", M01, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M02", M02, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M03", M03, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M04", M04, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M05", M05, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M06", M06, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M07", M07, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M08", M08, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M09", M09, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M10", M10, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M11", M11, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M12", M12, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M13", M13, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M14", M14, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M15", M15, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M16", M16, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M17", M17, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M18", M18, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M19", M19, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M20", M20, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M21", M21, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M22", M22, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M23", M23, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M24", M24, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");    
    _num_memory = 24;
    
    Add_Memory_Location("MA", MA, new Mem_Boolean(Mem_Status::CAN_CONFIG_VALUE), "1");
    Add_Memory_Location("INITMA", INITMA, new Mem_Boolean(Mem_Status::CAN_CONFIG_VALUE), "1");
    Add_Memory_Location("TIMINI", TIMINI, new Mem_Boolean(Mem_Status::CAN_CONFIG_VALUE), "1");

    Add_Memory_Location("STEP01", STEP01, new Mem_Step(), "");
    Add_Memory_Location("STEP02", STEP02, new Mem_Step(), "");
    Add_Memory_Location("STEP03", STEP03, new Mem_Step(), "");
    Add_Memory_Location("STEP04", STEP04, new Mem_Step(), "");
    Add_Memory_Location("STEP05", STEP05, new Mem_Step(), "");
    Add_Memory_Location("STEP06", STEP06, new Mem_Step(), "");
    Add_Memory_Location("STEP07", STEP07, new Mem_Step(), "");
    Add_Memory_Location("STEP08", STEP08, new Mem_Step(), "");
    Add_Memory_Location("STEP09", STEP09, new Mem_Step(), "");
    Add_Memory_Location("STEP10", STEP10, new Mem_Step(), "");
    Add_Memory_Location("STEP11", STEP11, new Mem_Step(), "");
    Add_Memory_Location("STEP12", STEP12, new Mem_Step(), "");
    Add_Memory_Location("STEP13", STEP13, new Mem_Step(), "");
    Add_Memory_Location("STEP14", STEP14, new Mem_Step(), "");
    Add_Memory_Location("STEP15", STEP15, new Mem_Step(), "");
    Add_Memory_Location("STEP16", STEP16, new Mem_Step(), "");
    Add_Memory_Location("STEP17", STEP17, new Mem_Step(), "");
    Add_Memory_Location("STEP18", STEP18, new Mem_Step(), "");
    Add_Memory_Location("STEP19", STEP19, new Mem_Step(), "");
    Add_Memory_Location("STEP20", STEP20, new Mem_Step(), "");
    Add_Memory_Location("STEP21", STEP21, new Mem_Step(), "");
    Add_Memory_Location("STEP22", STEP22, new Mem_Step(), "");
    Add_Memory_Location("STEP23", STEP23, new Mem_Step(), "");
    Add_Memory_Location("STEP24", STEP24, new Mem_Step(), "");
    Add_Memory_Location("STEP25", STEP25, new Mem_Step(), "");
    Add_Memory_Location("STEP26", STEP26, new Mem_Step(), "");
    Add_Memory_Location("STEP27", STEP27, new Mem_Step(), "");
    Add_Memory_Location("STEP28", STEP28, new Mem_Step(), "");
    Add_Memory_Location("STEP29", STEP29, new Mem_Step(), "");
    Add_Memory_Location("STEP30", STEP30, new Mem_Step(), "");
    Add_Memory_Location("STEP31", STEP31, new Mem_Step(), "");
    Add_Memory_Location("STEP32", STEP32, new Mem_Step(), "");
    Add_Memory_Location("STEP33", STEP33, new Mem_Step(), "");
    Add_Memory_Location("STEP34", STEP34, new Mem_Step(), "");
    Add_Memory_Location("STEP35", STEP35, new Mem_Step(), "");
    Add_Memory_Location("STEP36", STEP36, new Mem_Step(), "");
    Add_Memory_Location("STEP37", STEP37, new Mem_Step(), "");
    Add_Memory_Location("STEP38", STEP38, new Mem_Step(), "");
    Add_Memory_Location("STEP39", STEP39, new Mem_Step(), "");
    Add_Memory_Location("STEP40", STEP40, new Mem_Step(), "");
    Add_Memory_Location("STEP41", STEP41, new Mem_Step(), "");
    Add_Memory_Location("STEP42", STEP42, new Mem_Step(), "");
    Add_Memory_Location("STEP43", STEP43, new Mem_Step(), "");
    Add_Memory_Location("STEP44", STEP44, new Mem_Step(), "");
    Add_Memory_Location("STEP45", STEP45, new Mem_Step(), "");
    Add_Memory_Location("STEP46", STEP46, new Mem_Step(), "");
    Add_Memory_Location("STEP47", STEP47, new Mem_Step(), "");
    Add_Memory_Location("STEP48", STEP48, new Mem_Step(), "");
    Add_Memory_Location("STEP49", STEP49, new Mem_Step(), "");
    Add_Memory_Location("STEP50", STEP50, new Mem_Step(), "");
    _num_steps = 50;
    
    // Pseudoparameters
    Add_Memory_Location("I01", I01, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 31) , "0"); 
    Add_Memory_Location("I02", I02, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 30) , "0"); 
    Add_Memory_Location("I03", I03, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 29) , "0"); 
    Add_Memory_Location("I04", I04, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 28) , "0"); 
    Add_Memory_Location("I05", I05, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 27) , "0"); 
    Add_Memory_Location("I06", I06, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 26) , "0"); 
    Add_Memory_Location("I07", I07, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 25) , "0"); 
    Add_Memory_Location("I08", I08, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 24) , "0"); 
    Add_Memory_Location("I09", I09, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 23) , "0"); 
    Add_Memory_Location("I10", I10, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 22) , "0"); 
    Add_Memory_Location("I11", I11, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 21) , "0"); 
    Add_Memory_Location("I12", I12, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 20) , "0"); 
    Add_Memory_Location("I13", I13, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 19) , "0"); 
    Add_Memory_Location("I14", I14, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 18) , "0"); 
    Add_Memory_Location("I15", I15, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 17) , "0"); 
    Add_Memory_Location("I16", I16, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 16) , "0"); 
    Add_Memory_Location("I17", I17, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 15) , "0"); 
    Add_Memory_Location("I18", I18, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 14) , "0"); 
    Add_Memory_Location("I19", I19, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 13) , "0"); 
    Add_Memory_Location("I20", I20, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 12) , "0"); 
    Add_Memory_Location("I21", I21, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 11) , "0"); 
    Add_Memory_Location("I22", I22, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 10) , "0"); 
    Add_Memory_Location("I23", I23, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 9) , "0"); 
    Add_Memory_Location("I24", I24, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 8) , "0"); 
    Add_Memory_Location("I25", I25, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 7) , "0"); 
    Add_Memory_Location("I26", I26, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 6) , "0"); 
    Add_Memory_Location("I27", I27, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 5) , "0"); 
    Add_Memory_Location("I28", I28, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 4) , "0"); 
    Add_Memory_Location("I29", I29, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 3) , "0"); 
    Add_Memory_Location("I30", I30, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 2) , "0"); 
    Add_Memory_Location("I31", I31, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 1) , "0"); 
    Add_Memory_Location("I32", I32, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 0) , "0"); 

    Add_Memory_Location("O01", O01, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 31) , "0"); 
    Add_Memory_Location("O02", O02, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 30) , "0"); 
    Add_Memory_Location("O03", O03, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 29) , "0"); 
    Add_Memory_Location("O04", O04, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 28) , "0"); 
    Add_Memory_Location("O05", O05, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 27) , "0"); 
    Add_Memory_Location("O06", O06, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 26) , "0"); 
    Add_Memory_Location("O07", O07, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 25) , "0"); 
    Add_Memory_Location("O08", O08, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 24) , "0"); 
    Add_Memory_Location("O09", O09, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 23) , "0"); 
    Add_Memory_Location("O10", O10, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 22) , "0"); 
    Add_Memory_Location("O11", O11, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 21) , "0"); 
    Add_Memory_Location("O12", O12, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 20) , "0"); 
    Add_Memory_Location("O13", O13, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 19) , "0"); 
    Add_Memory_Location("O14", O14, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 18) , "0"); 
    Add_Memory_Location("O15", O15, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 17) , "0"); 
    Add_Memory_Location("O16", O16, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 16) , "0"); 
    Add_Memory_Location("O17", O17, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 15) , "0"); 
    Add_Memory_Location("O18", O18, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 14) , "0"); 
    Add_Memory_Location("O19", O19, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 13) , "0"); 
    Add_Memory_Location("O20", O20, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 12) , "0"); 
    Add_Memory_Location("O21", O21, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 11) , "0"); 
    Add_Memory_Location("O22", O22, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 10) , "0"); 
    Add_Memory_Location("O23", O23, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 9) , "0"); 
    Add_Memory_Location("O24", O24, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 8) , "0"); 
    Add_Memory_Location("O25", O25, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 7) , "0"); 
    Add_Memory_Location("O26", O26, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 6) , "0"); 
    Add_Memory_Location("O27", O27, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 5) , "0"); 
    Add_Memory_Location("O28", O28, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 4) , "0"); 
    Add_Memory_Location("O29", O29, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 3) , "0"); 
    Add_Memory_Location("O30", O30, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 2) , "0"); 
    Add_Memory_Location("O31", O31, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 1) , "0"); 
    Add_Memory_Location("O32", O32, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 0) , "0"); 

    
    // Other registers not exposed
    Add_Memory_Location("RNDSEED", RNDSEED, new Mem_Int(0), "0"); 
    Add_Memory_Location("TIME", TIME, 	new Mem_Int(0), "0"); 
    Add_Memory_Location("PERROR", PERROR, new Mem_Int(0), "0"); 


    Index_To_Memory (TYPE)->Set_Configuration("CALCA");
    //FireChanged ();
}

/**
*/    

void Machine::Set_Up_Memory_CALC() {
    _type = CALC;

    Add_Instruction("NOP", NOP, new Instruction_NOP());
    
    Add_Instruction("ABS", ABS, new Instruction_ABS());
    Add_Instruction("ACOS", ACOS, new Instruction_ACOS());
    Add_Instruction("ADD", ADD, new Instruction_ADD());
    Add_Instruction("ALN", ALN, new Instruction_ALN());
    Add_Instruction("ALOG", ALOG, new Instruction_ALOG());
    Add_Instruction("ASIN", ASIN, new Instruction_ASIN());
    Add_Instruction("ATAN", ATAN, new Instruction_ATAN());
    Add_Instruction("AVE", AVE, new Instruction_AVE());
    Add_Instruction("CHS", CHS, new Instruction_CHS());
    Add_Instruction("COS", COS, new Instruction_COS());
    Add_Instruction("DEC", DEC, new Instruction_DEC());
    Add_Instruction("DIV", DIV, new Instruction_DIV());
    Add_Instruction("EXP", EXP, new Instruction_EXP());
    Add_Instruction("IDIV", IDIV, new Instruction_IDIV());
    Add_Instruction("IMOD", IMOD, new Instruction_IMOD());
    Add_Instruction("INC", INC, new Instruction_INC());
    Add_Instruction("LN", LN, new Instruction_LN());
    Add_Instruction("LOG", LOG, new Instruction_LOG());
    Add_Instruction("MAX", MAX, new Instruction_MAX());
    Add_Instruction("MAXO", MAXO, new Instruction_MAXO());
    Add_Instruction("MIN", MIN, new Instruction_MIN());
    Add_Instruction("MEDN", MEDN, new Instruction_MEDN());
    Add_Instruction("MUL", MUL, new Instruction_MUL());
    Add_Instruction("RAND", RAND, new Instruction_RAND());
    Add_Instruction("RANG", RANG, new Instruction_RANG());
    Add_Instruction("RND", RND, new Instruction_RND());
    Add_Instruction("SEED", SEED, new Instruction_SEED());
    Add_Instruction("SIN", SIN, new Instruction_SIN());
    Add_Instruction("SQR", SQR, new Instruction_SQR());
    Add_Instruction("SQRT", SQRT, new Instruction_SQRT());
    Add_Instruction("SUB", SUB, new Instruction_SUB());
    Add_Instruction("TAN", TAN, new Instruction_TAN());
    Add_Instruction("TRC", TRC, new Instruction_TRC());

    Add_Instruction("AND", AND, new Instruction_AND());
    Add_Instruction("ANDX", ANDX, new Instruction_ANDX());
    Remove_Instruction("NAN", NAN);
    Add_Instruction("NANX", NANX, new Instruction_NANX());
    Add_Instruction("NAND", NAND, new Instruction_NAND());
    Add_Instruction("NOR", NOR, new Instruction_NOR());
    Add_Instruction("NORX", NORX, new Instruction_NORX());
    Add_Instruction("NOT", NOT, new Instruction_NOT());
    Add_Instruction("NOTX", NOTX, new Instruction_NOTX());
    Remove_Instruction("NXO", NXO);
    Add_Instruction("NXOR", NXOR, new Instruction_NXOR());
    Add_Instruction("NXOX", NXOX, new Instruction_NXOX());
    Add_Instruction("OR", OR, new Instruction_OR());
    Add_Instruction("ORX", ORX, new Instruction_ORX());
    Add_Instruction("XOR", XOR, new Instruction_XOR());
    Add_Instruction("XORX", XORX, new Instruction_XORX());

    Add_Instruction("CBD", CBD, new Instruction_CBD());
    Add_Instruction("CE", CE, new Instruction_CE());
    Add_Instruction("COO", COO, new Instruction_COO());
    Add_Instruction("IN", IN_, new Instruction_IN());
    Add_Instruction("INB", INB, new Instruction_INB());
    Add_Instruction("INH", INH, new Instruction_INH());
    Add_Instruction("INL", INL, new Instruction_INL());
    Add_Instruction("INR", INR, new Instruction_INR());
    Add_Instruction("INS", INS, new Instruction_INS());
    Add_Instruction("OUT", OUT_, new Instruction_OUT());
    Add_Instruction("RBD", RBD, new Instruction_RBD());
    Add_Instruction("RCL", RCL, new Instruction_RCL());
    Add_Instruction("RCN", RCN, new Instruction_RCN());
    Add_Instruction("RE", RE, new Instruction_RE());
    Add_Instruction("REL", REL, new Instruction_REL());
    Add_Instruction("RON", RON, new Instruction_RON());
    Add_Instruction("ROO", ROO, new Instruction_ROO());
    Remove_Instruction("RQE", RQE);
    Add_Instruction("RQL", RQL, new Instruction_RQL());
    Remove_Instruction("SAC", SAC);
    Add_Instruction("SBD", SBD, new Instruction_SBD());
    Add_Instruction("SE", SE, new Instruction_SE());
    Add_Instruction("SEC", SEC, new Instruction_SEC());
    Add_Instruction("SOO", SOO, new Instruction_SOO());
    Add_Instruction("STH", STH, new Instruction_STH());
    Add_Instruction("STL", STL, new Instruction_STL());
    Add_Instruction("SWP", SWP, new Instruction_SWP());

    Add_Instruction("PRI", PRI, new Instruction_PRI());
    Add_Instruction("PRO", PRO, new Instruction_PRO());
    Add_Instruction("PRP", PRP, new Instruction_PRP());

    Add_Instruction("CLA", CLA, new Instruction_CLA());
    Add_Instruction("CLM", CLM, new Instruction_CLM());
    Add_Instruction("CST", CST, new Instruction_CST());
    Add_Instruction("DUP", DUP, new Instruction_DUP());
    Add_Instruction("LAC", LAC, new Instruction_LAC());
    Add_Instruction("LACI", LACI, new Instruction_LACI());
    Add_Instruction("POP", POP, new Instruction_POP());
    Add_Instruction("STM", STM, new Instruction_STM());
    Add_Instruction("STMI", STMI, new Instruction_STMI());
    Remove_Instruction("TSTB", TSTB);

    Add_Instruction("BIF", BIF, new Instruction_BIF());
    Add_Instruction("BII", BII, new Instruction_BII());
    Add_Instruction("BIN", BIN, new Instruction_BIN());
    Add_Instruction("BIP", BIP, new Instruction_BIP());
    Add_Instruction("BIT", BIT, new Instruction_BIT());
    Add_Instruction("BIZ", BIZ, new Instruction_BIZ());
    Add_Instruction("END", END, new Instruction_END());
    Add_Instruction("EXIT", EXIT, new Instruction_EXIT());
    Add_Instruction("GTI", GTI, new Instruction_GTI());
    Add_Instruction("GTO", GTO, new Instruction_GTO());

    Remove_Instruction("CLL", CLL);
    Add_Instruction("CLR", CLR, new Instruction_CLR());
    Add_Instruction("CLRB", CLRB, new Instruction_CLRB());
    Add_Instruction("SET", SET, new Instruction_SET());
    Add_Instruction("SETB", SETB, new Instruction_SETB());
    Add_Instruction("SSF", SSF, new Instruction_SSF());
    Add_Instruction("SSI", SSI, new Instruction_SSI());
    Add_Instruction("SSN", SSN, new Instruction_SSN());
    Add_Instruction("SSP", SSP, new Instruction_SSP());
    Add_Instruction("SST", SST, new Instruction_SST());
    Add_Instruction("SSZ", SSZ, new Instruction_SSZ());

    Add_Instruction("CHI", CHI, new Instruction_CHI());
    Add_Instruction("CHN", CHN, new Instruction_CHN());
    Add_Instruction("DOFF", DOFF, new Instruction_DOFF());
    Add_Instruction("DON", DON, new Instruction_DON());
    Add_Instruction("OSP", OSP, new Instruction_OSP());
    Add_Instruction("TIM", TIM, new Instruction_TIM());

    Add_Instruction("FF", FF, new Instruction_FF());
    Add_Instruction("MRS", MRS, new Instruction_MRS());

    Add_Instruction("CLE", CLE, new Instruction_CLE());
    Add_Instruction("RER", RER, new Instruction_RER());
    Add_Instruction("SIEC", SIEC, new Instruction_SIEC());



    Add_Memory_Location("NAME", NAME, 	new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "");
    Add_Memory_Location("TYPE", TYPE, 	new Mem_Type(Mem_Status::CAN_CONFIG_VALUE), "");
    Add_Memory_Location("DESCRP", DESCRP, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "");
    Add_Memory_Location("PERIOD", PERIOD, new Mem_Int(Mem_Status::CAN_CONFIG_VALUE), "1");
    Add_Memory_Location("PHASE", PHASE,	new Mem_Int(Mem_Status::CAN_CONFIG_VALUE), "0");
    Add_Memory_Location("LOOPID", LOOPID, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "");

    Add_Memory_Location("BI01", BI01, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI02", BI02, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI03", BI03, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI04", BI04, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI05", BI05, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI06", BI06, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI07", BI07, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI08", BI08, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI09", BI09, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI10", BI10, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI11", BI11, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI12", BI12, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI13", BI13, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI14", BI14, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI15", BI15, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI16", BI16, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");

    Add_Memory_Location("RI01", RI01, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI02", RI02, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI03", RI03, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI04", RI04, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI05", RI05, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI06", RI06, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI07", RI07, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI08", RI08, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");

    Add_Memory_Location("HSCI1", HSCI1, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "100.0");
    Add_Memory_Location("HSCI2", HSCI2, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "100.0");
    Add_Memory_Location("HSCI3", HSCI3, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "100.0");
    Add_Memory_Location("HSCI4", HSCI4, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "100.0");
    Add_Memory_Location("HSCI5", HSCI5, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "100.0");
    Add_Memory_Location("HSCI6", HSCI6, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "100.0");
    Add_Memory_Location("HSCI7", HSCI7, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "100.0");
    Add_Memory_Location("HSCI8", HSCI8, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "100.0");

    Add_Memory_Location("LSCI1", LSCI1, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("LSCI2", LSCI2, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("LSCI3", LSCI3, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("LSCI4", LSCI4, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("LSCI5", LSCI5, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("LSCI6", LSCI6, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("LSCI7", LSCI7, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("LSCI8", LSCI8, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");

    Add_Memory_Location("DELTI1", DELTI1, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("DELTI2", DELTI2, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("DELTI3", DELTI3, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("DELTI4", DELTI4, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("DELTI5", DELTI5, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("DELTI6", DELTI6, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("DELTI7", DELTI7, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("DELTI8", DELTI8, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");

    Add_Memory_Location("EI1", EI1, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "%");
    Add_Memory_Location("EI2", EI2, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "%");
    Add_Memory_Location("EI3", EI3, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "%");
    Add_Memory_Location("EI4", EI4, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "%");
    Add_Memory_Location("EI5", EI5, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "%");
    Add_Memory_Location("EI6", EI6, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "%");
    Add_Memory_Location("EI7", EI7, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "%");
    Add_Memory_Location("EI8", EI8, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "%");


    Add_Memory_Location("II01", II01, new Mem_Int(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("II02", II02, new Mem_Int(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");

    Add_Memory_Location("LI01", LI01, new Mem_Int(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("LI02", LI02, new Mem_Int(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");

    Add_Memory_Location("BO01", BO01, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO02", BO02, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO03", BO03, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO04", BO04, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO05", BO05, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO06", BO06, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO07", BO07, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO08", BO08, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");

    Add_Memory_Location("RO01", RO01, new Mem_Float(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RO02", RO02, new Mem_Float(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RO03", RO03, new Mem_Float(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RO04", RO04, new Mem_Float(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0.0");

    Add_Memory_Location("HSCO1", HSCO1, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "100.0");
    Add_Memory_Location("HSCO2", HSCO2, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "100.0");
    Add_Memory_Location("HSCO3", HSCO3, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "100.0");
    Add_Memory_Location("HSCO4", HSCO4, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "100.0");

    Add_Memory_Location("LSCO1", LSCO1, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("LSCO2", LSCO2, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("LSCO3", LSCO3, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");
    Add_Memory_Location("LSCO4", LSCO4, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE), "0.0");

    Add_Memory_Location("EO1", EO1, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "%");
    Add_Memory_Location("EO2", EO2, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "%");
    Add_Memory_Location("EO3", EO3, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "%");
    Add_Memory_Location("EO4", EO4, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "%");


    Add_Memory_Location("IO01", IO01, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("IO02", IO02, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("IO03", IO03, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("IO04", IO04, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("IO05", IO05, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("IO06", IO06, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");

    Add_Memory_Location("LO01", LO01, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("LO02", LO02, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");

    Add_Memory_Location("M01", M01, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M02", M02, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M03", M03, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M04", M04, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M05", M05, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M06", M06, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M07", M07, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M08", M08, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M09", M09, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M10", M10, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M11", M11, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M12", M12, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M13", M13, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M14", M14, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M15", M15, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M16", M16, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M17", M17, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M18", M18, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M19", M19, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M20", M20, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M21", M21, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M22", M22, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M23", M23, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M24", M24, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");    
    _num_memory = 24;

    Add_Memory_Location("MA", MA, new Mem_Boolean(Mem_Status::CAN_CONFIG_VALUE), "1");
    Add_Memory_Location("INITMA", INITMA, new Mem_Boolean(Mem_Status::CAN_CONFIG_VALUE), "1");
    Add_Memory_Location("TIMINI", TIMINI, new Mem_Boolean(Mem_Status::CAN_CONFIG_VALUE), "1");

    Add_Memory_Location("STEP01", STEP01, new Mem_Step(), "");
    Add_Memory_Location("STEP02", STEP02, new Mem_Step(), "");
    Add_Memory_Location("STEP03", STEP03, new Mem_Step(), "");
    Add_Memory_Location("STEP04", STEP04, new Mem_Step(), "");
    Add_Memory_Location("STEP05", STEP05, new Mem_Step(), "");
    Add_Memory_Location("STEP06", STEP06, new Mem_Step(), "");
    Add_Memory_Location("STEP07", STEP07, new Mem_Step(), "");
    Add_Memory_Location("STEP08", STEP08, new Mem_Step(), "");
    Add_Memory_Location("STEP09", STEP09, new Mem_Step(), "");
    Add_Memory_Location("STEP10", STEP10, new Mem_Step(), "");
    Add_Memory_Location("STEP11", STEP11, new Mem_Step(), "");
    Add_Memory_Location("STEP12", STEP12, new Mem_Step(), "");
    Add_Memory_Location("STEP13", STEP13, new Mem_Step(), "");
    Add_Memory_Location("STEP14", STEP14, new Mem_Step(), "");
    Add_Memory_Location("STEP15", STEP15, new Mem_Step(), "");
    Add_Memory_Location("STEP16", STEP16, new Mem_Step(), "");
    Add_Memory_Location("STEP17", STEP17, new Mem_Step(), "");
    Add_Memory_Location("STEP18", STEP18, new Mem_Step(), "");
    Add_Memory_Location("STEP19", STEP19, new Mem_Step(), "");
    Add_Memory_Location("STEP20", STEP20, new Mem_Step(), "");
    Add_Memory_Location("STEP21", STEP21, new Mem_Step(), "");
    Add_Memory_Location("STEP22", STEP22, new Mem_Step(), "");
    Add_Memory_Location("STEP23", STEP23, new Mem_Step(), "");
    Add_Memory_Location("STEP24", STEP24, new Mem_Step(), "");
    Add_Memory_Location("STEP25", STEP25, new Mem_Step(), "");
    Add_Memory_Location("STEP26", STEP26, new Mem_Step(), "");
    Add_Memory_Location("STEP27", STEP27, new Mem_Step(), "");
    Add_Memory_Location("STEP28", STEP28, new Mem_Step(), "");
    Add_Memory_Location("STEP29", STEP29, new Mem_Step(), "");
    Add_Memory_Location("STEP30", STEP30, new Mem_Step(), "");
    Add_Memory_Location("STEP31", STEP31, new Mem_Step(), "");
    Add_Memory_Location("STEP32", STEP32, new Mem_Step(), "");
    Add_Memory_Location("STEP33", STEP33, new Mem_Step(), "");
    Add_Memory_Location("STEP34", STEP34, new Mem_Step(), "");
    Add_Memory_Location("STEP35", STEP35, new Mem_Step(), "");
    Add_Memory_Location("STEP36", STEP36, new Mem_Step(), "");
    Add_Memory_Location("STEP37", STEP37, new Mem_Step(), "");
    Add_Memory_Location("STEP38", STEP38, new Mem_Step(), "");
    Add_Memory_Location("STEP39", STEP39, new Mem_Step(), "");
    Add_Memory_Location("STEP40", STEP40, new Mem_Step(), "");
    Add_Memory_Location("STEP41", STEP41, new Mem_Step(), "");
    Add_Memory_Location("STEP42", STEP42, new Mem_Step(), "");
    Add_Memory_Location("STEP43", STEP43, new Mem_Step(), "");
    Add_Memory_Location("STEP44", STEP44, new Mem_Step(), "");
    Add_Memory_Location("STEP45", STEP45, new Mem_Step(), "");
    Add_Memory_Location("STEP46", STEP46, new Mem_Step(), "");
    Add_Memory_Location("STEP47", STEP47, new Mem_Step(), "");
    Add_Memory_Location("STEP48", STEP48, new Mem_Step(), "");
    Add_Memory_Location("STEP49", STEP49, new Mem_Step(), "");
    Add_Memory_Location("STEP50", STEP50, new Mem_Step(), "");
    _num_steps = 50;

    // Pseudoparameters
    Add_Memory_Location("I01", I01, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 31) , "0"); 
    Add_Memory_Location("I02", I02, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 30) , "0"); 
    Add_Memory_Location("I03", I03, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 29) , "0"); 
    Add_Memory_Location("I04", I04, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 28) , "0"); 
    Add_Memory_Location("I05", I05, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 27) , "0"); 
    Add_Memory_Location("I06", I06, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 26) , "0"); 
    Add_Memory_Location("I07", I07, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 25) , "0"); 
    Add_Memory_Location("I08", I08, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 24) , "0"); 
    Add_Memory_Location("I09", I09, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 23) , "0"); 
    Add_Memory_Location("I10", I10, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 22) , "0"); 
    Add_Memory_Location("I11", I11, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 21) , "0"); 
    Add_Memory_Location("I12", I12, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 20) , "0"); 
    Add_Memory_Location("I13", I13, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 19) , "0"); 
    Add_Memory_Location("I14", I14, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 18) , "0"); 
    Add_Memory_Location("I15", I15, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 17) , "0"); 
    Add_Memory_Location("I16", I16, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 16) , "0"); 
    Add_Memory_Location("I17", I17, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 15) , "0"); 
    Add_Memory_Location("I18", I18, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 14) , "0"); 
    Add_Memory_Location("I19", I19, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 13) , "0"); 
    Add_Memory_Location("I20", I20, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 12) , "0"); 
    Add_Memory_Location("I21", I21, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 11) , "0"); 
    Add_Memory_Location("I22", I22, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 10) , "0"); 
    Add_Memory_Location("I23", I23, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 9) , "0"); 
    Add_Memory_Location("I24", I24, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 8) , "0"); 
    Add_Memory_Location("I25", I25, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 7) , "0"); 
    Add_Memory_Location("I26", I26, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 6) , "0"); 
    Add_Memory_Location("I27", I27, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 5) , "0"); 
    Add_Memory_Location("I28", I28, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 4) , "0"); 
    Add_Memory_Location("I29", I29, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 3) , "0"); 
    Add_Memory_Location("I30", I30, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 2) , "0"); 
    Add_Memory_Location("I31", I31, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 1) , "0"); 
    Add_Memory_Location("I32", I32, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 0) , "0"); 

    Add_Memory_Location("O01", O01, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 31) , "0"); 
    Add_Memory_Location("O02", O02, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 30) , "0"); 
    Add_Memory_Location("O03", O03, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 29) , "0"); 
    Add_Memory_Location("O04", O04, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 28) , "0"); 
    Add_Memory_Location("O05", O05, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 27) , "0"); 
    Add_Memory_Location("O06", O06, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 26) , "0"); 
    Add_Memory_Location("O07", O07, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 25) , "0"); 
    Add_Memory_Location("O08", O08, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 24) , "0"); 
    Add_Memory_Location("O09", O09, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 23) , "0"); 
    Add_Memory_Location("O10", O10, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 22) , "0"); 
    Add_Memory_Location("O11", O11, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 21) , "0"); 
    Add_Memory_Location("O12", O12, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 20) , "0"); 
    Add_Memory_Location("O13", O13, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 19) , "0"); 
    Add_Memory_Location("O14", O14, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 18) , "0"); 
    Add_Memory_Location("O15", O15, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 17) , "0"); 
    Add_Memory_Location("O16", O16, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 16) , "0"); 
    Add_Memory_Location("O17", O17, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 15) , "0"); 
    Add_Memory_Location("O18", O18, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 14) , "0"); 
    Add_Memory_Location("O19", O19, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 13) , "0"); 
    Add_Memory_Location("O20", O20, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 12) , "0"); 
    Add_Memory_Location("O21", O21, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 11) , "0"); 
    Add_Memory_Location("O22", O22, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 10) , "0"); 
    Add_Memory_Location("O23", O23, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 9) , "0"); 
    Add_Memory_Location("O24", O24, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 8) , "0"); 
    Add_Memory_Location("O25", O25, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 7) , "0"); 
    Add_Memory_Location("O26", O26, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 6) , "0"); 
    Add_Memory_Location("O27", O27, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 5) , "0"); 
    Add_Memory_Location("O28", O28, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 4) , "0"); 
    Add_Memory_Location("O29", O29, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 3) , "0"); 
    Add_Memory_Location("O30", O30, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 2) , "0"); 
    Add_Memory_Location("O31", O31, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 1) , "0"); 
    Add_Memory_Location("O32", O32, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 0) , "0"); 
    
    // Other registers not exposed
    Add_Memory_Location("RNDSEED", RNDSEED, new Mem_Int(0), "0"); 
    Add_Memory_Location("TIME", TIME, 	new Mem_Int(0), "0"); 
    Add_Memory_Location("PERROR", PERROR, new Mem_Int(0), "0"); 


    Index_To_Memory (TYPE)->Set_Configuration("CALC");
    //FireChanged ();
}

/**
*/    

void Machine::Set_Up_Memory_MATH() {
    _type = MATH;

    Add_Instruction("NOP", NOP, new Instruction_NOP());
    
    Add_Instruction("ABS", ABS, new Instruction_ABS());
    Add_Instruction("ACOS", ACOS, new Instruction_ACOS());
    Add_Instruction("ADD", ADD, new Instruction_ADD());
    Add_Instruction("ALN", ALN, new Instruction_ALN());
    Add_Instruction("ALOG", ALOG, new Instruction_ALOG());
    Add_Instruction("ASIN", ASIN, new Instruction_ASIN());
    Add_Instruction("ATAN", ATAN, new Instruction_ATAN());
    Add_Instruction("AVE", AVE, new Instruction_AVE());
    Add_Instruction("CHS", CHS, new Instruction_CHS());
    Add_Instruction("COS", COS, new Instruction_COS());
    Add_Instruction("DEC", DEC, new Instruction_DEC());
    Add_Instruction("DIV", DIV, new Instruction_DIV());
    Add_Instruction("EXP", EXP, new Instruction_EXP());
    Remove_Instruction("IDIV", IDIV);
    Remove_Instruction("IMOD", IMOD);
    Add_Instruction("INC", INC, new Instruction_INC());
    Add_Instruction("LN", LN, new Instruction_LN());
    Add_Instruction("LOG", LOG, new Instruction_LOG());
    Add_Instruction("MAX", MAX, new Instruction_MAX());
    Remove_Instruction("MAXO", MAXO);
    Add_Instruction("MIN", MIN, new Instruction_MIN());
    Add_Instruction("MEDN", MEDN, new Instruction_MEDN());
    Add_Instruction("MUL", MUL, new Instruction_MUL());
    Remove_Instruction("RAND", RAND);
    Remove_Instruction("RANG", RANG);
    Remove_Instruction("RND", RND);
    Remove_Instruction("SEED", SEED);
    Add_Instruction("SIN", SIN, new Instruction_SIN());
    Add_Instruction("SQR", SQR, new Instruction_SQR());
    Add_Instruction("SQRT", SQRT, new Instruction_SQRT());
    Add_Instruction("SUB", SUB, new Instruction_SUB());
    Add_Instruction("TAN", TAN, new Instruction_TAN());
    Remove_Instruction("TRC", TRC);

    Remove_Instruction("AND", AND);
    Remove_Instruction("ANDX", ANDX);
    Remove_Instruction("NAN", NAN);
    Remove_Instruction("NANX", NANX);
    Remove_Instruction("NAND", NAND);
    Remove_Instruction("NOR", NOR);
    Remove_Instruction("NORX", NORX);
    Remove_Instruction("NOT", NOT);
    Remove_Instruction("NOTX", NOTX);
    Remove_Instruction("NXO", NXO);
    Remove_Instruction("NXOR", NXOR);
    Remove_Instruction("NXOX", NXOX);
    Remove_Instruction("OR", OR);
    Remove_Instruction("ORX", ORX);
    Remove_Instruction("XOR", XOR);
    Remove_Instruction("XORX", XORX);

    Add_Instruction("CBD", CBD, new Instruction_CBD());
    Remove_Instruction("CE", CE);
    Remove_Instruction("COO", COO);
    Add_Instruction("IN", IN_, new Instruction_IN());
    Remove_Instruction("INB", INB);
    Remove_Instruction("INH", INH);
    Remove_Instruction("INL", INL);
    Add_Instruction("INR", INR, new Instruction_INR());
    Add_Instruction("INS", INS, new Instruction_INS());
    Add_Instruction("OUT", OUT_, new Instruction_OUT());
    Add_Instruction("RBD", RBD, new Instruction_RBD());
    Add_Instruction("RCL", RCL, new Instruction_RCL());
    Remove_Instruction("RCN", RCN);
    Remove_Instruction("RE", RE);
    Remove_Instruction("REL", REL);
    Remove_Instruction("RON", RON);
    Remove_Instruction("ROO", ROO);
    Add_Instruction("RQE", RQE, new Instruction_RQE());
    Add_Instruction("RQL", RQL, new Instruction_RQL());
    Remove_Instruction("SAC", SAC);
    Add_Instruction("SBD", SBD, new Instruction_SBD());
    Remove_Instruction("SE", SE);
    Add_Instruction("SEC", SEC, new Instruction_SEC());
    Remove_Instruction("SOO", SOO);
    Remove_Instruction("STH", STH);
    Remove_Instruction("STL", STL);
    Add_Instruction("SWP", SWP, new Instruction_SWP());

    Remove_Instruction("PRI", PRI);
    Add_Instruction("PRO", PRO, new Instruction_PRO());
    Remove_Instruction("PRP", PRP);

    Add_Instruction("CLA", CLA, new Instruction_CLA());
    Add_Instruction("CLM", CLM, new Instruction_CLM());
    Add_Instruction("CST", CST, new Instruction_CST());
    Add_Instruction("DUP", DUP, new Instruction_DUP());
    Remove_Instruction("LAC", LAC);
    Add_Instruction("LACI", LACI, new Instruction_LACI());
    Add_Instruction("POP", POP, new Instruction_POP());
    Add_Instruction("STM", STM, new Instruction_STM());
    Add_Instruction("STMI", STMI, new Instruction_STMI());
    Remove_Instruction("TSTB", TSTB);

    Remove_Instruction("BIF", BIF);
    Add_Instruction("BII", BII, new Instruction_BII());
    Add_Instruction("BIN", BIN, new Instruction_BIN());
    Add_Instruction("BIP", BIP, new Instruction_BIP());
    Remove_Instruction("BIT", BIT);
    Add_Instruction("BIZ", BIZ, new Instruction_BIZ());
    Add_Instruction("END", END, new Instruction_END());
    Add_Instruction("EXIT", EXIT, new Instruction_EXIT());
    Add_Instruction("GTI", GTI, new Instruction_GTI());
    Add_Instruction("GTO", GTO, new Instruction_GTO());

    Remove_Instruction("CLL", CLL);
    Add_Instruction("CLR", CLR, new Instruction_CLR());
    Remove_Instruction("CLRB", CLRB);
    Add_Instruction("SET", SET, new Instruction_SET());
    Remove_Instruction("SETB", SETB);
    Remove_Instruction("SSF", SSF);
    Add_Instruction("SSI", SSI, new Instruction_SSI());
    Add_Instruction("SSN", SSN, new Instruction_SSN());
    Add_Instruction("SSP", SSP, new Instruction_SSP());
    Remove_Instruction("SST", SST);
    Add_Instruction("SSZ", SSZ, new Instruction_SSZ());

    Remove_Instruction("CHI", CHI);
    Remove_Instruction("CHN", CHN);
    Remove_Instruction("DOFF", DOFF);
    Remove_Instruction("DON", DON);
    Remove_Instruction("OSP", OSP);
    Remove_Instruction("TIM", TIM);

    Remove_Instruction("FF", FF);
    Remove_Instruction("MRS", MRS);

    Remove_Instruction("CLE", CLE);
    Remove_Instruction("RER", RER);
    Remove_Instruction("SIEC", SIEC);    



    Add_Memory_Location("NAME", NAME, 	new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "");
    Add_Memory_Location("TYPE", TYPE, 	new Mem_Type(Mem_Status::CAN_CONFIG_VALUE), "");
    Add_Memory_Location("DESCRP", DESCRP, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "");
    Add_Memory_Location("PERIOD", PERIOD, new Mem_Int(Mem_Status::CAN_CONFIG_VALUE), "1");
    Add_Memory_Location("PHASE", PHASE,	new Mem_Int(Mem_Status::CAN_CONFIG_VALUE), "0");
    Add_Memory_Location("LOOPID", LOOPID, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "");

    Remove_Memory_Location("BI01", BI01);
    Remove_Memory_Location("BI02", BI02);
    Remove_Memory_Location("BI03", BI03);
    Remove_Memory_Location("BI04", BI04);
    Remove_Memory_Location("BI05", BI05);
    Remove_Memory_Location("BI06", BI06);
    Remove_Memory_Location("BI07", BI07);
    Remove_Memory_Location("BI08", BI08);
    Remove_Memory_Location("BI09", BI09);
    Remove_Memory_Location("BI10", BI10);
    Remove_Memory_Location("BI11", BI11);
    Remove_Memory_Location("BI12", BI12);
    Remove_Memory_Location("BI13", BI13);
    Remove_Memory_Location("BI14", BI14);
    Remove_Memory_Location("BI15", BI15);
    Remove_Memory_Location("BI16", BI16);

    Add_Memory_Location("RI01", RI01, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI02", RI02, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI03", RI03, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI04", RI04, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI05", RI05, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI06", RI06, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI07", RI07, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI08", RI08, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");

    Remove_Memory_Location("HSCI1", HSCI1);
    Remove_Memory_Location("HSCI2", HSCI2);
    Remove_Memory_Location("HSCI3", HSCI3);
    Remove_Memory_Location("HSCI4", HSCI4);
    Remove_Memory_Location("HSCI5", HSCI5);
    Remove_Memory_Location("HSCI6", HSCI6);
    Remove_Memory_Location("HSCI7", HSCI7);
    Remove_Memory_Location("HSCI8", HSCI8);

    Remove_Memory_Location("LSCI1", LSCI1);
    Remove_Memory_Location("LSCI2", LSCI2);
    Remove_Memory_Location("LSCI3", LSCI3);
    Remove_Memory_Location("LSCI4", LSCI4);
    Remove_Memory_Location("LSCI5", LSCI5);
    Remove_Memory_Location("LSCI6", LSCI6);
    Remove_Memory_Location("LSCI7", LSCI7);
    Remove_Memory_Location("LSCI8", LSCI8);

    Remove_Memory_Location("DELTI1", DELTI1);
    Remove_Memory_Location("DELTI2", DELTI2);
    Remove_Memory_Location("DELTI3", DELTI3);
    Remove_Memory_Location("DELTI4", DELTI4);
    Remove_Memory_Location("DELTI5", DELTI5);
    Remove_Memory_Location("DELTI6", DELTI6);
    Remove_Memory_Location("DELTI7", DELTI7);
    Remove_Memory_Location("DELTI8", DELTI8);

    Remove_Memory_Location("EI1", EI1);
    Remove_Memory_Location("EI2", EI2);
    Remove_Memory_Location("EI3", EI3);
    Remove_Memory_Location("EI4", EI4);
    Remove_Memory_Location("EI5", EI5);
    Remove_Memory_Location("EI6", EI6);
    Remove_Memory_Location("EI7", EI7);
    Remove_Memory_Location("EI8", EI8);


    Remove_Memory_Location("II01", II01);
    Remove_Memory_Location("II02", II02);

    Remove_Memory_Location("LI01", LI01);
    Remove_Memory_Location("LI02", LI02);

    Remove_Memory_Location("BO01", BO01);
    Remove_Memory_Location("BO02", BO02);
    Remove_Memory_Location("BO03", BO03);
    Remove_Memory_Location("BO04", BO04);
    Remove_Memory_Location("BO05", BO05);
    Remove_Memory_Location("BO06", BO06);
    Remove_Memory_Location("BO07", BO07);
    Remove_Memory_Location("BO08", BO08);

    Add_Memory_Location("RO01", RO01, new Mem_Float(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RO02", RO02, new Mem_Float(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RO03", RO03, new Mem_Float(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RO04", RO04, new Mem_Float(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0.0");

    Remove_Memory_Location("HSCO1", HSCO1);
    Remove_Memory_Location("HSCO2", HSCO2);
    Remove_Memory_Location("HSCO3", HSCO3);
    Remove_Memory_Location("HSCO4", HSCO4);

    Remove_Memory_Location("LSCO1", LSCO1);
    Remove_Memory_Location("LSCO2", LSCO2);
    Remove_Memory_Location("LSCO3", LSCO3);
    Remove_Memory_Location("LSCO4", LSCO4);

    Remove_Memory_Location("EO1", EO1);
    Remove_Memory_Location("EO2", EO2);
    Remove_Memory_Location("EO3", EO3);
    Remove_Memory_Location("EO4", EO4);


    Remove_Memory_Location("IO01", IO01);
    Remove_Memory_Location("IO02", IO02);
    Remove_Memory_Location("IO03", IO03);
    Remove_Memory_Location("IO04", IO04);
    Remove_Memory_Location("IO05", IO05);
    Remove_Memory_Location("IO06", IO06);

    Remove_Memory_Location("LO01", LO01);
    Remove_Memory_Location("LO02", LO02);

    Add_Memory_Location("M01", M01, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M02", M02, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M03", M03, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M04", M04, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M05", M05, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Remove_Memory_Location("M06", M06);
    Remove_Memory_Location("M07", M07);
    Remove_Memory_Location("M08", M08);
    Remove_Memory_Location("M09", M09);
    Remove_Memory_Location("M10", M10);
    Remove_Memory_Location("M11", M11);
    Remove_Memory_Location("M12", M12);
    Remove_Memory_Location("M13", M13);
    Remove_Memory_Location("M14", M14);
    Remove_Memory_Location("M15", M15);
    Remove_Memory_Location("M16", M16);
    Remove_Memory_Location("M17", M17);
    Remove_Memory_Location("M18", M18);
    Remove_Memory_Location("M19", M19);
    Remove_Memory_Location("M20", M20);
    Remove_Memory_Location("M21", M21);
    Remove_Memory_Location("M22", M22);
    Remove_Memory_Location("M23", M23);
    Remove_Memory_Location("M24", M24);    
    _num_memory = 5;

    Add_Memory_Location("MA", MA, new Mem_Boolean(Mem_Status::CAN_CONFIG_VALUE), "1");
    Add_Memory_Location("INITMA", INITMA, new Mem_Boolean(Mem_Status::CAN_CONFIG_VALUE), "1");
    Remove_Memory_Location("TIMINI", TIMINI);

    Add_Memory_Location("STEP01", STEP01, new Mem_Step(), "");
    Add_Memory_Location("STEP02", STEP02, new Mem_Step(), "");
    Add_Memory_Location("STEP03", STEP03, new Mem_Step(), "");
    Add_Memory_Location("STEP04", STEP04, new Mem_Step(), "");
    Add_Memory_Location("STEP05", STEP05, new Mem_Step(), "");
    Add_Memory_Location("STEP06", STEP06, new Mem_Step(), "");
    Add_Memory_Location("STEP07", STEP07, new Mem_Step(), "");
    Add_Memory_Location("STEP08", STEP08, new Mem_Step(), "");
    Add_Memory_Location("STEP09", STEP09, new Mem_Step(), "");
    Add_Memory_Location("STEP10", STEP10, new Mem_Step(), "");
    Add_Memory_Location("STEP11", STEP11, new Mem_Step(), "");
    Add_Memory_Location("STEP12", STEP12, new Mem_Step(), "");
    Add_Memory_Location("STEP13", STEP13, new Mem_Step(), "");
    Add_Memory_Location("STEP14", STEP14, new Mem_Step(), "");
    Add_Memory_Location("STEP15", STEP15, new Mem_Step(), "");
    Add_Memory_Location("STEP16", STEP16, new Mem_Step(), "");
    Add_Memory_Location("STEP17", STEP17, new Mem_Step(), "");
    Add_Memory_Location("STEP18", STEP18, new Mem_Step(), "");
    Add_Memory_Location("STEP19", STEP19, new Mem_Step(), "");
    Add_Memory_Location("STEP20", STEP20, new Mem_Step(), "");
    Remove_Memory_Location("STEP21", STEP21);
    Remove_Memory_Location("STEP22", STEP22);
    Remove_Memory_Location("STEP23", STEP23);
    Remove_Memory_Location("STEP24", STEP24);
    Remove_Memory_Location("STEP25", STEP25);
    Remove_Memory_Location("STEP26", STEP26);
    Remove_Memory_Location("STEP27", STEP27);
    Remove_Memory_Location("STEP28", STEP28);
    Remove_Memory_Location("STEP29", STEP29);
    Remove_Memory_Location("STEP30", STEP30);
    Remove_Memory_Location("STEP31", STEP31);
    Remove_Memory_Location("STEP32", STEP32);
    Remove_Memory_Location("STEP33", STEP33);
    Remove_Memory_Location("STEP34", STEP34);
    Remove_Memory_Location("STEP35", STEP35);
    Remove_Memory_Location("STEP36", STEP36);
    Remove_Memory_Location("STEP37", STEP37);
    Remove_Memory_Location("STEP38", STEP38);
    Remove_Memory_Location("STEP39", STEP39);
    Remove_Memory_Location("STEP40", STEP40);
    Remove_Memory_Location("STEP41", STEP41);
    Remove_Memory_Location("STEP42", STEP42);
    Remove_Memory_Location("STEP43", STEP43);
    Remove_Memory_Location("STEP44", STEP44);
    Remove_Memory_Location("STEP45", STEP45);
    Remove_Memory_Location("STEP46", STEP46);
    Remove_Memory_Location("STEP47", STEP47);
    Remove_Memory_Location("STEP48", STEP48);
    Remove_Memory_Location("STEP49", STEP49);
    Remove_Memory_Location("STEP50", STEP50);
    _num_steps = 20;
    
    // Pseudoparameters
    Remove_Memory_Location("I01", I01); 
    Remove_Memory_Location("I02", I02); 
    Remove_Memory_Location("I03", I03); 
    Remove_Memory_Location("I04", I04); 
    Remove_Memory_Location("I05", I05); 
    Remove_Memory_Location("I06", I06); 
    Remove_Memory_Location("I07", I07); 
    Remove_Memory_Location("I08", I08); 
    Remove_Memory_Location("I09", I09); 
    Remove_Memory_Location("I10", I10); 
    Remove_Memory_Location("I11", I11); 
    Remove_Memory_Location("I12", I12); 
    Remove_Memory_Location("I13", I13); 
    Remove_Memory_Location("I14", I14); 
    Remove_Memory_Location("I15", I15); 
    Remove_Memory_Location("I16", I16); 
    Remove_Memory_Location("I17", I17); 
    Remove_Memory_Location("I18", I18); 
    Remove_Memory_Location("I19", I19); 
    Remove_Memory_Location("I20", I20); 
    Remove_Memory_Location("I21", I21); 
    Remove_Memory_Location("I22", I22); 
    Remove_Memory_Location("I23", I23); 
    Remove_Memory_Location("I24", I24); 
    Remove_Memory_Location("I25", I25); 
    Remove_Memory_Location("I26", I26); 
    Remove_Memory_Location("I27", I27); 
    Remove_Memory_Location("I28", I28); 
    Remove_Memory_Location("I29", I29); 
    Remove_Memory_Location("I30", I30); 
    Remove_Memory_Location("I31", I31); 
    Remove_Memory_Location("I32", I32); 

    Remove_Memory_Location("O01", O01); 
    Remove_Memory_Location("O02", O02); 
    Remove_Memory_Location("O03", O03); 
    Remove_Memory_Location("O04", O04); 
    Remove_Memory_Location("O05", O05); 
    Remove_Memory_Location("O06", O06); 
    Remove_Memory_Location("O07", O07); 
    Remove_Memory_Location("O08", O08); 
    Remove_Memory_Location("O09", O09); 
    Remove_Memory_Location("O10", O10); 
    Remove_Memory_Location("O11", O11); 
    Remove_Memory_Location("O12", O12); 
    Remove_Memory_Location("O13", O13); 
    Remove_Memory_Location("O14", O14); 
    Remove_Memory_Location("O15", O15); 
    Remove_Memory_Location("O16", O16); 
    Remove_Memory_Location("O17", O17); 
    Remove_Memory_Location("O18", O18); 
    Remove_Memory_Location("O19", O19); 
    Remove_Memory_Location("O20", O20); 
    Remove_Memory_Location("O21", O21); 
    Remove_Memory_Location("O22", O22); 
    Remove_Memory_Location("O23", O23); 
    Remove_Memory_Location("O24", O24); 
    Remove_Memory_Location("O25", O25); 
    Remove_Memory_Location("O26", O26); 
    Remove_Memory_Location("O27", O27); 
    Remove_Memory_Location("O28", O28); 
    Remove_Memory_Location("O29", O29); 
    Remove_Memory_Location("O30", O30); 
    Remove_Memory_Location("O31", O31); 
    Remove_Memory_Location("O32", O32); 

    // Other registers not exposed
    Remove_Memory_Location("RNDSEED", RNDSEED); 
    Remove_Memory_Location("TIME", TIME); 
    Add_Memory_Location("PERROR", PERROR, new Mem_Int(0), "0"); 


    Index_To_Memory (TYPE)->Set_Configuration("MATH");
    //FireChanged ();
}

/**
*/    

void Machine::Set_Up_Memory_LOGIC() {
    _type = LOGIC;

    Add_Instruction("NOP", NOP, new Instruction_NOP());
    
    Remove_Instruction("ABS", ABS);
    Remove_Instruction("ACOS", ACOS);
    Remove_Instruction("ADD", ADD);
    Remove_Instruction("ALN", ALN);
    Remove_Instruction("ALOG", ALOG);
    Remove_Instruction("ASIN", ASIN);
    Remove_Instruction("ATAN", ATAN);
    Remove_Instruction("AVE", AVE);
    Remove_Instruction("CHS", CHS);
    Remove_Instruction("COS", COS);
    Remove_Instruction("DEC", DEC);
    Remove_Instruction("DIV", DIV);
    Remove_Instruction("EXP", EXP);
    Remove_Instruction("IDIV", IDIV);
    Remove_Instruction("IMOD", IMOD);
    Remove_Instruction("INC", INC);
    Remove_Instruction("LN", LN);
    Remove_Instruction("LOG", LOG);
    Remove_Instruction("MAX", MAX);
    Remove_Instruction("MAXO", MAXO);
    Remove_Instruction("MIN", MIN);
    Remove_Instruction("MEDN", MEDN);
    Remove_Instruction("MUL", MUL);
    Remove_Instruction("RAND", RAND);
    Remove_Instruction("RANG", RANG);
    Remove_Instruction("RND", RND);
    Remove_Instruction("SEED", SEED);
    Remove_Instruction("SIN", SIN);
    Remove_Instruction("SQR", SQR);
    Remove_Instruction("SQRT", SQRT);
    Remove_Instruction("SUB", SUB);
    Remove_Instruction("TAN", TAN);
    Remove_Instruction("TRC", TRC);

    Add_Instruction("AND", AND, new Instruction_AND());
    Remove_Instruction("ANDX", ANDX);
    Remove_Instruction("NAN", NAN);
    Remove_Instruction("NANX", NANX);
    Add_Instruction("NAND", NAND, new Instruction_NAND());
    Add_Instruction("NOR", NOR, new Instruction_NOR());
    Remove_Instruction("NORX", NORX);
    Add_Instruction("NOT", NOT, new Instruction_NOT());
    Remove_Instruction("NOTX", NOTX);
    Remove_Instruction("NXO", NXO);
    Add_Instruction("NXOR", NXOR, new Instruction_NXOR());
    Remove_Instruction("NXOX", NXOX);
    Add_Instruction("OR", OR, new Instruction_OR());
    Remove_Instruction("ORX", ORX);
    Add_Instruction("XOR", XOR, new Instruction_XOR());
    Remove_Instruction("XORX", XORX);

    Add_Instruction("CBD", CBD, new Instruction_CBD());
    Remove_Instruction("CE", CE);
    Remove_Instruction("COO", COO);
    Add_Instruction("IN", IN_, new Instruction_IN());
    Add_Instruction("INB", INB, new Instruction_INB());
    Remove_Instruction("INH", INH);
    Remove_Instruction("INL", INL);
    Remove_Instruction("INR", INR);
    Remove_Instruction("INS", INS);
    Add_Instruction("OUT", OUT_, new Instruction_OUT());
    Add_Instruction("RBD", RBD, new Instruction_RBD());
    Add_Instruction("RCL", RCL, new Instruction_RCL());
    Remove_Instruction("RCN", RCN);
    Remove_Instruction("RE", RE);
    Add_Instruction("REL", REL, new Instruction_REL());
    Remove_Instruction("RON", RON);
    Remove_Instruction("ROO", ROO);
    Add_Instruction("RQE", RQE, new Instruction_RQE());
    Add_Instruction("RQL", RQL, new Instruction_RQL());
    Remove_Instruction("SAC", SAC);
    Add_Instruction("SBD", SBD, new Instruction_SBD());
    Remove_Instruction("SE", SE);
    Add_Instruction("SEC", SEC, new Instruction_SEC());
    Remove_Instruction("SOO", SOO);
    Remove_Instruction("STH", STH);
    Remove_Instruction("STL", STL);
    Add_Instruction("SWP", SWP, new Instruction_SWP());

    Remove_Instruction("PRI", PRI);
    Remove_Instruction("PRO", PRO);
    Remove_Instruction("PRP", PRP);

    Add_Instruction("CLA", CLA, new Instruction_CLA());
    Add_Instruction("CLM", CLM, new Instruction_CLM());
    Add_Instruction("CST", CST, new Instruction_CST());
    Add_Instruction("DUP", DUP, new Instruction_DUP());
    Remove_Instruction("LAC", LAC);
    Remove_Instruction("LACI", LACI);
    Add_Instruction("POP", POP, new Instruction_POP());
    Add_Instruction("STM", STM, new Instruction_STM());
    Remove_Instruction("STMI", STMI);
    Remove_Instruction("TSTB", TSTB);

    Add_Instruction("BIF", BIF, new Instruction_BIF());
    Add_Instruction("BII", BII, new Instruction_BII());
    Remove_Instruction("BIN", BIN);
    Remove_Instruction("BIP", BIP);
    Add_Instruction("BIT", BIT, new Instruction_BIT());
    Remove_Instruction("BIZ", BIZ);
    Add_Instruction("END", END, new Instruction_END());
    Add_Instruction("EXIT", EXIT, new Instruction_EXIT());
    Add_Instruction("GTI", GTI, new Instruction_GTI());
    Add_Instruction("GTO", GTO, new Instruction_GTO());

    Remove_Instruction("CLL", CLL);
    Add_Instruction("CLR", CLR, new Instruction_CLR());
    Remove_Instruction("CLRB", CLRB);
    Add_Instruction("SET", SET, new Instruction_SET());
    Remove_Instruction("SETB", SETB);
    Add_Instruction("SSF", SSF, new Instruction_SSF());
    Add_Instruction("SSI", SSI, new Instruction_SSI());
    Remove_Instruction("SSN", SSN);
    Remove_Instruction("SSP", SSP);
    Add_Instruction("SST", SST, new Instruction_SST());
    Remove_Instruction("SSZ", SSZ);

    Remove_Instruction("CHI", CHI);
    Remove_Instruction("CHN", CHN);
    Add_Instruction("DOFF", DOFF, new Instruction_DOFF());
    Add_Instruction("DON", DON, new Instruction_DON());
    Add_Instruction("OSP", OSP, new Instruction_OSP());
    Remove_Instruction("TIM", TIM);

    Add_Instruction("FF", FF, new Instruction_FF());
    Add_Instruction("MRS", MRS, new Instruction_MRS());

    Remove_Instruction("CLE", CLE);
    Remove_Instruction("RER", RER);
    Remove_Instruction("SIEC", SIEC); 



    Add_Memory_Location("NAME", NAME, 	new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "");
    Add_Memory_Location("TYPE", TYPE, 	new Mem_Type(Mem_Status::CAN_CONFIG_VALUE), "");
    Add_Memory_Location("DESCRP", DESCRP, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "");
    Add_Memory_Location("PERIOD", PERIOD, new Mem_Int(Mem_Status::CAN_CONFIG_VALUE), "1");
    Add_Memory_Location("PHASE", PHASE,	new Mem_Int(Mem_Status::CAN_CONFIG_VALUE), "0");
    Add_Memory_Location("LOOPID", LOOPID, new Mem_String(Mem_Status::CAN_CONFIG_VALUE), "");

    Add_Memory_Location("BI01", BI01, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI02", BI02, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI03", BI03, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI04", BI04, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI05", BI05, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI06", BI06, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI07", BI07, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI08", BI08, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI09", BI09, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI10", BI10, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI11", BI11, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI12", BI12, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI13", BI13, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI14", BI14, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI15", BI15, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BI16", BI16, new Mem_Boolean(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");

    Add_Memory_Location("RI01", RI01, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Add_Memory_Location("RI02", RI02, new Mem_Float(Mem_Status::CAN_CONFIG_CONNECT /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0.0");
    Remove_Memory_Location("RI03", RI03);
    Remove_Memory_Location("RI04", RI04);
    Remove_Memory_Location("RI05", RI05);
    Remove_Memory_Location("RI06", RI06);
    Remove_Memory_Location("RI07", RI07);
    Remove_Memory_Location("RI08", RI08);

    Remove_Memory_Location("HSCI1", HSCI1);
    Remove_Memory_Location("HSCI2", HSCI2);
    Remove_Memory_Location("HSCI3", HSCI3);
    Remove_Memory_Location("HSCI4", HSCI4);
    Remove_Memory_Location("HSCI5", HSCI5);
    Remove_Memory_Location("HSCI6", HSCI6);
    Remove_Memory_Location("HSCI7", HSCI7);
    Remove_Memory_Location("HSCI8", HSCI8);

    Remove_Memory_Location("LSCI1", LSCI1);
    Remove_Memory_Location("LSCI2", LSCI2);
    Remove_Memory_Location("LSCI3", LSCI3);
    Remove_Memory_Location("LSCI4", LSCI4);
    Remove_Memory_Location("LSCI5", LSCI5);
    Remove_Memory_Location("LSCI6", LSCI6);
    Remove_Memory_Location("LSCI7", LSCI7);
    Remove_Memory_Location("LSCI8", LSCI8);

    Remove_Memory_Location("DELTI1", DELTI1);
    Remove_Memory_Location("DELTI2", DELTI2);
    Remove_Memory_Location("DELTI3", DELTI3);
    Remove_Memory_Location("DELTI4", DELTI4);
    Remove_Memory_Location("DELTI5", DELTI5);
    Remove_Memory_Location("DELTI6", DELTI6);
    Remove_Memory_Location("DELTI7", DELTI7);
    Remove_Memory_Location("DELTI8", DELTI8);

    Remove_Memory_Location("EI1", EI1);
    Remove_Memory_Location("EI2", EI2);
    Remove_Memory_Location("EI3", EI3);
    Remove_Memory_Location("EI4", EI4);
    Remove_Memory_Location("EI5", EI5);
    Remove_Memory_Location("EI6", EI6);
    Remove_Memory_Location("EI7", EI7);
    Remove_Memory_Location("EI8", EI8);


    Remove_Memory_Location("II01", II01);
    Remove_Memory_Location("II02", II02);

    Add_Memory_Location("LI01", LI01, new Mem_Int(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */ | Mem_Status::CAN_CHANGE_STATUS), "0");
    Remove_Memory_Location("LI02", LI02);

    Add_Memory_Location("BO01", BO01, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO02", BO02, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO03", BO03, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Add_Memory_Location("BO04", BO04, new Mem_Boolean(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Remove_Memory_Location("BO05", BO05);
    Remove_Memory_Location("BO06", BO06);
    Remove_Memory_Location("BO07", BO07);
    Remove_Memory_Location("BO08", BO08);

    Remove_Memory_Location("RO01", RO01);
    Remove_Memory_Location("RO02", RO02);
    Remove_Memory_Location("RO03", RO03);
    Remove_Memory_Location("RO04", RO04);

    Remove_Memory_Location("HSCO1", HSCO1);
    Remove_Memory_Location("HSCO2", HSCO2);
    Remove_Memory_Location("HSCO3", HSCO3);
    Remove_Memory_Location("HSCO4", HSCO4);

    Remove_Memory_Location("LSCO1", LSCO1);
    Remove_Memory_Location("LSCO2", LSCO2);
    Remove_Memory_Location("LSCO3", LSCO3);
    Remove_Memory_Location("LSCO4", LSCO4);

    Remove_Memory_Location("EO1", EO1);
    Remove_Memory_Location("EO2", EO2);
    Remove_Memory_Location("EO3", EO3);
    Remove_Memory_Location("EO4", EO4);


    Remove_Memory_Location("IO01", IO01);
    Remove_Memory_Location("IO02", IO02);
    Remove_Memory_Location("IO03", IO03);
    Remove_Memory_Location("IO04", IO04);
    Remove_Memory_Location("IO05", IO05);
    Remove_Memory_Location("IO06", IO06);

    Add_Memory_Location("LO01", LO01, new Mem_Int(/*Mem_Status::CAN_ONLINE |*/ Mem_Status::CAN_CHANGE_STATUS), "0");
    Remove_Memory_Location("LO02", LO02);

    Add_Memory_Location("M01", M01, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M02", M02, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M03", M03, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M04", M04, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Add_Memory_Location("M05", M05, new Mem_Float(Mem_Status::CAN_CONFIG_VALUE /* | Mem_Status::CAN_ONLINE */), "0.0");
    Remove_Memory_Location("M06", M06);
    Remove_Memory_Location("M07", M07);
    Remove_Memory_Location("M08", M08);
    Remove_Memory_Location("M09", M09);
    Remove_Memory_Location("M10", M10);
    Remove_Memory_Location("M11", M11);
    Remove_Memory_Location("M12", M12);
    Remove_Memory_Location("M13", M13);
    Remove_Memory_Location("M14", M14);
    Remove_Memory_Location("M15", M15);
    Remove_Memory_Location("M16", M16);
    Remove_Memory_Location("M17", M17);
    Remove_Memory_Location("M18", M18);
    Remove_Memory_Location("M19", M19);
    Remove_Memory_Location("M20", M20);
    Remove_Memory_Location("M21", M21);
    Remove_Memory_Location("M22", M22);
    Remove_Memory_Location("M23", M23);
    Remove_Memory_Location("M24", M24);    
    _num_memory = 5;

    Add_Memory_Location("MA", MA, new Mem_Boolean(Mem_Status::CAN_CONFIG_VALUE), "1");
    Add_Memory_Location("INITMA", INITMA, new Mem_Boolean(Mem_Status::CAN_CONFIG_VALUE), "1");
    Add_Memory_Location("TIMINI", TIMINI, new Mem_Boolean(Mem_Status::CAN_CONFIG_VALUE), "1");

    Add_Memory_Location("STEP01", STEP01, new Mem_Step(), "");
    Add_Memory_Location("STEP02", STEP02, new Mem_Step(), "");
    Add_Memory_Location("STEP03", STEP03, new Mem_Step(), "");
    Add_Memory_Location("STEP04", STEP04, new Mem_Step(), "");
    Add_Memory_Location("STEP05", STEP05, new Mem_Step(), "");
    Add_Memory_Location("STEP06", STEP06, new Mem_Step(), "");
    Add_Memory_Location("STEP07", STEP07, new Mem_Step(), "");
    Add_Memory_Location("STEP08", STEP08, new Mem_Step(), "");
    Add_Memory_Location("STEP09", STEP09, new Mem_Step(), "");
    Add_Memory_Location("STEP10", STEP10, new Mem_Step(), "");
    Add_Memory_Location("STEP11", STEP11, new Mem_Step(), "");
    Add_Memory_Location("STEP12", STEP12, new Mem_Step(), "");
    Add_Memory_Location("STEP13", STEP13, new Mem_Step(), "");
    Add_Memory_Location("STEP14", STEP14, new Mem_Step(), "");
    Add_Memory_Location("STEP15", STEP15, new Mem_Step(), "");
    Remove_Memory_Location("STEP16", STEP16);
    Remove_Memory_Location("STEP17", STEP17);
    Remove_Memory_Location("STEP18", STEP18);
    Remove_Memory_Location("STEP19", STEP19);
    Remove_Memory_Location("STEP20", STEP20);
    Remove_Memory_Location("STEP21", STEP21);
    Remove_Memory_Location("STEP22", STEP22);
    Remove_Memory_Location("STEP23", STEP23);
    Remove_Memory_Location("STEP24", STEP24);
    Remove_Memory_Location("STEP25", STEP25);
    Remove_Memory_Location("STEP26", STEP26);
    Remove_Memory_Location("STEP27", STEP27);
    Remove_Memory_Location("STEP28", STEP28);
    Remove_Memory_Location("STEP29", STEP29);
    Remove_Memory_Location("STEP30", STEP30);
    Remove_Memory_Location("STEP31", STEP31);
    Remove_Memory_Location("STEP32", STEP32);
    Remove_Memory_Location("STEP33", STEP33);
    Remove_Memory_Location("STEP34", STEP34);
    Remove_Memory_Location("STEP35", STEP35);
    Remove_Memory_Location("STEP36", STEP36);
    Remove_Memory_Location("STEP37", STEP37);
    Remove_Memory_Location("STEP38", STEP38);
    Remove_Memory_Location("STEP39", STEP39);
    Remove_Memory_Location("STEP40", STEP40);
    Remove_Memory_Location("STEP41", STEP41);
    Remove_Memory_Location("STEP42", STEP42);
    Remove_Memory_Location("STEP43", STEP43);
    Remove_Memory_Location("STEP44", STEP44);
    Remove_Memory_Location("STEP45", STEP45);
    Remove_Memory_Location("STEP46", STEP46);
    Remove_Memory_Location("STEP47", STEP47);
    Remove_Memory_Location("STEP48", STEP48);
    Remove_Memory_Location("STEP49", STEP49);
    Remove_Memory_Location("STEP50", STEP50);
    _num_steps = 15;

    // Pseudoparameters
    Add_Memory_Location("I01", I01, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 31) , "0"); 
    Add_Memory_Location("I02", I02, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 30) , "0"); 
    Add_Memory_Location("I03", I03, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 29) , "0"); 
    Add_Memory_Location("I04", I04, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 28) , "0"); 
    Add_Memory_Location("I05", I05, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 27) , "0"); 
    Add_Memory_Location("I06", I06, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 26) , "0"); 
    Add_Memory_Location("I07", I07, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 25) , "0"); 
    Add_Memory_Location("I08", I08, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 24) , "0"); 
    Add_Memory_Location("I09", I09, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 23) , "0"); 
    Add_Memory_Location("I10", I10, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 22) , "0"); 
    Add_Memory_Location("I11", I11, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 21) , "0"); 
    Add_Memory_Location("I12", I12, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 20) , "0"); 
    Add_Memory_Location("I13", I13, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 19) , "0"); 
    Add_Memory_Location("I14", I14, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 18) , "0"); 
    Add_Memory_Location("I15", I15, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 17) , "0"); 
    Add_Memory_Location("I16", I16, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 16) , "0"); 
    Add_Memory_Location("I17", I17, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 15) , "0"); 
    Add_Memory_Location("I18", I18, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 14) , "0"); 
    Add_Memory_Location("I19", I19, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 13) , "0"); 
    Add_Memory_Location("I20", I20, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 12) , "0"); 
    Add_Memory_Location("I21", I21, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 11) , "0"); 
    Add_Memory_Location("I22", I22, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 10) , "0"); 
    Add_Memory_Location("I23", I23, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 9) , "0"); 
    Add_Memory_Location("I24", I24, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 8) , "0"); 
    Add_Memory_Location("I25", I25, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 7) , "0"); 
    Add_Memory_Location("I26", I26, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 6) , "0"); 
    Add_Memory_Location("I27", I27, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 5) , "0"); 
    Add_Memory_Location("I28", I28, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 4) , "0"); 
    Add_Memory_Location("I29", I29, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 3) , "0"); 
    Add_Memory_Location("I30", I30, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 2) , "0"); 
    Add_Memory_Location("I31", I31, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 1) , "0"); 
    Add_Memory_Location("I32", I32, new Mem_Bit_Reference(0, Index_To_Memory(LI01), 0) , "0"); 

    Add_Memory_Location("O01", O01, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 31) , "0"); 
    Add_Memory_Location("O02", O02, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 30) , "0"); 
    Add_Memory_Location("O03", O03, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 29) , "0"); 
    Add_Memory_Location("O04", O04, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 28) , "0"); 
    Add_Memory_Location("O05", O05, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 27) , "0"); 
    Add_Memory_Location("O06", O06, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 26) , "0"); 
    Add_Memory_Location("O07", O07, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 25) , "0"); 
    Add_Memory_Location("O08", O08, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 24) , "0"); 
    Add_Memory_Location("O09", O09, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 23) , "0"); 
    Add_Memory_Location("O10", O10, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 22) , "0"); 
    Add_Memory_Location("O11", O11, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 21) , "0"); 
    Add_Memory_Location("O12", O12, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 20) , "0"); 
    Add_Memory_Location("O13", O13, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 19) , "0"); 
    Add_Memory_Location("O14", O14, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 18) , "0"); 
    Add_Memory_Location("O15", O15, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 17) , "0"); 
    Add_Memory_Location("O16", O16, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 16) , "0"); 
    Add_Memory_Location("O17", O17, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 15) , "0"); 
    Add_Memory_Location("O18", O18, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 14) , "0"); 
    Add_Memory_Location("O19", O19, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 13) , "0"); 
    Add_Memory_Location("O20", O20, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 12) , "0"); 
    Add_Memory_Location("O21", O21, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 11) , "0"); 
    Add_Memory_Location("O22", O22, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 10) , "0"); 
    Add_Memory_Location("O23", O23, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 9) , "0"); 
    Add_Memory_Location("O24", O24, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 8) , "0"); 
    Add_Memory_Location("O25", O25, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 7) , "0"); 
    Add_Memory_Location("O26", O26, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 6) , "0"); 
    Add_Memory_Location("O27", O27, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 5) , "0"); 
    Add_Memory_Location("O28", O28, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 4) , "0"); 
    Add_Memory_Location("O29", O29, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 3) , "0"); 
    Add_Memory_Location("O30", O30, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 2) , "0"); 
    Add_Memory_Location("O31", O31, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 1) , "0"); 
    Add_Memory_Location("O32", O32, new Mem_Bit_Reference(0, Index_To_Memory(LO01), 0) , "0"); 
    
    // Other registers not exposed
    Add_Memory_Location("RNDSEED", RNDSEED, new Mem_Int(0), "0"); 
    Add_Memory_Location("TIME", TIME, 	new Mem_Int(0), "0"); 
    Add_Memory_Location("PERROR", PERROR, new Mem_Int(0), "0"); 


    Index_To_Memory (TYPE)->Set_Configuration("LOGIC");
    //FireChanged ();
}

