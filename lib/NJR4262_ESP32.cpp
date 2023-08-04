#include "NJR4262_ESP32.h"

// initialize
NJR4262_ESP32::NJR4262_ESP32() {
  iOffset = ICH_OFFSET, qOffset = QCH_OFFSET;
}

// main code for NJR4262_ESP32
void NJR4262_ESP32::updateVariable(int updateIntervalMsec = DELTA_TIME) {
  updateIQ();
  updateVector();
  calcvelocityAbs();
  calcPosition(updateIntervalMsec);
  calcPositionLPF(updateIntervalMsec);
}

// Update I/Q ch
// end():now 1:now-1 begin():now-2
void NJR4262_ESP32::updateIQ(int samplingNum = SAMPLING_NUM) {
  std::vector<float>::iterator i = ich->begin(), q = qch->begin();
  for(std::size_t cnt = 0; cnt < ich->size() - 1;cnt++) {
    *i = *(i+1);
    *q = *(q+1);
    i++, q++;
  }
  *i = getAdcVoltage(ICH_PIN, samplingNum);
  *q = getAdcVoltage(QCH_PIN, samplingNum);
}

// Update vector from I/Q ch
void NJR4262_ESP32::updateVector() {
  std::vector<float>::iterator velo = velocity->begin(),
  cent = center ->begin(), i = ich->begin(), q = qch->begin();
  *velo = *(q+2) - *(q+1);
  *(velo+1) = *(i+2) - *(i+1);
  *cent = *(q+2) - qOffset;
  *(cent+1) = *(i+2) - iOffset;
}

// Calculate velocityAbs from cross product
void NJR4262_ESP32::calcvelocityAbs() {
  std::vector<float>::iterator velo = velocity->begin(), cent = center ->begin();
  float crossProduct = *velo * *(cent+1) - *(velo+1) * *cent;
  velocityAbs = sqrt(*velo * *velo + *(velo+1) * *(velo+1));
  velocityAbs = (crossProduct < 0) ? (-1.0)*velocityAbs : velocityAbs;
}

// Calculate positoin by integral
void NJR4262_ESP32::calcPosition(int updateIntervalMsec) {
  position += velocityAbs * updateIntervalMsec;
}

// Calculate positionLPF by RC software
// ref:https://haizairenmei.com/2018/10/27/arduino_noise/
void NJR4262_ESP32::calcPositionLPF(int updateIntervalMsec) {
  float a = 1.0 / (2.0*PI*CUT_OFF_FREQ*updateIntervalMsec*0.001 + 1.0);
  positionLPF = a*positionLPF + (1.0 - a)*position;
}

// Get ADC Voltage by sampling
float NJR4262_ESP32::getAdcVoltage(int analogPin, int samplingNum = SAMPLING_NUM) {
  float voltage = 0.0;
  for(int i = 0; i < samplingNum; i++) {
    voltage += convertAdcCountToVoltage(analogRead(analogPin));
  }
  return voltage / (float)samplingNum;
}

// Get voltage from adc value
float NJR4262_ESP32::convertAdcCountToVoltage(int adcCount) {
  return (float)adcCount * ESP32_ADC_VREF / ESP32_ADC_MAX;
}

// Getter
float NJR4262_ESP32::getPosition() {
  return position;
}

float NJR4262_ESP32::getpositionLPF() {
  return positionLPF;
}

float NJR4262_ESP32::getvelocityAbs() {
  return velocityAbs;
}
