#include "falle.h"
#include "ligge.h"
#include "svelge.h"
#include "libraries.h"


// deklarasjon av globale variable
sensors_event_t a, g, t;
Adafruit_MPU6050 mpu;
Svelge s;

String data;

int sample_rate = 100;
int period_ms = 1000/100;
int timer = 0;

void setup() {
  //starting terminal for testing
  Serial.begin(115200);

  //range of accelerometer
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);

  //range of gyroscope
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  //bandwith of digital anti aliasing low pass filter
  mpu.setFilterBandwidth(MPU6050_BAND_260_HZ);

  pinMode(13, OUTPUT); // Built-in LED
  digitalWrite(13, HIGH);
}



void loop () {
    // oppdaterer variable fra gyroskop
    if(millis() >= timer) {
      mpu.getEvent(&a, &g, &t);

      s.test_loop(a, g ,t);
      data = String(s.get_piezo()) + ',' + String(s.get_gyro()) + ',' + String(s.get_ts());
      Serial.println(data);
      timer = millis() + period_ms;
    }
  
}