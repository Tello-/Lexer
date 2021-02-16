#pragma once

#include <vector>
#include <string>

class LexicalAnalyzer
{
public:
							LexicalAnalyzer(const std::string& sourceCode);
	std::vector<std::string>		LexemeList() const;
	std::vector<std::string>		TokenList() const;
	std::string				PrintTable() const;



private: // Private Data

	enum Lexer_State{};
	Lexer_State m_currentState;
	std::string m_sourceString;

private: // Private Methods
	
	void							SetState(Lexer_State);

};

/*

Assignment1 Tokens and Lexemes list for CPSC323 Spring 2021: Anthony Le

TOKENS			Example Lexemes

KEYWORDS 	=	int, float, bool, True, False, if, else, then, endif, endelse, while, whileend, do, enddo, for, endfor, STDinput, STDoutput, and, or, not
IDENTIFIERS 	=	legal identifiers must start with alphabetic character follow by digits, letters, underscore or $
SEPARATORS 	=	(){}[],.:;
OPERATORS 	=	*+-=/><%


Additional examples and symbols:
Valid IDENTIFIERS	:  	num, num1, large$, num$1, num2, num2$, a9, ab, ab2, my_num, Max_Num, etc...
Valid Numbers		:	integers whole numbers (0,1,2,3,...9) and reals or floats (5.0, 0.9, 1.75, ...)
Valid Block Comments	:	!  this is a block comment to be ignored by the Lexical Analyzer !
Special Characters   	:	the ! (exclamation mark), the _ (underscore) and the sp (space character)

*/