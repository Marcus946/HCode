#pragma once

#include "Stepper.h"

struct vec2 {
  vec2(float x_val, float y_val) : x(x_val), y(y_val) {}
  vec2(float val) : x(val), y(val) {}
  float x, y;
};

class CNC {
public:
  CNC(int pinsX[], int pinsY[]);
  ~CNC();
  void linearInterp(vec2 dest, float feedrate);
  void circularInterp(vec2 dest, float radius, const char* mode, float feedrate);
private:
  void inline stepX(int steps);
  void inline stepY(int steps);
  // don't forget to change later
  static constexpr float MMPERSTEP = 0.1f;
  static constexpr float MMPSPERIPM = 0.42333333333333;
  static constexpr unsigned int STEPSPERREV = 96;
  Stepper *m_xStepper;
  Stepper *m_yStepper;
  float m_xPos;
  float m_yPos;
};