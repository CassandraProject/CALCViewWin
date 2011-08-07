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

#pragma once

// View update parameters
const int	UPDATE_REGISTER_TABLE_SIZE = 1;
const int	UPDATE_STACK_TABLE_SIZE = 2;
const int	UPDATE_STEPS_TABLE_SIZE = 3;

const int	UPDATE_REGISTER_TABLE_CONTENTS = 4;
const int	UPDATE_STACK_TABLE_CONTENTS = 5;
const int	UPDATE_STEPS_TABLE_CONTENTS = 6;
const int	UPDATE_ALL_TABLE_CONTENTS = 7;

// Block limits
const int	NUM_STACK_SIZE = 25;
const int	NUM_INSTRUCTIONS = 122;
const int	NUM_REGISTERS = 242;

const int	PARAM_EMPTY = -1;
const int	PARAM_NUMBER = -2;
const int	PARAM_ERROR = -3;


// Other registers
const int	NAME = 0;
const int	TYPE = 1;
const int	DESCRP = 2;
const int	PERIOD = 3;
const int	PHASE = 4;
const int	LOOPID = 5;

// I/O Parameters
const int	RI01 = 6;
const int	RI02 = 7;
const int	RI03 = 8;
const int	RI04 = 9;
const int	RI05 = 10;
const int	RI06 = 11;
const int	RI07 = 12;
const int	RI08 = 13;

const int	HSCI1 = 14;
const int	HSCI2 = 15;
const int	HSCI3 = 16;
const int	HSCI4 = 17;
const int	HSCI5 = 18;
const int	HSCI6 = 19;
const int	HSCI7 = 20;
const int	HSCI8 = 21;

const int	LSCI1 = 22;
const int	LSCI2 = 23;
const int	LSCI3 = 24;
const int	LSCI4 = 25;
const int	LSCI5 = 26;
const int	LSCI6 = 27;
const int	LSCI7 = 28;
const int	LSCI8 = 29;

const int	DELTI1 = 30;
const int	DELTI2 = 31;
const int	DELTI3 = 32;
const int	DELTI4 = 33;
const int	DELTI5 = 34;
const int	DELTI6 = 35;
const int	DELTI7 = 36;
const int	DELTI8 = 37;

const int	EI1 = 38;
const int	EI2 = 39;
const int	EI3 = 40;
const int	EI4 = 41;
const int	EI5 = 42;
const int	EI6 = 43;
const int	EI7 = 44;
const int	EI8 = 45;

const int	BI01 = 46;
const int	BI02 = 47;
const int	BI03 = 48;
const int	BI04 = 49;
const int	BI05 = 50;
const int	BI06 = 51;
const int	BI07 = 52;
const int	BI08 = 53;
const int	BI09 = 54;
const int	BI10 = 55;
const int	BI11 = 56;
const int	BI12 = 57;
const int	BI13 = 58;
const int	BI14 = 59;
const int	BI15 = 60;
const int	BI16 = 61;

const int	II01 = 62;
const int	II02 = 63;

const int	LI01 = 64;
const int	LI02 = 65;

const int	BO01 = 66;
const int	BO02 = 67;
const int	BO03 = 68;
const int	BO04 = 69;
const int	BO05 = 70;
const int	BO06 = 71;
const int	BO07 = 72;
const int	BO08 = 73;

const int	RO01 = 74;
const int	RO02 = 75;
const int	RO03 = 76;
const int	RO04 = 77;

const int	HSCO1 = 78;
const int	HSCO2 = 79;
const int	HSCO3 = 80;
const int	HSCO4 = 81;

const int	LSCO1 = 82;
const int	LSCO2 = 83;
const int	LSCO3 = 84;
const int	LSCO4 = 85;

const int	EO1 = 86;
const int	EO2 = 87;
const int	EO3 = 88;
const int	EO4 = 89;

const int	IO01 = 90;
const int	IO02 = 91;
const int	IO03 = 92;
const int	IO04 = 93;
const int	IO05 = 94;
const int	IO06 = 95;

const int	LO01 = 96;
const int	LO02 = 97;

const int	MA = 98;
const int	INITMA = 99;
const int	TIMINI = 100;

