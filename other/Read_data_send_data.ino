//Author


#include <Adafruit_MPU6050.h> //library gyro
#include <Adafruit_Sensor.h> 
#include <Wire.h> 

#define ARRAY_LENGTH 1000 // Må trolig endres
#define PIEZO_SENSOR A0  // the piezo is connected to analog pin 0
#define PIEZO_THRESHOLD 3000 //must be set through testing
#define GYRO_THRESHOLD 0.5 //must be set through testing

//Making an array for Piezo values
int piezo[ARRAY_LENGTH] ={0};
int p_index = 0; // Initialize the piezo index tracker

// Define the time array
unsigned long time_values[ARRAY_LENGTH]={0};

//Make array for gyro values of rotation in y-axis
float gyro_values[ARRAY_LENGTH]={0};


//lag en array som skal holde alle svelgetidspunkt
unsigned long swallow_times[ARRAY_LENGTH];
int s_index = 0; // Index for the swallow_times array

//setter nåværende tid til 0
unsigned long time_now = 0;

//variabel for å holde piezodata
int piezo_reading = 0;  // variable to store the value read from the sensor pin

//oppretter objekt for MPU
Adafruit_MPU6050 mpu;
sensors_event_t a, g, temp;

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  // Try to initialize!
  while(!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
      delay(10);
  }
  Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}



void loop() {

  /* Get new sensor events by reading the mpu */
  mpu.getEvent(&a, &g, &temp);

  //reads the piezo sensor
  piezo_reading = analogRead(PIEZO_SENSOR);
  append_data();
  detect_swallows();
  
}






void append_data(){
  if (p_index >= ARRAY_LENGTH-1) {
    p_index = 0;
  }
  // Append data to the arrays
  piezo[p_index] = piezo_reading;
  time_values[p_index] = millis()/1000;
  gyro_values[p_index] = g.gyro.y; 
  p_index++; // Increment the index tracker
}

void detect_swallows() {
  if (piezo[p_index] > PIEZO_THRESHOLD && abs(gyro_values[p_index]) < GYRO_THRESHOLD){
    if(s_index == 0){
      swallow_times[s_index] = time_values[p_index];
      send_swallow_data();
      s_index++;
    }
    //check if the time or the time one second before the time we want to append is already in swallow_times
    else if ((time_values[p_index] - swallow_times[s_index-1]) > 1){
      swallow_times[s_index] = time_values[p_index];
      send_swallow_data();
      s_index++;
    }
  }
}

void send_swallow_data(){
  Serial.println(swallow_times[s_index]);
}

