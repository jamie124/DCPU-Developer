#include "include/constants.h"
#include "include/phrases.h"

Phrases::Phrases(void)
{
	// Return codes
	responses.insert(SOURCE_FILE_MISSING, "Source file missing");
	responses.insert(BIN_FILE_MISSING, "Binary file missing");
	responses.insert(DCPU_SUCCESSFUL, "Emulator finished sucessfully");
	responses.insert(DCPU_RESERVED_OPCODE, "Reserved Opcode hit");
	responses.insert(ASSEMBLER_SUCESSFUL, "Assembler finished");
	responses.insert(ASSEMBLER_FAILED, "Assembler failed");

	// Emulator error messages
	responses.insert(DCPU_OVERFULL_INTERRUPT_QUEUE, "Overfull interrupt queue");

	// Assembler error messages
	responses.insert(ASSEMBLER_INVALID_REG, "Invalid Register");
	responses.insert(ASSEMBLER_UNRESOLVED_LABEL_A, "Unresolved Label A");
	responses.insert(ASSEMBLER_UNRESOLVED_LABEL_B, "Unresolved Label B");
	responses.insert(ASSEMBLER_EMPTY_DAT, "Dat data not found");

}


Phrases::~Phrases(void)
{
	responses.clear();
}


QString Phrases::getResponseMessage(int errorCode)
{
	return responses.value(errorCode);
}