#include "phrases.h"
#include "assembler.h"
#include <QDebug>
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>

Assembler::Assembler(QObject *parent) : QThread(parent), assemblerRunning(false)
{
}

Assembler::~Assembler(void)
{
}

opcode_t Assembler::opcodeFor(char* command)
{
	if (!strcmp(command, "set")) {
		return OP_SET;
	}

	if (!strcmp(command, "add")) {
		return OP_ADD;
	}

	if (!strcmp(command, "sub")) {
		return OP_SUB;
	}

	if (!strcmp(command, "mul")) {
		return OP_MUL;
	}

	if (!strcmp(command, "div")) {
		return OP_DIV;
	}

	if (!strcmp(command, "mod")) {
		return OP_MOD;
	}

	if (!strcmp(command, "shl")) {
		return OP_SHL;
	}

	if (!strcmp(command, "shr")) {
		return OP_SHR;
	}

	if (!strcmp(command, "and")) {
		return OP_AND;
	}

	if (!strcmp(command, "bor")) {
		return OP_BOR;
	}

	if (!strcmp(command, "xor")) {
		return OP_XOR;
	}

	if (!strcmp(command, "ife")) {
		return OP_IFE;
	}

	if (!strcmp(command, "ifn")) {
		return OP_IFN;
	}

	if (!strcmp(command, "ifg")) {
		return OP_IFG;
	}
	if (!strcmp(command, "ifb")) {
		return OP_IFB;
	}

	// Assume non-basic
	return OP_NONBASIC;
}

// Get non-basic opcode from string
nonbasicOpcode_t Assembler::nonbasicOpcodeFor(char* command)
{
	if (!strcmp(command, "jsr")) {
		return OP_JSR;
	}

	// Instruction not found
	qDebug() << "ERROR: Unknown instruction \"" << command << "\"";
	return 0;
}

// Check register number 
int Assembler::registerFor(char regName)
{
	switch(regName) {
	case 'a':
		return 0;
		break;
	case 'b':
		return 1;
		break;
	case 'c':
		return 2;
		break;
	case 'x':
		return 3;
		break;
	case 'y':
		return 4;
		break;
	case 'z':
		return 5;
		break;
	case 'i':
		return 6;
		break;
	case 'j':
		return 7;
		break;
	default:
		return -1;
		break;
	}
}