const int	M01 = 101;
const int	M02 = 102;
const int	M03 = 103;
const int	M04 = 104;
const int	M05 = 105;
const int	M06 = 106;
const int	M07 = 107;
const int	M08 = 108;
const int	M09 = 109;
const int	M10 = 110;
const int	M11 = 111;
const int	M12 = 112;
const int	M13 = 113;
const int	M14 = 114;
const int	M15 = 115;
const int	M16 = 116;
const int	M17 = 117;
const int	M18 = 118;
const int	M19 = 119;
const int	M20 = 120;
const int	M21 = 121;
const int	M22 = 122;
const int	M23 = 123;
const int	M24 = 124;

const int	STEP01 = 125;
const int	STEP02 = 126;
const int	STEP03 = 127;
const int	STEP04 = 128;
const int	STEP05 = 129;
const int	STEP06 = 130;
const int	STEP07 = 131;
const int	STEP08 = 132;
const int	STEP09 = 133;
const int	STEP10 = 134;
const int	STEP11 = 135;
const int	STEP12 = 136;
const int	STEP13 = 137;
const int	STEP14 = 138;
const int	STEP15 = 139;
const int	STEP16 = 140;
const int	STEP17 = 141;
const int	STEP18 = 142;
const int	STEP19 = 143;
const int	STEP20 = 144;
const int	STEP21 = 145;
const int	STEP22 = 146;
const int	STEP23 = 147;
const int	STEP24 = 148;
const int	STEP25 = 149;
const int	STEP26 = 150;
const int	STEP27 = 151;
const int	STEP28 = 152;
const int	STEP29 = 153;
const int	STEP30 = 154;
const int	STEP31 = 155;
const int	STEP32 = 156;
const int	STEP33 = 157;
const int	STEP34 = 158;
const int	STEP35 = 159;
const int	STEP36 = 160;
const int	STEP37 = 161;
const int	STEP38 = 162;
const int	STEP39 = 163;
const int	STEP40 = 164;
const int	STEP41 = 165;
const int	STEP42 = 166;
const int	STEP43 = 167;
const int	STEP44 = 168;
const int	STEP45 = 169;
const int	STEP46 = 170;
const int	STEP47 = 171;
const int	STEP48 = 172;
const int	STEP49 = 173;
const int	STEP50 = 174;

// Pseudoparameters
const int	I01 = 175;
const int	I02 = 176;
const int	I03 = 177;
const int	I04 = 178;
const int	I05 = 179;
const int	I06 = 180;
const int	I07 = 181;
const int	I08 = 182;
const int	I09 = 183;
const int	I10 = 184;
const int	I11 = 185;
const int	I12 = 186;
const int	I13 = 187;
const int	I14 = 188;
const int	I15 = 189;
const int	I16 = 190;
const int	I17 = 191;
const int	I18 = 192;
const int	I19 = 193;
const int	I20 = 194;
const int	I21 = 195;
const int	I22 = 196;
const int	I23 = 197;
const int	I24 = 198;
const int	I25 = 199;
const int	I26 = 200;
const int	I27 = 201;
const int	I28 = 202;
const int	I29 = 203;
const int	I30 = 204;
const int	I31 = 205;
const int	I32 = 206;

const int	O01 = 207;
const int	O02 = 208;
const int	O03 = 209;
const int	O04 = 210;
const int	O05 = 211;
const int	O06 = 212;
const int	O07 = 213;
const int	O08 = 214;
const int	O09 = 215;
const int	O10 = 216;
const int	O11 = 217;
const int	O12 = 218;
const int	O13 = 219;
const int	O14 = 220;
const int	O15 = 221;
const int	O16 = 222;
const int	O17 = 223;
const int	O18 = 224;
const int	O19 = 225;
const int	O20 = 226;
const int	O21 = 227;
const int	O22 = 228;
const int	O23 = 229;
const int	O24 = 230;
const int	O25 = 231;
const int	O26 = 232;
const int	O27 = 233;
const int	O28 = 234;
const int	O29 = 235;
const int	O30 = 236;
const int	O31 = 237;
const int	O32 = 238;

// Other registers not exposed
const int	RNDSEED = 239;
const int	TIME = 240;
const int	PERROR = 241;



