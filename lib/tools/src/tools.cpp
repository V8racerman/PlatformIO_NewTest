#include <tools.h>

const int meas_samples = 10;    // number of voltage measurement samples
const int meas_delay   = 80;    // time delay between measurement samples
const float k1_factor  = CORRECTION_FACTOR*VREF / RESOLUTION;                 // ADC correction factor (used in read_voltage() function)
const float k2_factor  = CORRECTION_FACTOR*VREF / (RESOLUTION*meas_samples);  // ADC correction factor (used in  get_voltage() function)

float read_voltage() {

  return  (float)analogRead(A0)*k1_factor + DC_OFFSET;
  // return  (float)analogRead(A0)*CORRECTION_FACTOR*VREF/RESOLUTION + DC_OFFSET;
}

float get_voltage(int pre_delay) {

  float volt;
  int SensorValue;
  int i;
  
  volt = 0.0;
  SensorValue = 0;
  delay(pre_delay);
  for ( i = 0; i < meas_samples; i++ )  { 
    SensorValue = SensorValue + analogRead(A0); 
    delay(meas_delay);
    }
  volt = (float)SensorValue*k2_factor + DC_OFFSET;
  return  volt;
}