// Get argument value for string
argumentStruct_t Assembler::argumentFor(char* arg)
{
	argumentStruct_t toReturn;

	toReturn.badArgument = false;
	toReturn.labelReference = NULL;

	if (strlen(arg) == 0) {
		qDebug() << "ERROR: Empty argument string";

		toReturn.badArgument = true;
		toReturn.errorCode = ASSEMBLER_EMPTY_ARG_STR;

		return toReturn;
	}

	// If it begins with 0-9 it's a number
	if (arg[0] >= '0' && arg[0] <= '9') {
		int argValue;
		char* format;

		if (strlen(arg) > 2 && arg[0] == '0' && arg[1] == 'x') {
			// Value is hex
			format = "%x";
		} else {
			// Decimal
			format = "%d";
		}

		if (sscanf(arg, format, &argValue) != 1) {
			qDebug() << "ERROR: Invalid literal value: " << arg;

			toReturn.badArgument = true;
			toReturn.errorCode = ASSEMBLER_INVALID_LITERAL;

			return toReturn;
		}

		if (argValue < ARG_LITERAL_END - ARG_LITERAL_START) {
			toReturn.argument = ARG_LITERAL_START + argValue;

			return toReturn;
		}

		toReturn.argument = ARG_NEXTWORD;
		toReturn.nextWord = argValue;

		return toReturn;
	}

	if (arg[0] == '[' || arg[0] == '(') {
		if (strlen(arg) == 3 && (arg[2] == ']' || arg[2] == ')')) {
			// If it's 1 char in bracket it's a register
			int regNum = registerFor(arg[1]);

			if (regNum != -1) {
				toReturn.argument = ARG_REG_INDEX_START + regNum;
				return toReturn;
			} else {
				qDebug() << "ERROR: Invalid [register]: " << arg;
				
				toReturn.badArgument = true;
				toReturn.errorCode = ASSEMBLER_INVALID_REG;

				return toReturn;
			}
		}

		// Hex value?
		int hexValue;
		if (sscanf(arg + 1, "0x%x", &hexValue) == 1) {
			// +register?
			char regName;
			if (sscanf(arg + 1, "0x%x+%c", &hexValue, &regName) == 2) {
				// TODO enforce closing
				int regNum = registerFor(regName);

				if (regNum != -1) {
					toReturn.argument = ARG_REG_NEXTWORD_INDEX_START + regNum;
					toReturn.nextWord = hexValue;
					return toReturn;
				} else {
					qDebug() << "ERROR: Invalid register name " << regName << " in: " << arg;

					toReturn.badArgument = true;
					toReturn.errorCode = ASSEMBLER_INVALID_REG_NAME;

					return toReturn;
				}
			} else {
				// Just hex in brackets
				// TODO: enforce closing
				toReturn.argument = ARG_NEXTWORD_INDEX;
				toReturn.nextWord = hexValue;
				return toReturn;
			}
		} else {
			char* labelStart = arg + 1;

			char* labelEnd = strchr(arg, '+');
			if (labelEnd == NULL) {
				labelEnd = strchr(arg, ']');
			}

			if (labelEnd == NULL) {
				labelEnd = strchr(arg, ')');
			}

			if (labelEnd == NULL) {
				qDebug() << "ERROR: Unterminated label in argument: " << arg;

				toReturn.badArgument = true;
				toReturn.errorCode = ASSEMBLER_UNTERM_LABEL;

				return toReturn;
			}

			// Store label
			char* label = (char*) malloc((labelEnd - labelStart) + 1);
			strncpy(label, labelStart, (labelEnd - labelStart));
			label[labelEnd - labelStart] = '\0';

			toReturn.labelReference = label;

			// Try to parse register
			char regName;
			if (sscanf(labelEnd, "+%c", &regName) == 1) {
				int regNum = registerFor(regName);
				if (regNum != -1) {
					toReturn.argument = ARG_REG_NEXTWORD_INDEX_START + regNum;
					return toReturn;
				} else {
					qDebug() << "ERROR: Invalid register name '" << regName << "' in: " << arg << " (" << labelEnd << ")";

					toReturn.badArgument = true;
					return toReturn;
				}
			} else {
				toReturn.argument = ARG_NEXTWORD_INDEX;
				return toReturn;
			}
		}
	}
	// Check for reserved words
	if (!strcmp(arg, "pop")) {
		toReturn.argument = ARG_POP;
		return toReturn;
	}

	if (!strcmp(arg, "peek")) {
		toReturn.argument = ARG_PEEK;
		return toReturn;
	}

	if (!strcmp(arg, "push")) {
		toReturn.argument = ARG_PUSH;
		return toReturn;
	}

	if (!strcmp(arg, "sp")) {
		toReturn.argument = ARG_SP;
		return toReturn;
	}

	if (!strcmp(arg, "pc")) {
		toReturn.argument = ARG_PC;
		return toReturn;
	}

	if (!strcmp(arg, "o")) {
		toReturn.argument = ARG_O;
		return toReturn;
	}

	// Is register?
	if (strlen(arg) == 1) {
		int regNum = registerFor(arg[0]);
		if (regNum != -1) {
			toReturn.argument = ARG_REG_START + regNum;
			return toReturn;
		}
	}

	toReturn.argument = ARG_NEXTWORD;

	// Store label for later
	char* label = (char*) malloc(strlen(arg) + 1);
	strcpy(label, arg);

	toReturn.labelReference = label;
	return toReturn;
}

std::string replace(std::string& str, const std::string& from, const std::string& to) {
	std::string temp = str;

	size_t start_pos = str.find(from);
	if(start_pos == std::string::npos)
		return false;
	return temp.replace(start_pos, from.length(), to);

}

void Assembler::startEmulator()
{
	qDebug() << "Starting emulator";
	assemblerRunning = true;

	start();
}

void Assembler::stopEmulator()
{
	assemblerRunning = false;
}

void Assembler::setFilename(std::string filename)
{
	sourceFilename = filename;
}

