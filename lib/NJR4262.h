#include <Arduino.h>
#include <ArduinoSTL.h>

// Pin assgin
#define ICH_PIN A0
#define QCH_PIN A1
// Offset voltage for I/Q ch
#define ICH_OFFSET 1.6
#define QCH_OFFSET 1.6
// delta time[ms] = 1/fs
#define DELTA_TIME 20.0
// LPF fc[Hz]
#define CUT_OFF_FREQ 1.0

class NJR4262 {
  private:
    // variable:vector & position
    std::vector<float> velocity[2], center[2];
    float pos, posFiltered;
    // variable:I/Q ch voltage
    std::vector<float> ich[3], qch[3];
    float iOffset, qOffset;
    // function: UpdateIO
    void updateIQ(int);
    void updateVector();
    // function: UpdateVariable
    void calcVelocity();

  public:
    void updateVariable();
};

// function
void updateIQ(int);
void updateVector();
float calcVelocity();
void calcLPF();
void calcOffset();
float adcToVoltage(int);
float getAdcVoltage(int, int);

void setup() {
  Serial.begin(9600);
  delay(30);
  updateIQ(10);
}

void loop() {
  updateIQ(10);
  updateVector();
  // integral velocity -> position
  position += calcVelocity()*DELTA_TIME;
  calcLPF();
  // plot to teleplot
  //Serial.print(">position:");
  //Serial.println(position);
  Serial.print(">Respiratory Waveform:");
  Serial.println(positoinLPF);
}

// Update I/Q ch
// 2->now 1->now-1 0->now-2
void updateIQ(int samplingNum) {
  for(int i = 0; i < 2;i++) {
    ich[i] = ich[i+1];
    qch[i] = qch[i+1];
  }
  ich[2] = getAdcVoltage(ICH_PIN, samplingNum);
  qch[2] = getAdcVoltage(QCH_PIN, samplingNum);
  delay(DELTA_TIME);
}

// Update vector from I/Q ch
void updateVector() {
  velocityVec[0] = qch[2] - qch[1];
  velocityVec[1] = ich[2] - ich[1];
  centerVec[0] = qch[2] - QCH_OFFSET;
  centerVec[1] = ich[2] - ICH_OFFSET;
}

// Calculate velocity from cross product
float calcVelocity() {
  float crossProduct = velocityVec[0]*centerVec[1] - velocityVec[1]*centerVec[0];
  float velocity = sqrt(velocityVec[0]*velocityVec[0] + velocityVec[1]*velocityVec[1]);
  velocity = (crossProduct < 0) ? (-1.0)*velocity : velocity;
  return velocity;
}

// Calculate by RC LPF
// ref:https://haizairenmei.com/2018/10/27/arduino_noise/
void calcLPF() {
  float a = 1.0 / (2.0*PI*CUT_OFF_FREQ*DELTA_TIME*0.001 + 1.0);
  positoinLPF = a*positoinLPF + (1.0 - a)*position;
}

// Calculate offset from I/Q ch
// I don't use this function
// Offset is located at center of I/Q ch circle
// ref:https://qiita.com/tydesign/items/2fd456f40f5aeeb461ff
void calcOffset() {
  float c1 = 2*(qch[0]-qch[1])*(ich[0]-ich[2])-2*(qch[0]-qch[2])*(ich[0]-ich[1]);
  float c2 = qch[2]*qch[2]-qch[0]*qch[0]+ich[2]*ich[2]-ich[0]*ich[0];
  float c3 = qch[1]*qch[1]-qch[0]*qch[0]+ich[1]*ich[1]-ich[0]*ich[0];
  iOffset = ((ich[0]-ich[1])*c2-(ich[0]-ich[2])*c3)/c1;
  qOffset = ((qch[0]-qch[2])*c3-(qch[0]-qch[1])*c2)/c1;
}

// Get ADC Voltage by sampling
float getAdcVoltage(int analogPin, int samplingNum) {
  float voltage = 0.0;
  for(int i = 0; i < samplingNum; i++) {
    voltage += adcToVoltage(analogRead(analogPin));
  }
  return voltage / (float)samplingNum;
}

// Get voltage from adc value
// ADC 10bit VREF 5V
float adcToVoltage(int adcCount) {
  return (float)adcCount * 5.0 / 1024.0;
}

