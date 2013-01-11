/**
Common Utils
Written by James Whitwell, 2012.

Started 7-Apr-2012 
*/

#include "include/utils.h"

Utils::Utils(void)
{
}


Utils::~Utils(void)
{
}

word_t Utils::pack(word_t opcode, argument_t argA, argument_t argB)
{
	// Format 5 bit opcode, 5 bit argB, 6 bit argA
	return (argB << 10)  | (argA << 5) | opcode;
}

word_t Utils::setOpcode(word_t instruction, word_t opcode)
{
	// Clear low 4 bits and OR in opcode
	return (instruction & 0xFFF0) | opcode; 
}

word_t Utils::setArgument(word_t instruction, bool_t which, argument_t argument)
{
	if (!which) {
		// A argument
		return (instruction & 0xFC1F) | (((word_t) argument) << 4);
	} else {
		// B argument
		return (instruction & 0x03FF) | (((word_t) argument) << 10);		
	}
}

// Check if argument references next word
bool_t Utils::usesNextWord(argument_t argument)
{
	return (argument >= ARG_REG_NEXTWORD_INDEX_START && argument < ARG_REG_NEXTWORD_INDEX_END)
		|| argument == ARG_NEXTWORD_INDEX
		|| argument == ARG_NEXTWORD;
}

// Reverse the byte order
word_t Utils::swapByteOrder(word_t instruction)
{
	return (instruction<<8) | (instruction>>8);
}


std::string Utils::replace(std::string& str, const std::string& from, const std::string& to) {
	std::string temp = str;

	size_t start_pos = str.find(from);
	if(start_pos == std::string::npos)
		return false;
	return temp.replace(start_pos, from.length(), to);

}