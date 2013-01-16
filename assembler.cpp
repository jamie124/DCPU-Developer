#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QRegExp>

#include <iostream>
#include <fstream>
#include <string.h>
#include <stdio.h>

#include <QScopedPointer>

#include "include/phrases.h"
#include "include/assembler.h"

Assembler::Assembler(QObject *parent) : QThread(parent), assemblerRunning(false)
{
	createDebugFile = true;

	instructionOffset = 1;
}

Assembler::~Assembler(void)
{
}

word_t Assembler::opcodeFor(const QString command)
{
	if (command == "set") {
		return OP_SET;
	}

	if (command == "add") {
		return OP_ADD;
	}

	if (command == "sub") {
		return OP_SUB;
	}

	if (command == "mul") {
		return OP_MUL;
	}

	if (command == "mli") {
		return OP_MLI;
	}

	if (command == "div") {
		return OP_DIV;
	}

	if (command == "dvi") {
		return OP_DVI;
	}

	if (command == "mod") {
		return OP_MOD;
	}

	if (command == "mdi") {
		return OP_MDI;
	}

	if (command == "and") {
		return OP_AND;
	}

	if (command == "bor") {
		return OP_BOR;
	}

	if (command == "xor") {
		return OP_XOR;
	}

	if (command == "shr") {
		return OP_SHR;
	}

	if (command == "asr") {
		return OP_ASR;
	}

	if (command == "shl") {
		return OP_SHL;
	}

	if (command == "ifb") {
		return OP_IFB;
	}

	if (command == "ifc") {
		return OP_IFC;
	}

	if (command == "ife") {
		return OP_IFE;
	}

	if (command == "ifn") {
		return OP_IFN;
	}

	if (command == "ifg") {
		return OP_IFG;
	}

	if (command == "ifa") {
		return OP_IFA;
	}

	if (command == "ifl") {
		return OP_IFL;
	}

	if (command == "adx") {
		return OP_ADX;
	}

	if (command == "sbx") {
		return OP_SBX;
	}

	if (command == "sti") {
		return OP_STI;
	}

	if (command == "std") {
		return OP_STD;
	}

	// SPECIAL OPCODES 

	// Push address of next instruction to stack
	if (command == "jsr") {
		return OP_JSR;
	}

	// Trigger software interupt with message a
	if (command == "int") {
		return OP_INT;
	}

	// Sets a to IA
	if (command == "iag") {
		return OP_IAG;
	}

	// Sets IA to a
	if (command == "ias") {
		return OP_IAS;
	}

	// Disables interrupt queueing, pop A from stack then pop PC
	if (command == "rfi") {
		return OP_RFI;
	}

	// If a is non-zero, interrupts will be added to queue instead of triggered.
	// If a is zero interrupts will be triggered.
	if (command == "iaq") {
		return OP_IAQ;
	}

	// Sets a to number of connected hardware devices
	if (command == "hwn") {
		return OP_HWN;
	}

	// Sets A, B, C, X, Y registers to information about hardware a A+(B<<16) is a 32 bit word 
	// identifying the hardware id. C is the hardware version
	if (command == "hwq") {
		return OP_HWQ;
	}

	// Sends an interrupt to hardware a
	if (command == "hwi") {
		return OP_HWI;
	}

	// Assume non-basic
	return OP_NULL;
}

// Get non-basic opcode from string
nonbasicOpcode_t Assembler::nonbasicOpcodeFor(QString command)
{
	// Push address of next instruction to stack
	if (command == "jsr") {
		return OP_JSR;
	}

	// Trigger software interupt with message a
	if (command == "int") {
		return OP_INT;
	}

	// Sets a to IA
	if (command == "iag") {
		return OP_IAG;
	}

	// Sets IA to a
	if (command == "ias") {
		return OP_IAS;
	}

	// Disables interrupt queueing, pop A from stack then pop PC
	if (command == "rfi") {
		return OP_RFI;
	}

	// If a is non-zero, interrupts will be added to queue instead of triggered.
	// If a is zero interrupts will be triggered.
	if (command == "iaq") {
		return OP_IAQ;
	}

	// Sets a to number of connected hardware devices
	if (command == "hwn") {
		return OP_HWN;
	}

	// Sets A, B, C, X, Y registers to information about hardware a A+(B<<16) is a 32 bit word 
	// identifying the hardware id. C is the hardware version
	if (command == "hwq") {
		return OP_HWQ;
	}

	// Sends an interrupt to hardware a
	if (command == "hwi") {
		return OP_HWI;
	}

	// Instruction not found
	qDebug() << "ERROR: Unknown instruction \"" << command << "\"";
	return 0;
}

