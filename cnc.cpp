#include "cnc.h"
#include "math.h"
#include <Arduino.h>

CNC::CNC(int x[], int y[]) {
  Stepper stepperX(STEPSPERREV, x[0], x[1], x[2], x[3]);
  Stepper stepperY(STEPSPERREV, y[0], y[1], y[2], y[3]);
  m_xStepper = &stepperX;
  m_yStepper = &stepperY;
  m_xPos, m_yPos = 0.0f;
}

void CNC::linearInterp(vec2 dest, float feedrate) {
  // this could probably all be cleaned up
  float mmPerSec = feedrate * MMPSPERIPM;
  float distX = fabs(dest.x - m_xPos);
  float timeX = mmPerSec / distX;
  unsigned int stepsX = distX / MMPERSTEP;
  float timePerStepX = timeX / stepsX;
  float dirX = (dest.x - m_xPos > 0) ? 1.0f : -1.0f;

  float distY = fabs(dest.y - m_yPos);
  float timeY = mmPerSec / distY;
  unsigned int stepsY = distY / MMPERSTEP;
  float timePerStepY = timeY / stepsY;
  float dirY = (dest.y - m_yPos > 0) ? 1.0f : -1.0f;

  bool finished = false;
  bool startTime = millis();
  const float timeTolerance = 0.05f;
  while (!finished) {
    float execTime = millis() - startTime;
    if (execTime - (timePerStepX * (execTime / timeX)) < timeTolerance) {
      stepX(1 * dirX);
    }
    if (execTime - (timePerStepY * (execTime / timeY)) < timeTolerance) {
      stepX(1 * dirY);
    }
  }
}

void CNC::circularInterp(vec2 dest, float radius, const char* mode, float feedrate) {
  // step motors at each 1/STEPSPERREV of whole number interval of trig graph
  float mmPerSec = feedrate * MMPSPERIPM;
  float distX = fabs(dest.x - m_xPos);
  float timeX = mmPerSec / distX;
  unsigned int stepsX = distX / MMPERSTEP;
  float dirX = (dest.x - m_xPos > 0) ? 1.0f : -1.0f;

  float distY = fabs(dest.y - m_yPos);
  float timeY = mmPerSec / distY;
  unsigned int stepsY = distY / MMPERSTEP;
  float dirY = (dest.y - m_yPos > 0) ? 1.0f : -1.0f;

  bool finished = false;
  bool startTime = millis();
  const float timeTolerance = 0.05f;
  while (!finished) {
    float timePerStepX = cos(timeX / stepsX);
    float timePerStepY = sin(timeX / stepsX);
    float execTime = millis() - startTime;
    if (execTime - (timePerStepX * (execTime / timeX)) < timeTolerance) {
      stepX(1 * dirX);
    }
    if (execTime - (timePerStepY * (execTime / timeY)) < timeTolerance) {
      stepX(1 * dirY);
    }
  }
}

void CNC::stepX(int steps) {
  m_xStepper->step(steps);
  m_xPos += steps * (MMPERSTEP);
}

void CNC::stepY(int steps) {
  m_yStepper->step(steps);
  m_yPos += steps * (MMPERSTEP);
}