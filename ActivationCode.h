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

#include <time.h>
#include <string>
#include "StringUtils.h"
#include "Exception_Invalid_Code.h"

const std::string	cypher1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
const std::string	cypher2 = "JSW9KLXDEFGATUV012Z834HIYMQR67BCNOP5";
const char			date_format[] = "%m%d%y%H%M%S";

class ActivationCode
{    
	public:
	
		//final private std::string _cypher1 = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
		//final private std::string _cypher2 = "JSW9KLXDEFGATUV012Z834HIYMQR67BCNOP5";
		//final private std::string _date_format = "MMddyyHHmmss";
	    
		std::string DateToString (tm &date) {
			char datestr[13];
			
			int month = date.tm_mon;
			int day = date.tm_mday;
			int year = date.tm_year % 100;	// Keep 2 digits
			int hour = date.tm_hour;
			int minute = date.tm_min;
			int second = date.tm_sec;
			
			sprintf(datestr, "%2d%2d%2d%2d%2d%2d", month, day, year, hour, minute, second );
			return std::string(datestr);
		}

		tm StringToDate (std::string datestr) {
			int month, day, year, hour, minute, second;
			tm date;
			
			sscanf(datestr.c_str(), "%2d%2d%2d%2d%2d%2d", &month, &day, &year, &hour, &minute, &second);
			
			date.tm_mon = month;
			date.tm_mday = day;
			date.tm_year = year + 2000 - 1900;
			date.tm_hour = hour;
			date.tm_min = minute;
			date.tm_sec = second;
			date.tm_isdst = -1;
		
			if (mktime(&date) == -1)
				throw Exception_Invalid_Code();
			
			return date;
	        
		}
	    
		std::string Permute (std::string str, std::string src_key, std::string dest_key) {
			std::string result;
	            
			for (int i = 0; i < str.size(); ++i) {
				int index = src_key.find_first_of(str[i]);
				if (index == std::string::npos)
					throw Exception_Invalid_Code();
				
				result = result + dest_key[index];
			}
	        
			return result;
		}
	    
		std::string AddStrings (std::string a, std::string b) {
			std::string result;
	        
			for (int i = 0; i < a.size(); ++i) {
				int an = (int) cypher1.find_first_of(a[i]);
				int bn = (int) cypher1.find_first_of(b[i]);
				
				if (an == std::string::npos || bn == std::string::npos)
					throw Exception_Invalid_Code();
	            
				result = result + cypher1[(an + bn) % cypher1.size()];
			}
	        
			return result;
		}

		std::string SubStrings (std::string a, std::string b) {
			std::string result;
	        
			for (int i = 0; i < a.size(); ++i) {
				int an = (int) cypher1.find_first_of(a[i]);
				int bn = (int) cypher1.find_first_of(b[i]);

				if (an == std::string::npos || bn == std::string::npos)
					throw Exception_Invalid_Code();
	            
				int index = (an - bn) % (int) cypher1.size();
				if (index < 0) index = cypher1.size() + index;
	            
				result = result + cypher1[index];
			}
	        
			return result;
			
		}

		std::string BuildName (std::string name) {
			try {
				std::string name_upper = ToUppercase(name);
				std::string working;
	        
				for (int i = 0; i < name_upper.size(); ++i) {
					int index = cypher1.find_first_of(name_upper[i]);
					
					if (index >= 0)
						working = working + name_upper[i];
				}
	            
				if (working.size() <= 0)
					throw new Exception_Invalid_Code();
	    
				while (working.size() < 12) 
					working = working + working;
	                
				return working.substr(0,12);
			}
			catch (...) {
				throw new Exception_Invalid_Code();
			}
		}
	    
		int Generate_Checksum (std::string s) {
			int checksum = 0;
	    
			for (int i = 0; i < s.size() - 1; ++i) {
				int c1 = ((int) s[i]) & 0x000000FF;
				int c2 = ((int) s[i+1]) & 0x000000FF;
	            
				checksum = checksum ^ (c1 | (c2 << 8));
			}
	        
			return checksum;
		}
	    
		/*std::string Generate_Code (std::string name, tm &expires) {
			try {
				std::string namestring = BuildName(name);
				if (namestring == null)
					throw new Exception_Invalid_Code();
	    
				std::string datestd::string = DateTostd::string (expires);
	            
				std::string codestring = Permute (Addstd::strings (datestd::string, namestring), _cypher2, _cypher1);
	    
				// Assemble checksums onto std::string
				int checksum = Generate_Checksum (codestring);
				char c1 = _cypher1.charAt((0x000000FF & checksum) % _cypher1.length());
				char c2 = _cypher1.charAt((0x000000FF & (checksum >> 8)) % _cypher1.length());
	            
				codestring = c1 + codestring + c2;
	            
				return codestring;
			}
			catch (Exception e) {
				throw new Exception_Invalid_Code();
			}
		}*/
	    
		bool Check_Code (std::string name, std::string code) {   
			try {
				std::string codestring = code.substr(1,code.size()-2);
	            
				// Check Checksums
				char c1test = code[0];
				char c2test = code[code.size()-1];
	        
				// Assemble checksums
				int checksum = Generate_Checksum (codestring);
				char c1 = cypher1[(0x000000FF & checksum) % cypher1.size()];
				char c2 = cypher1[(0x000000FF & (checksum >> 8)) % cypher1.size()];
	    
				// See if checksums match
				if (c1 != c1test || c2 != c2test)
					throw Exception_Invalid_Code();
	            
				std::string namestring = BuildName(name);
				if (namestring == "")
					throw Exception_Invalid_Code();
	            
				codestring = Permute (codestring, cypher1, cypher2);
	            
				std::string datestring = SubStrings (codestring, namestring);
	                    
				tm date = StringToDate(datestring);
				
				// Check the date compared to the current date
				time_t codetime = mktime(&date);
				time_t nowtime = time(NULL);
			
				return difftime(codetime, nowtime) > 0.0F;
			}
			catch (...) {
				throw new Exception_Invalid_Code();
			}
		}
    
};
