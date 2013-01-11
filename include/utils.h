/**
Common Utils
Written by James Whitwell, 2012.

Started 7-Apr-2012 
*/

#ifndef _UTILS_H
#define _UTILS_H

#include "constants.h"

#include <string>

class Utils
{

public:
	Utils(void);
	~Utils(void);

	static word_t pack(word_t opcode, argument_t argA, argument_t argB);

	static bool_t usesNextWord(argument_t argument);
	static word_t setOpcode(word_t instruction, word_t opcode);
	static word_t setArgument(word_t instruction, bool_t which, argument_t argument);

	static word_t swapByteOrder(word_t instruction);

	static std::string replace(std::string& str, const std::string& from, const std::string& to);
};

#endif