void Assembler::run()
{
	lineNumber = 0;

	std::string compiledFilename = replace(sourceFilename, "dasm16", "bin");

    std::ifstream sourceFile(sourceFilename);

	if (!sourceFile.is_open()) {
		qDebug() << "ERROR: Could not open source file " << sourceFilename.c_str();
		assemblerError(SOURCE_FILE_MISSING, lineNumber);
	}

	// TODO: Add automatic file naming
	FILE* compiledFile = fopen(compiledFilename.c_str(), "wb");

	if (!compiledFile) {
		qDebug() << "ERROR: Could not open output file " << compiledFilename.c_str();

		assemblerError(BIN_FILE_MISSING, lineNumber);
	}

	char lineBuffer[MAX_CHARS];

	bool foundComment = false;
	bool finished = false;

	word_t address = 0;

	assembledInstruction_t* head = NULL;
	assembledInstruction_t* tail = NULL;
	assembledInstruction_t* instruction = NULL;

	char command[MAX_CHARS], label[MAX_CHARS], arg1[MAX_CHARS], arg2[MAX_CHARS], data[MAX_CHARS];

	bool skipTillNextLine = false;	

	while (1) {
		// Reset variables
		for (int i = 0; i < MAX_CHARS; i++) {
			data[i] = '\0';

			if (!skipTillNextLine) {
				label[i] = '\0';
			}

			command[i] = '\0';
			arg1[i] = '\0';
			arg2[i] = '\0';
		}

		if (skipTillNextLine) {
			skipTillNextLine = false;
		}

		lineNumber++;

		if (sourceFile.getline(lineBuffer, MAX_CHARS).eof()) {
			finished = true;
		}


		char* temp = cleanString(lineBuffer);

		// Check if whole line is a blank
		if (strlen(temp) == 0) {
			if (strlen(label) > 0) {
				processCommand("", "", address, label, head, tail, instruction);
			}
		} else {
			// Non blank line, start processing

			// Get label if applicable
			if (temp[0] == ':') {
				processLine(temp, data, label, skipTillNextLine, command, arg1, arg2, true);

				qDebug() << "label: " << label << " "; 

			} else {
				processLine(temp, data, label, skipTillNextLine, command, arg1, arg2, false);
			}

			if (!skipTillNextLine) {

				processCommand(command, data, address, label, head, tail, instruction);

				if (strcmp(command, "dat") != 0) {
					processArg1(command, arg1, address, label, instruction);
					processArg2(command, arg2, address, label, instruction);
				}

				qDebug() << "\tCommand: " << command << " Arg1: " << arg1 << " Arg2: " << arg2 << " Dat: " << data;
			}
		}

		delete temp;

		if (finished) {
			break;
		}
	}

	std::cout << std::endl;

	for (assembledInstruction_t* instruction = head; instruction != NULL; instruction = instruction->next) {
		qDebug() << "Assembling for address " << instruction->address;

		if (instruction->data != NULL) {
			continue;
		}

		// Label reference for A
		if (instruction->a.labelReference != NULL) {
			qDebug() << "Unresolved label for a: " << instruction->a.labelReference;

			for (assembledInstruction_t* other = head; other != NULL; other = other->next) {
				if (other->label != NULL && !strcmp(other->label, instruction->a.labelReference)) {
					// Match
					qDebug() << "Resolved " << instruction->a.labelReference << " to address " << other->address;
					instruction->a.nextWord = other->address;
					instruction->a.labelReference = NULL;
					break;
				}
			}
		}

		// Label reference for B
		if (instruction->b.labelReference != NULL) {
			qDebug() << "Unresolved label for b: " << instruction->b.labelReference;

			for (assembledInstruction_t* other = head; other != NULL; other = other->next) {
				if (other->label != NULL && !strcmp(other->label, instruction->b.labelReference)) {
					// Match
					qDebug() << "Resolved " << instruction->b.labelReference << " to address " << other->address;
					instruction->b.nextWord = other->address;
					instruction->b.labelReference = NULL;
					break;
				}
			}
		}

		// Any references left?
		if (instruction->a.labelReference != NULL) {
			qDebug() << "Unresolved label for a: " << instruction->a.labelReference;
			
			assemblerError(ASSEMBLER_UNRESOLVED_LABEL_A, instruction->a.lineNumber);
		}

		if (instruction->b.labelReference != NULL) {
			qDebug() << "Unresolved label for b: " << instruction->b.labelReference;
			
			assemblerError(ASSEMBLER_UNRESOLVED_LABEL_B, instruction->b.lineNumber);
		}
	}

	// Write out code
	for (assembledInstruction_t* instruction = head; instruction != NULL; instruction = instruction->next) {
		if (instruction->data != NULL) {
			qDebug() << "DATA: " << instruction->dataLength << " words";
			fwrite(instruction->data, sizeof(word_t), instruction->dataLength, compiledFile);
			continue;
		}

		instruction_t packed = 0;
        packed = Emulator::setOpcode(packed, instruction->opcode);
        packed = Emulator::setArgument(packed, 0, instruction->a.argument);
        packed = Emulator::setArgument(packed, 1, instruction->b.argument);

		instruction_t swapped = (packed>>8) | (packed<<8);

		// Save instruction
		qDebug() << address << ": Assembled instruction: " << packed << " Swapped: " << swapped;
		fwrite(&swapped, sizeof(instruction_t), 1, compiledFile);

        if (instruction->opcode != OP_NONBASIC && Emulator::usesNextWord(instruction->a.argument)) {
			swapped = (instruction->a.nextWord>>8) | (instruction->a.nextWord<<8);

			qDebug() << ++address << ": Extra Word A: " << instruction->a.nextWord << " Swapped: " << swapped;

			fwrite(&swapped, sizeof(word_t), 1, compiledFile);
		}

        if (Emulator::usesNextWord(instruction->b.argument)) {
			swapped = (instruction->b.nextWord>>8) | (instruction->b.nextWord<<8);

			qDebug() << ++address << ": Extra Word B: " << instruction->b.nextWord << " Swapped: " << swapped;
			fwrite(&swapped, sizeof(word_t), 1, compiledFile);
		}
	}

	qDebug() << "Program compiled successfully.";

	assemblerError(ASSEMBLER_SUCESSFUL, 0);

	fclose(compiledFile);
}

