#include <Arduino.h>
#include <ArduinoSTL.h>

// Pin assgin
#define ICH_PIN A10
#define QCH_PIN A12
// ESP32 ADC 12bit VREF 3.3V
#define ESP32_ADC_MAX 4096.0
#define ESP32_ADC_VREF 3.3
#define SAMPLING_NUM 10
// Offset voltage for I/Q ch
#define ICH_OFFSET 1.6
#define QCH_OFFSET 1.6
// delta time[ms] = 1/fs
#define DELTA_TIME 20
// LPF fc[Hz]
#define CUT_OFF_FREQ 1.0

class NJR4262_ESP32 {
  private:
    // variable:vector & position
    std::vector<float> velocity[2], center[2];
    float position, positionLPF, velocityAbs;
    // variable:I/Q ch voltage
    std::vector<float> ich[3], qch[3];
    float iOffset, qOffset;
    // function: control adc
    float getAdcVoltage(int analogPin, int samplingNum);
    float convertAdcCountToVoltage(int adcCount);
    // function: update variable
    void updateIQ(int samplingNum);
    void updateVector();
    // function: calculate variable
    void calcvelocityAbs();
    void calcPosition(int updateIntervalMsec);
    void calcPositionLPF(int updateIntervalMsec);

  public:
    // function: initialize
    NJR4262_ESP32();
    // function: call in your loop()
    void updateVariable(int updateIntervalMsec = DELTA_TIME);
    // function: Getter
    float getPosition();
    float getpositionLPF();
    float getvelocityAbs();
};