bool Is_Boolean_Input 		(int r);
bool Is_Pseudo_Input 		(int r);
bool Is_Real_Input 			(int r);
bool Is_Integer_Input 		(int r);
bool Is_Long_Integer_Input	(int r);

bool Is_Boolean_Output 		(int r);
bool Is_Pseudo_Output 		(int r);
bool Is_Real_Output 		(int r);
bool Is_Integer_Output 		(int r);
bool Is_Long_Integer_Output	(int r);

bool Is_Memory 				(int r);




// Instruction Opcodes - No Op
const int	NOP = 0;

// Instruction Opcodes - Arithmetic
const int	ABS = 1;
const int	ACOS = 2;
const int	ADD = 3;
const int	ALN = 4;
const int	ALOG = 5;
const int	ASIN = 6;
const int	ATAN = 7;
const int	AVE = 8;
const int	CHS = 9;
const int	COS = 10;
const int	DEC = 11;
const int	DIV = 12;
const int	EXP = 13;
const int	IDIV = 14;
const int	IMOD = 15;
const int	INC = 16;
const int	LN = 17;
const int	LOG = 18;
const int	MAX = 19;
const int	MAXO = 20;
const int	MIN = 21;
const int	MEDN = 22;
const int	MUL = 23;
const int	RAND = 24;
const int	RANG = 25;
const int	RND = 26;
const int	SEED = 27;
const int	SIN = 28;
const int	SQR = 29;
const int	SQRT = 30;
const int	SUB = 31;
const int	TAN = 32;
const int	TRC = 33;

// Instruction Opcodes - bool
const int	AND = 34;
const int	ANDX = 35;
const int	NAN = 36;
const int	NANX = 121;
const int	NAND = 37;
const int	NOR = 38;
const int	NORX = 39;
const int	NOT = 40;
const int	NOTX = 41;
const int	NXO = 42;
const int	NXOR = 43;
const int	NXOX = 44;
const int	OR = 45;
const int	ORX = 46;
const int	XOR = 47;
const int	XORX = 48;

// Instruction Opcodes - I/O
const int	CBD = 49;
const int	CE = 50;
const int	COO = 51;
const int	IN_ = 52;
const int	INB = 53;
const int	INH = 54;
const int	INL = 55;
const int	INR = 56;
const int	INS = 57;
const int	OUT_ = 58;
const int	RBD = 59;
const int	RCL = 60;
const int	RCN = 61;
const int	RE = 62;
const int	REL = 63;
const int	RON = 64;
const int	ROO = 65;
const int	RQE = 66;
const int	RQL = 67;
const int	SAC = 68;
const int	SBD = 69;
const int	SE = 70;
const int	SEC = 71;
const int	SOO = 72;
const int	STH = 73;
const int	STL = 74;
const int	SWP = 75;

// Instruction Opcodes - Cascade Propagation
const int	PRI = 76;
const int	PRO = 77;
const int	PRP = 78;

// Instruction Opcodes - Machine and Stack
const int	CLA = 79;
const int	CLM = 80;
const int	CST = 81;
const int	DUP = 82;
const int	LAC = 83;
const int	LACI = 84;
const int	POP = 85;
const int	STM = 86;
const int	STMI = 87;
const int	TSTB = 88;

// Instruction Opcodes - Flow control
const int	BIF = 89;
const int	BII = 90;
const int	BIN = 91;
const int	BIP = 92;
const int	BIT = 93;
const int	BIZ = 94;
const int	END = 95;
const int	EXIT = 96;
const int	GTI = 97;
const int	GTO = 98;

// Instruction Opcodes - Clear/Set
const int	CLL = 99;
const int	CLR = 100;
const int	CLRB = 101;
const int	SET = 102;
const int	SETB = 103;
const int	SSF = 104;
const int	SSI = 105;
const int	SSN = 106;
const int	SSP = 107;
const int	SST = 108;
const int	SSZ = 109;

// Instruction Opcodes - Timing
const int	CHI = 110;
const int	CHN = 111;
const int	DOFF = 112;
const int	DON = 113;
const int	OSP = 114;
const int	TIM = 115;

// Instruction Opcodes - Logic
const int	FF = 116;
const int	MRS = 117;

// Instruction Opcodes - Error Control
const int	CLE = 118;
const int	RER = 119;
const int	SIEC = 120;
    