void Assembler::assemblerError(int errorCode, int lineNumber)
{
	assembler_error_t* error = new assembler_error_t;

	error->errorCode = errorCode;
	error->lineNumber = lineNumber;

	emit sendAssemblerMessage(error);
}

// Remove any extra characters to make line easier to parse
char* Assembler::cleanString(char *rawLine)
{
	char* temp = new char[MAX_CHARS];
	int tempIndex = 0, rawIndex = 0;

	if (rawLine[0] != ';') {

		bool removingDuplicates = false;
		bool skippingChars = false;

		if (rawLine[0] == ' ' || rawLine[0] == '\t') {
			skippingChars = true;
		}

		while (rawLine[rawIndex] != ';' && rawLine[rawIndex] != '\0') {
			if (rawLine[rawIndex] >= 65 && rawLine[rawIndex] < 123 
				|| rawLine[rawIndex] == ':') {
					skippingChars = false;
			}

			if (!skippingChars){
				if (rawLine[rawIndex] == '\t') {
					// Replace tab with space
					temp[tempIndex++] = ' ';
				} else {
					temp[tempIndex++] = rawLine[rawIndex];
				}
			}

			rawIndex++;
		}

	}
	temp[tempIndex] = '\0';

	return temp;
}

// Split up the line and work out what values are in it.
// This is sort of shit, will need to update this at some point.
int Assembler::processLine(char *currentLine, char *data, char *label, bool &functionOnNextLine, char *command, char *arg1, char *arg2, bool containsLabel)
{
	int lineIndex = 0;						// Current position in line
	int itemIndex = 0;						// Current position in item being stored

	if (containsLabel) {
		// Don't include ':' in label
		lineIndex++;

		// Read in until either a space or end of line is found
		while (currentLine[lineIndex] != ' '  && currentLine[lineIndex] != '\t' 
			&& currentLine[lineIndex] != '\n' && currentLine[lineIndex] != '\0') {

				label[itemIndex++] = tolower(currentLine[lineIndex++]); 
		}

		label[itemIndex++] = '\0';

		int tempLineIndex = lineIndex;

		if (currentLine[lineIndex] == '\0') {
			functionOnNextLine = true;
			return 1;
		} else {
			while (currentLine[tempLineIndex] == ' ' || currentLine[tempLineIndex] == '\t'
				|| currentLine[tempLineIndex] != '\0') {

					if (currentLine[tempLineIndex] >= 65 && currentLine[tempLineIndex] < 123){
						functionOnNextLine = false;
						break;
					} else {
						functionOnNextLine = true;
					}

					tempLineIndex++;
			}
		}

		// Consume whitespace between label and command if needed
		lineIndex++;

	} 

	itemIndex = 0;

	//lineIndex++;

	// Check if label is on the same line as first statement
	if ((currentLine[lineIndex] >= 32 && currentLine[lineIndex] < 127) && 
		(currentLine[lineIndex] != ' ' || currentLine[lineIndex] != '\t')) {
			// Consume any spaces or tabs between label and command
			while (currentLine[lineIndex] == ' ' || currentLine[lineIndex] == '\t') {
				lineIndex++;
			}

			while (currentLine[lineIndex] != ' ') {
				command[itemIndex++] = currentLine[lineIndex++];
			}
	} else {
		lineIndex = 0;
		while (currentLine[lineIndex] == ' ' || currentLine[lineIndex] == '\t') {
			lineIndex++;
		}

		while (currentLine[lineIndex] != ' ' || currentLine[lineIndex] == '\t') {
			command[itemIndex++] = currentLine[lineIndex++];
		}
	}


	command[itemIndex++] = '\0';

	itemIndex = 0;

	// Check if remaining data belongs to 'dat' command.
	int i = strcmp(command, "dat");
	if (strcmp(command, "dat") == 0) {
		while (currentLine[lineIndex] == ' ' || currentLine[lineIndex] == '\t') {
			lineIndex++;
		}

		while (currentLine[lineIndex] != '\0' && currentLine[lineIndex] != ';' 
			&& currentLine[lineIndex] != '0' && currentLine[lineIndex] != '\n'){
				data[itemIndex++] = currentLine[lineIndex++];
		}

		data[itemIndex++] = '\0';

	} else {

		itemIndex = 0;

		while (currentLine[lineIndex] == ' ' || currentLine[lineIndex] == '\t') {
			lineIndex++;
		}

		// Find first arg
		// This will start with either 'a-z', 'A-Z', or '['
		if ((currentLine[lineIndex] >= 48 && currentLine[lineIndex] < 58) 
			|| (currentLine[lineIndex] >= 65 && currentLine[lineIndex] < 123) 
			|| currentLine[lineIndex] == '[') {
			while (currentLine[lineIndex] != ',' && currentLine[lineIndex] != ' ') {
				if (currentLine[lineIndex] == '\0') {
					// ',' was not found
					qDebug() << "\",\" not found.";

					return -1;
				}

				arg1[itemIndex++] = currentLine[lineIndex++];
			}
		}

		arg1[itemIndex++] = '\0';

		itemIndex = 0;

		// Find second arg, optional
		// Find start of second arg
		bool hasArg2 = false;

		if (currentLine[lineIndex] == ',') {
			// Check next character for ',' indicating a second arg
			hasArg2 = true;
		} else {
			while (currentLine[lineIndex] == ' ' && currentLine[lineIndex] != '\0' ) {
				lineIndex++;
			}
			if (currentLine[lineIndex] == ','){
				hasArg2 = true;
			}
		}

		if (hasArg2){
			while (currentLine[lineIndex] < 48 || currentLine[lineIndex] >= 123) {
				lineIndex++;
			}

			while (currentLine[lineIndex] != '\0' && currentLine[lineIndex] != ' ' 
				&& currentLine[lineIndex] != '\t' && currentLine[lineIndex] != '\n'){

					arg2[itemIndex++] = currentLine[lineIndex++];
			}


			arg2[itemIndex++] = '\0';
		} else {
			arg2[0] = '\0';
		}

	}
	itemIndex = 0;
}

