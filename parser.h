/* The following H&M Codes are allowed:
* H00 - Rapid traverse
* H01 - Linear interpolation at a given feed rate
* H02 - Clockwise interpolation
* H03 - Counterclockwise interpolation
* M02 - End of program
* All instructions must be in mm (except feed rate)
* All coordinates are absolute besides centerpoints
Any other instructions will be ignored */

#pragma once

#include "Stepper.h"
#include "cnc.h"

class Parser {
public:
  Parser(const char* program);
  ~Parser();
	void loadProgram(const char *filePath);
	void executeProgram();
private:
  void executeLine(const char *code);
  char* getInstructionTokens(const char *code);
  void move(vec2 movement);
  void moveX(float dist);
  void moveY(float dist);
  void moveZ(float dist);
  float extractFloat(const char* token);
  const char *m_program;
  const CNC *m_cnc;
};