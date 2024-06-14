#include "parser.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>

Parser::Parser(const char* program) {
  m_program = program;
  int pinsX[] = {3, 4, 5, 6};
  int pinsY[] = {7, 8, 9, 10};
  const CNC cnc = CNC(pinsX, pinsY);
  m_cnc = &cnc;
}

void Parser::loadProgram(const char *program) {
  m_program = program;
}

void Parser::executeLine(const char *code) {
  // tokens is init'ed as addr of first mem location
  // so it can be indexed, but alse returned from function
  const char *tokens = getInstructionTokens(code);
  if (tokens[1] == "H00") {
    // example: N013 H00 X0.15 Y4.83
    vec2 dest(0.0f);
    dest.x = extractFloat(tokens[2]);
    dest.y = extractFloat(tokens[3]);
    m_cnc->linearInterp(dest, 40.0f);
  }
  else if (tokens[1] == "H01") {
    // example: N013 H01 X0.15 Y4.83 F5.0
    vec2 dest(0.0f);
    dest.x = extractFloat(tokens[2]);
    dest.y = extractFloat(tokens[3]);
    float feedrate = extractFloat(tokens[4]);
    m_cnc->linearInterp(dest, feedrate);
  }
  else if (tokens[1] == "H02") {
    // example: N013 H02 X12.5 Y14.7 R2.0
    vec2 dest(0.0f);
    dest.x = extractFloat(tokens[2]);
    dest.y = extractFloat(tokens[3]);
    float radius = extractFloat(tokens[4]);
    m_cnc->circularInterp(dest, radius, "CW", 20.0);
  }
  else if (tokens[1] == "H03") {
    // example: N013 H02 X12.5 Y14.7 R2.0
    vec2 dest(0.0f);
    dest.x = extractFloat(tokens[2]);
    dest.y = extractFloat(tokens[3]);
    float radius = extractFloat(tokens[4]);
    m_cnc->circularInterp(dest, radius, "CCW", 20.0);
  }
  else if (tokens[1] == "M02") {
    // not sure how to implement program ends yet
  }
}

char* getInstructionTokens(const char *code) {
  // get token count for array init.
  // there's 1 more token than delimiters
  int tokenCnt = 1;
  while (*code) {
    if (*code == ' ') {
      tokenCnt++;
    }
    code++;
  }
  // extract instruction tokens from code
  char *instrTokens[tokenCnt];
  char *token;
  token = strtok(code, " ");
  for (int i = 0; token != nullptr; i++) {
    instrTokens[i] = token;
    token = (nullptr, " ");
  }
  return instrTokens[0];
}

void Parser::executeProgram() {
}

float extractFloat(const char* token) {
  size_t newLength = strlen(token) - 1;
  char* newToken = new char[newLength + 1];
  strncpy(newToken, token + 1, newLength);
  newToken[newLength] = '\0';
  char* end;
  float value = strtod(newToken, &end);
  return value;
}