// Process the command
int Assembler::processCommand(char* command, char *data, word_t &address, char* label, assembledInstruction_t *&head,  assembledInstruction_t *&tail, assembledInstruction_t *&instruction)
{
	int i = 0, index = 0;

	while (command[i] != '\0') {
		command[i] = tolower(command[i]);
		i++;
	}

	instruction = new assembledInstruction_t;
	if (head == NULL) {
		head = instruction;
		tail = instruction;
	} else {
		tail->next = instruction;
		tail = tail->next;
	}

	instruction->next = NULL;
	instruction->address = address;
	instruction->label = strdup(label);

	instruction->data = NULL;

	if (!strcmp(command, "dat")) {
		instruction->data = (word_t*) malloc(MAX_CHARS * sizeof(word_t));
		instruction->dataLength = 0;

		while(1) {

			int nextChar = data[index++];
			if (nextChar == '"') {
				qDebug() << "Reading string.";

				bool_t escaped = 0;
				while(1) {
					nextChar = data[index++];
					char toPut;

					if (escaped) {
						// Escape translation
						switch(nextChar) {
						case 'n':
							toPut = '\n';
							break;

						case 't':
							toPut = '\t';
							break;

						case '\\':
							toPut = '\\';
							break;

						case '"':
							toPut = '"';
							break;

						default:
							qDebug() << "ERROR: Unrecognized escape sequence " << nextChar;
							return -1;
						}

						escaped = 0;
					} else if (nextChar == '"' || nextChar == '\0') {
						break;
					} else if (nextChar == '\\') {
						escaped = 1;
						continue;
					} else {
						// Normal character
						toPut = nextChar;
					}

					instruction->data[instruction->dataLength++] = toPut;
					std::cout << toPut;
				}

				std::cout << std::endl;
			} else {
				int nextNextChar = data[index++];

				if (nextNextChar == -1) {
					break;
				}

				if (nextChar == '0' && nextNextChar == 'x') {
					// Revert back 2 chars.
					data[index - 2];

					// Hex literal
					qDebug() << "Reading hex literal";

					if (!sscanf(data, "0x%hx", &instruction->data[instruction->dataLength]) == 1) {
						qDebug() << "ERROR: Expected hex literal";
						return -1;
					}

					instruction->dataLength++;
				} else if(sscanf(data, "%hu", &instruction->data[instruction->dataLength]) == 1) {
					// Decimal literal
					qDebug() << "Reading decimal literal";
					instruction->dataLength++;
				} else {
					// Not a real literal
					qDebug() << "Out of literals";
					break;
				}

			}

		}

		address += instruction->dataLength;
	}

	return 1;
}

