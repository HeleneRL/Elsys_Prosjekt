
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
sensors_event_t accel, gyro, temp;

int current_pos = 0;                   //current position: right = 1, left = 2, back = 3, sitting = 4, other = 0
int current_pos_ts;                    //timestamp for time in current position
constexpr int time_limit = 120000;     // 120 000 millisec - 2 min 
 


int check_current_pos (sensors_event_t a) {
  if      (right_side(a)) { return 1; }
  else if (left_side(a))  { return 2; }
  else if (back_side(a))  { return 3; }
  else if (sitting(a))    { return 4; }
  else                    { return 0; }
}

// true if pasient is laying on this side
bool right_side (sensors_event_t a) {
  if (a.acceleration.x >= -8    && a.acceleration.x <= -2   &&
      a.acceleration.y >= -8.5  && a.acceleration.y <= -2.5 &&
      a.acceleration.z >= 2.5   && a.acceleration.z <= 8.5) { return 1; }
  else { return 0; }
}

bool left_side (sensors_event_t a){
  if (a.acceleration.x >= 2.5   && a.acceleration.x <= 8.5   &&
      a.acceleration.y >= -5    && a.acceleration.y <= 1    &&
      a.acceleration.z >= -12   && a.acceleration.z <= -6) { return 1; }
  else { return 0; }
}

bool back_side (sensors_event_t a){ 
  if (a.acceleration.x >= 3     && a.acceleration.x <= 9    &&
      a.acceleration.y >= -11    && a.acceleration.y <= -4.5 &&
      a.acceleration.z >= -0.5   && a.acceleration.z <= 5.5) { return 1; }
  else { return 0; }
}

bool sitting (sensors_event_t a){
  if (a.acceleration.x >= -2.5  && a.acceleration.x <= 4    &&
      a.acceleration.y >= -13   && a.acceleration.y <= -7   &&
      a.acceleration.z >= -5     && a.acceleration.z <= 2) { return 1; }
  else { return 0; }
}

//send functions for position and alarm
void send_pos_laying() {
  Serial.print("pos "); Serial.println(current_pos);
}

void send_alarm_laying() {
  Serial.println(1);
}

//
void setup() {

  //starting terminal for testing
  Serial.begin(115200);
  while (!Serial){
    delay(10);
  }
  Serial.print(F("Adafruit MPU6050 test!"));

  //initializing, error if not
  mpu.begin();
  if (!mpu.begin()) {
      Serial.print(F("Failed to find MPU6050 chip"));
      }
  

  //range of accelerometer
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);

  //range of gyroscope
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  //bandwith of digital anti aliasing low pass filter
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}

void loop() {
  mpu.getEvent(&accel, &gyro, &temp);

  
  //checking if pasient has switched position
  int new_pos = check_current_pos(accel);
  if (current_pos != new_pos) {
    current_pos = new_pos;
    current_pos_ts = millis();
    send_pos_laying();
  }
  //sending alarm if pasient has layed too long on same side
  else {
    if ((millis() - current_pos_ts) > time_limit) {
      current_pos_ts = 0;
      send_alarm_laying();
    }
  }
}