// Check register number 
int Assembler::registerFor(QChar regName)
{
	switch(regName.toAscii()) {
	case 'a':
		return REG_A;
		break;
	case 'b':
		return REG_B;
		break;
	case 'c':
		return REG_C;
		break;
	case 'x':
		return REG_X;
		break;
	case 'y':
		return REG_Y;
		break;
	case 'z':
		return REG_Z;
		break;
	case 'i':
		return REG_I;
		break;
	case 'j':
		return REG_J;
		break;
	default:
		return -1;
		break;
	}
}

// Get argument value for string
argumentStruct_t Assembler::argumentFor(QString arg) {

	argumentStruct_t toReturn;

	toReturn.badArgument = false;
	toReturn.labelReference = "";

	if (arg.length() == 0) {
		qDebug() << "ERROR: Empty argument string";

		toReturn.badArgument = true;
		toReturn.errorCode = ASSEMBLER_EMPTY_ARG_STR;

		return toReturn;
	}

	// If it begins with 0-9 it's a number
	QChar charCheck = arg.at(0);

	if (charCheck.toAscii() >= '0' && charCheck.toAscii() <= '9') {
		word_t argValue;

		QTextStream inputData(&arg);


		inputData >> argValue;

		qDebug() << QString::number(argValue);


		if (argValue == ARG_LITERAL_START) {
			toReturn.argument = 0xffff;

			return toReturn;
		} else if (argValue >= 0x21) {
			toReturn.argument = argValue - 0x21;
		}

		if (argValue < ARG_LITERAL_END - ARG_LITERAL_START) {
			toReturn.argument = ARG_LITERAL_START + (argValue == 0xffff ? 0x00 : (0x01 + argValue));

			return toReturn;
		}


		toReturn.argument = ARG_NEXTWORD;
		toReturn.nextWord = argValue;

		return toReturn;
	}

	if (charCheck.toAscii() == '[' || charCheck.toAscii() == '(') {
		if (arg.length() == 3 && (arg.at(2).toAscii() == ']' || arg.at(2).toAscii() == ')')) {
			// If it's 1 char in bracket it's a register
			int regNum = registerFor(arg.at(1).toAscii());

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

		QTextStream inputData(&arg);


		if (arg.contains("0x")) {
			//if (arg.at(1).toAscii() >= '0' && arg.at(1).toAscii() <= '9') {
			// +register?
			int hexValue;
			QChar regName;
			QString reg;

			arg = arg.replace("[", "").replace("]", "").replace("+", "").trimmed();
			arg = arg.replace("(", "").replace(")", "").trimmed();

			if (arg.at(0).toAscii() >= '0' && arg.at(0).toAscii() <= '9') {
				// Hex value before register
				inputData >> hex >> hexValue;
				inputData >> reg;
			} else {
				// Register before hex value
				inputData >> reg;
				inputData >> hex >> hexValue;
			}

			qDebug() << arg;

			qDebug() << "Hex: " + QString::number(hexValue) + ", Reg: " + reg;

			if (reg.length() == 1) {
				regName = reg.at(0);

				//	if (sscanf(tempBuffer.data() + 1, "0x%x+%c", &hexValue, &regName) == 2) {

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

			arg = arg.replace(QString(" "), QString(""));

			bool containsReg = false;

			int labelEnd = 0;

			labelEnd = arg.indexOf('+');

			if (labelEnd > -1) {
				containsReg = true;
			}

			if (labelEnd == -1) {
				labelEnd = arg.indexOf(']');
			}

			if (labelEnd == -1) {
				labelEnd = arg.indexOf(')');
			}

			if (labelEnd == -1) {
				qDebug() << "ERROR: Unterminated label in argument: " << arg;

				toReturn.badArgument = true;
				toReturn.errorCode = ASSEMBLER_UNTERM_LABEL;

				return toReturn;
			}

			// Account for last character
			labelEnd--;

			toReturn.labelReference = arg.mid(1, labelEnd);

			// Try to parse register

			if (containsReg) {
				// Find register, sort of shit way of doing it.
				// Finds first character after '+'
				//char regName = arg.mid(labelEnd + 2, 1).toStdString().c_str()[0];
				QString regName = arg.mid(labelEnd + 2, 1);

				qDebug() << regName;

				int regNum = registerFor(regName.at(0));
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
	if (arg =="push") {
		toReturn.argument = ARG_PUSH_POP;
		return toReturn;
	}

	if (arg =="pop") {
		toReturn.argument = ARG_PUSH_POP;
		return toReturn;
	}

	if (arg =="peek") {
		toReturn.argument = ARG_PEEK;
		return toReturn;
	}

	if (arg =="sp") {
		toReturn.argument = ARG_SP;
		return toReturn;
	}

	if (arg =="pc") {
		toReturn.argument = ARG_PC;
		return toReturn;
	}

	if (arg =="ex") {
		toReturn.argument = ARG_EX;
		return toReturn;
	}

	// Is register?
	if (arg.length() == 1) {
		int regNum = registerFor(arg.at(0));
		if (regNum != -1) {
			toReturn.argument = ARG_REG_START + regNum;
			return toReturn;
		}
	}

	toReturn.argument = ARG_NEXTWORD;

	// Store label for later
	//char* label = (char*) malloc(strlen(arg) + 1);
	//strcpy(label, arg);


	toReturn.labelReference = arg;
	return toReturn;
}



void Assembler::startAssembler()
{
	qDebug() << "Starting assembler";
	assemblerRunning = true;

	start();
}

void Assembler::stopAssembler()
{
	assemblerRunning = false;

	wait();
}

void Assembler::setFilename(const QString filename)
{
	sourceFilename = filename;
}

void Assembler::run()
{
	lineNumber = 0;

	instructionOffset = 1;

	std::string compiledFilename = Utils::replace(sourceFilename.toStdString(), "dasm16", "bin");
	std::string debugFilename = "debug_" + compiledFilename;
	std::string debugInfoFilename = Utils::replace(sourceFilename.toStdString(), "dasm16", "dbg");

	//std::ifstream sourceFile(sourceFilename.toStdString());

	qDebug() << sourceFilename;
	QFile sourceFile(sourceFilename);

	if (!sourceFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "ERROR: Could not open source file " << sourceFilename;
		assemblerError(SOURCE_FILE_MISSING, lineNumber);
	}

	QTextStream sourceInput(&sourceFile);

	// TODO: Add automatic file naming
	FILE* compiledFile = fopen(compiledFilename.c_str(), "wb");

	if (!compiledFile) {
		qDebug() << "ERROR: Could not open output file " << compiledFilename.c_str();

		assemblerError(BIN_FILE_MISSING, lineNumber);
	}


	QFile debugInfoFile(debugInfoFilename.c_str());

	if (!debugInfoFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		qDebug() << "ERROR: Unable to write a debug info file " << debugInfoFilename.c_str();
	}

	// Open debug stream
	QTextStream debugOut(&debugInfoFile);


	FILE* debugFile = fopen(debugFilename.c_str(), "wb");

	if (!debugFile) {
		qDebug() << "ERROR: Unable to write a debug file " << debugFilename.c_str();
	}

	char lineBuffer[MAX_CHARS];

	bool foundComment = false;
	bool finished = false;

	word_t address = 0;

	assembledInstruction_t* head = NULL;
	assembledInstruction_t* tail = NULL;
	assembledInstruction_t* instruction = NULL;

	QString command, label, arg1, arg2, data;

	//char* tempBuffer;

	bool skipTillNextLine = false;	

	QString currentLine;

	//currentLine = sourceInput.readLine();

	while (!sourceInput.atEnd()) {

		currentLine = sourceFile.readLine().trimmed();

		//qDebug() << currentLine;

		// Reset variables

		if (!skipTillNextLine) {
			label = "";
		}

		command = "";
		data = "";
		arg1 = "";
		arg2 = "";

		if (skipTillNextLine) {
			skipTillNextLine = false;
		}

		lineNumber++;

		removeComment(currentLine);

		if (currentLine.length() <= 1) {

			/*
			if (label.length() > 0) {
			processCommand(QString(""), QString(""), address, label, head, tail, instruction);
			}
			*/
		} else {

			// Non blank line, start processing

			// Get label if applicable
			if (currentLine[0] == ':') {
				processLine(currentLine, data, label, skipTillNextLine, command, arg1, arg2, true);


				qDebug() << "label: " << label << " "; 

			} else {
				processLine(currentLine, data, label, skipTillNextLine, command, arg1, arg2, false);
			}

			if (!skipTillNextLine) {


				processCommand(command, data, address, label, head, tail, instruction);


				if (command != "dat") {
					processArg1(command, arg1, address, label, instruction);
					processArg2(command, arg2, address, label, instruction);
				} 

				qDebug() << "\tCommand: " << command << " Arg1: " << arg1 << " Arg2: " << arg2 << " Dat: " << data << " Address: " << QString::number(address);
			}
		}

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
		if (instruction->a.labelReference != "") {
			qDebug() << "Unresolved label for a: " << instruction->a.labelReference;

			for (assembledInstruction_t* other = head; other != NULL; other = other->next) {
				if (other->label != NULL && other->label == instruction->a.labelReference) {
					// Match
					qDebug() << "Resolved " << instruction->a.labelReference << " to address " << other->address;
					instruction->a.nextWord = other->address;

					//instructionOffset += 2;

					instruction->a.labelReference = "";
					break;
				}
			}
		}

		// Label reference for B
		if (instruction->b.labelReference != "") {
			qDebug() << "Unresolved label for b: " << instruction->b.labelReference;

			for (assembledInstruction_t* other = head; other != NULL; other = other->next) {

				if (other->label != NULL && other->label == instruction->b.labelReference) {
					// Match
					qDebug() << "Resolved " << instruction->b.labelReference << " to address " << other->address;
					instruction->b.nextWord = other->address;

					//instructionOffset += 2;

					instruction->b.labelReference = "";
					break;
				}
			}
		}

		// Any references left?
		if (instruction->a.labelReference != "") {
			qDebug() << "Unresolved label for a: " << instruction->a.labelReference;

			assemblerError(ASSEMBLER_UNRESOLVED_LABEL_A, instruction->a.lineNumber);
		}

		if (instruction->b.labelReference != "") {
			qDebug() << "Unresolved label for b: " << instruction->b.labelReference;

			assemblerError(ASSEMBLER_UNRESOLVED_LABEL_B, instruction->b.lineNumber);
		}
	}

	//address = 0;

	// Write out code
	for (assembledInstruction_t* instruction = head; instruction != NULL; instruction = instruction->next) {

		debugOut << QString::number(instruction->a.lineNumber);

		if (instruction->data != NULL) {
			// Reverse endianess
			for (int i = 0; i < instruction->dataLength; i++) {
				debugOut << "|" << QString::number(instruction->data[i], 16).rightJustified(4, '0');

				instruction->data[i] = Utils::swapByteOrder(instruction->data[i]);

				debugOut << "\n";
			}

			qDebug() << "DATA: " << instruction->dataLength << " words";
			fwrite(instruction->data, sizeof(word_t), instruction->dataLength, compiledFile);

			if (createDebugFile) {
				fwrite(instruction->data, sizeof(word_t), instruction->dataLength, debugFile);
			}

			continue;
		}

		word_t packed = 0;

		qDebug() << "Opcode: " << instruction->opcode << "Arg A: " << instruction->a.argument;


		packed = Utils::pack(instruction->opcode, instruction->a.argument, instruction->b.argument);


		qDebug() << instruction->opcode << instruction->a.argument << instruction->b.argument;


		word_t swapped = (packed>>8) | (packed<<8);

		// Save instruction
		qDebug() << instruction->address << ": Assembled instruction: " << QString::number(packed, 16).rightJustified(4, '0') << " Swapped: " << QString::number(swapped, 16).rightJustified(4, '0') << "Opcode: " << instruction->opcode;

		if (createDebugFile) {
			word_t debugInfo = ((word_t)instruction->lineNumber) | 0xA000;

			debugInfo = (debugInfo >> 8) | (debugInfo << 8);

			fwrite(&debugInfo, sizeof(word_t), 1, debugFile);
		}

		fwrite(&swapped, sizeof(word_t), 1, compiledFile);

		if (createDebugFile) {
			fwrite(&swapped, sizeof(word_t), 1, debugFile);
		}

		debugOut << "|" << QString::number(packed, 16).rightJustified(4, '0');

		if (instruction->opcode != OP_NULL && Utils::usesNextWord(instruction->a.argument)) {
			swapped = (instruction->a.nextWord>>8) | (instruction->a.nextWord<<8);

			qDebug() << instruction->address << ": Extra Word A: " << QString::number(instruction->a.nextWord, 16).rightJustified(4, '0') << " Swapped: " << QString::number(swapped, 16).rightJustified(4, '0');

			debugOut << ":" << QString::number(instruction->a.nextWord, 16).rightJustified(4, '0');

			fwrite(&swapped, sizeof(word_t), 1, compiledFile);

			if (createDebugFile) {
				fwrite(&swapped, sizeof(word_t), 1, debugFile);
			}
		}

		if (Utils::usesNextWord(instruction->b.argument)) {
			swapped = (instruction->b.nextWord>>8) | (instruction->b.nextWord<<8);

			qDebug() << instruction->address << ": Extra Word B: " << QString::number(instruction->b.nextWord, 16).rightJustified(4, '0') << " Swapped: " << QString::number(swapped, 16).rightJustified(4, '0');

			debugOut << ":" << QString::number(instruction->b.nextWord, 16).rightJustified(4, '0');

			fwrite(&swapped, sizeof(word_t), 1, compiledFile);

			if (createDebugFile) {
				fwrite(&swapped, sizeof(word_t), 1, debugFile);
			}
		}


		debugOut << "\n";

		// TODO
		//delete instruction;
	}

	qDebug() << "Compile finished.";

	debugInfoFile.flush();
	debugInfoFile.close();

	// Close files

	fclose(compiledFile);

	fclose(debugFile);

	assemblerError(ASSEMBLER_SUCESSFUL, 0);
}

void Assembler::assemblerError(int errorCode, int lineNumber)
{
	assembler_update_t* error = new assembler_update_t;

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

// Remove comments from a string
void Assembler::removeComment(QString &input) {
	if (input.contains(";")) {
		int startOfComment = input.indexOf(";");

		input = input.left(startOfComment).trimmed();

	}
}

// Split up the line and work out what values are in it.
// This is sort of shit, will need to update this at some point.
int Assembler::processLine(const QString currentLine, QString &data, QString &label, bool &functionOnNextLine, QString &command, QString &arg1, QString &arg2, bool containsLabel) {
	int lineIndex = 0;						// Current position in line
	int itemIndex = 0;						// Current position in item being stored

	//char *tempBuffer = new char[MAX_CHARS];
	QString tempBuffer;

	// Remove trailing characters
	QString remainingLine = currentLine.trimmed();

	QRegExp whitespace(" ");

	if (containsLabel) {
		// Find first space, indicates end of label
		itemIndex = remainingLine.indexOf(whitespace);
		//lineIndex += itemIndex;

		//qDebug() << QString::number(itemIndex);
		// Read in any text after ':' till first whitespace.
		label = remainingLine.left(itemIndex).replace(":", "").toLower();

		remainingLine = remainingLine.right(remainingLine.length() -  itemIndex).trimmed();


		//qDebug() << remainingLine;


		//label = tempBuffer.replace(":", "").toLower();



		if (itemIndex > -1) {

			functionOnNextLine = false;
		} else {
			remainingLine = "";

			functionOnNextLine = true;

			return 1;
		}

	} 

	// Get command
	itemIndex = remainingLine.indexOf(whitespace);
	//lineIndex += itemIndex;

	//qDebug() << QString::number(itemIndex);
	command = remainingLine.left(itemIndex).toLower();

	remainingLine = remainingLine.right(remainingLine.length() - itemIndex).trimmed();

	itemIndex = 0;


	// Check if remaining data belongs to 'dat' command.
	if (command == "dat") {
		data = remainingLine;

	} else {
		// Find second arg, optional
		// Find start of second arg
		bool hasArg2 = false;

		QRegExp argSeperator(",(\s+)?");

		itemIndex = remainingLine.indexOf(argSeperator);

		//qDebug() << QString::number(itemIndex);

		if (itemIndex == -1) {
			arg1 = remainingLine;

		} else {
			arg1 = remainingLine.left(itemIndex);
			arg2 = remainingLine.right(remainingLine.length() - itemIndex);

			arg2 = arg2.replace(argSeperator, "");
		}
	}

	return 0;
}

// Process the command
int Assembler::processCommand(QString &command, QString &data, word_t &address, QString &label, assembledInstruction_t *&head,  assembledInstruction_t *&tail, assembledInstruction_t *&instruction) {
	int i = 0, index = 0;

	//QScopedPointer<const char> dataBuffer(_strdup(data.toStdString().c_str()));
	QString dataBuffer = data;

	command = command.toLower();

	if (command == "dat") {
		qDebug() << "dat found";
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

	// Set base address
	instruction->address = address;

	if (command != "dat") {
		// When creating a debug file add extra instrcution
		address += (createDebugFile ? 1 : 0);
	}


	instruction->label = label;

	instruction->data = NULL;

	if (command == "dat") {
		instruction->data = (word_t*) malloc(MAX_CHARS * sizeof(word_t));
		instruction->dataLength = 0;

		qDebug() << "Data: " + data;

		if (data.length() == 0) {
			emit assemblerError(ASSEMBLER_EMPTY_DAT, instruction->a.lineNumber);
		} else {

			QChar nextChar = data.at(0);

			if (nextChar == '"') {

				index = 1;

				//if (nextChar == '"') {
				qDebug() << "Reading string.";

				bool escaped = false;
				while(1) {

					nextChar = data.at(index++).toAscii(); //dataBuffer.data()[index++];
					char toPut;

					if (escaped) {
						// Escape translation
						switch(nextChar.toAscii()) {
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

						escaped = false;
					} else if (nextChar == '"') {
						break;
					} else if (nextChar == '\\') {
						escaped = true;
						continue;
					} else {
						// Normal character
						toPut = nextChar.toAscii();
					}

					instruction->data[instruction->dataLength++] = toPut;

					//qDebug() << instruction->data[instruction->dataLength];

					//for (int j = 0; j < instruction->dataLength; j++) {
					//	std::cout << (char)instruction->data[j];
					//}

					//std::cout << std::endl;
				}
			} else {
				QTextStream inputData(&data);

				unsigned short dataValue;

				inputData >> dataValue;

				//qDebug() << QString::number(dataValue);

				instruction->data[instruction->dataLength++] = dataValue;

				qDebug() << QString::number(instruction->data[0]);
			}



			address += instruction->dataLength;
		}
	} 

	//address = instruction->address;

	return 1;
}

// Process argument 1
void Assembler::processArg1(QString &command, QString &arg, word_t &address, QString &label, assembledInstruction_t *&instruction)
{
	int i = 0;

	bool preserveArg = false;
	int j = 0, temp = 0;

	// Determine opcode
	instruction->opcode = opcodeFor(command);

	arg = arg.toLower().trimmed();

	qDebug() << "Command: " << command << "Opcode: " << instruction->opcode;

	qDebug() << "Arg: " << arg;

	instruction->lineNumber = lineNumber;

	instruction->a = argumentFor(arg);

	instruction->a.lineNumber = lineNumber;

	if (instruction->a.badArgument) {
		assemblerError(instruction->a.errorCode, lineNumber);
	}

	// Advance address
	address++;

	if (Utils::usesNextWord(instruction->a.argument)) {
		//address +=  (createDebugFile ? 2 : 1);
		address++;
	}

	//address = instruction->address;
}

// Process argument 2
void Assembler::processArg2(QString &command, QString &arg, word_t &address, QString &label, assembledInstruction_t *&instruction)
{
	int i = 0;


	bool preserveArg = false;
	//char tempArg[MAX_CHARS], preservedArg[MAX_CHARS];
	int j = 0, temp = 0;

	//int len = strlen(arg);

	if (arg.length() == 0) {
		// No second arg
		instruction->b = instruction->a;

		instruction->a.argument = (argument_t) opcodeFor(command);
		instruction->a.labelReference = "";

	} else {

		arg = arg.toLower().trimmed();

		qDebug() << "Arg 2: " + arg;
		instruction->b = argumentFor(arg);

		instruction->b.lineNumber = lineNumber;

		if (instruction->b.badArgument) {
			assemblerError(instruction->b.errorCode, lineNumber);
		}

		//address++;

		if (Utils::usesNextWord(instruction->b.argument)) {
			//address +=  (createDebugFile ? 2 : 1);
			address++;
		}

	}
	
	//address = instruction->address;
}