// Process argument 1
void Assembler::processArg1(char* command, char* arg, word_t &address, char* label, assembledInstruction_t *&instruction)
{
	int i = 0;

	bool preserveArg = false;
	char tempArg[MAX_CHARS], preservedArg[MAX_CHARS];
	int j = 0, temp = 0;

	int len = strlen(arg);

	memcpy(tempArg, arg, len + 1);

	while (arg[i] != '\0') {
		if (arg[i] == ',') {

			arg[i] = '\0';

			continue;
		} 

		arg[i] = tolower(arg[i]);
		i++;
	}

	// Determine opcode
	instruction->opcode = opcodeFor(command);

	instruction->a = argumentFor(arg);

	instruction->a.lineNumber = lineNumber;

	if (instruction->a.badArgument) {
		assemblerError(instruction->a.errorCode, lineNumber);
	}

	// Advance address
	address++;

    if (Emulator::usesNextWord(instruction->a.argument)) {
		address++;
	}
}

// Process argument 2
void Assembler::processArg2(char* command, char* arg, word_t &address, char* label, assembledInstruction_t *&instruction)
{
	int i = 0;

	bool preserveArg = false;
	char tempArg[MAX_CHARS], preservedArg[MAX_CHARS];
	int j = 0, temp = 0;

	int len = strlen(arg);

	if (len == 0) {
		// No second arg
		instruction->b = instruction->a;

		instruction->a.argument = (argument_t) nonbasicOpcodeFor(command);
		instruction->a.labelReference = NULL;

	} else {
		memcpy(tempArg, arg, len + 1);

		while (arg[i] != '\0') {
			if (arg[i] == ',') {

				arg[i] = '\0';

				continue;
			} 

			arg[i] = tolower(arg[i]);
			i++;
		}

		instruction->b = argumentFor(arg);

		instruction->b.lineNumber = lineNumber;

		if (instruction->b.badArgument) {
			assemblerError(instruction->b.errorCode, lineNumber);
		}

        if (Emulator::usesNextWord(instruction->b.argument)) {
			address++;
		}
	}
}
