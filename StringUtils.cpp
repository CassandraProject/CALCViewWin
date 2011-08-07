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

#include	"StringUtils.h"
#include	<ctype.h>

std::string				ToUppercase		(std::string s)
{
	for (unsigned int i = 0; i < s.size(); ++i)
		s[i] = toupper(s[i]);

	return s;
}

std::list<std::string>	Tokenize		(std::string s, bool separate_spaces)
{
	int cursor = 0;
	std::string token;
	std::list<std::string> token_list;
	
	for (unsigned int i = 0; i < s.size(); ++i) {
		if (separate_spaces && (!isspace(s[i])))	// Spaces separate tokens
			token = token + s[i];
		else if (!separate_spaces && (!isspace(s[i]) || s[i] == ' ')) // Spaces don't separate tokens
			token = token + s[i];
		else if (token.size() > 0) {
			token_list.push_back(token);
			token = "";
		} else
			; // Do nothing, token was empty
	
	}
	
	// Finish off list
	if (token.size() > 0)
		token_list.push_back(token);
		
	return token_list;
}

std::string				Trim			(std::string s)
{
	if (s.size() > 0) {

		// Trim beginning
		while (isspace(s[0]))
			s.erase(0,1);
			
		// Trim end
		while (isspace(s[s.size() - 1]))
			s.erase(s.size() - 1,1);
	}
		
	return s;
}

