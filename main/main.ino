#include "libraries.h"
#include "svelge.h"
#include "ligge.h"
#include "falle.h"


// Skru av og på serial debugging. Må skrus av når arduino ikke får strøm fra en PC
#define SERIAL

// deklarasjon av globale variable
sensors_event_t a, g, t;
Adafruit_MPU6050 mpu;
Ligge ligge;
Svelge svelge;
Falle falle;
WiFiClient client;

const char* password = "12345678";
const char* serverAddress = "172.20.10.12";            // må oppdateres
const int serverPort = 5000;                            // Change to your server's port

int error;
String data;

void setup() {
  //starting terminal for testing
  #ifdef SERIAL //--------------------------------------
  Serial.begin(115200);
  while (!Serial){
    delay(10);
  }
  Serial.println(F("Looking for MPU6050"));

  //initializing, error if not
  if (!mpu.begin()) {
    Serial.println(F("Failed to find MPU6050 chip"));
  }
  #else //----------------------------------------------
  mpu.begin();
  #endif //---------------------------------------------

  //range of accelerometer
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);

  //range of gyroscope
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  //bandwith of digital anti aliasing low pass filter
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
}



void loop () {
    // oppdaterer variable fra gyroskop
    mpu.getEvent(&a, &g, &t);
    ligge.loop(a, g, t);
    svelge.loop(a, g ,t);
    falle.loop(a, g, t);
    error = send_data(svelge, falle, ligge, client);
    #ifdef SERIAL //--------------------------------------
    if(error) {
      Serial.println("Could not connect to PC! Data not sent");
    }
    #endif //---------------------------------------------
    
}


// data som skal sendes
int send_data(Svelge svelge, Falle falle, Ligge ligge, WiFiClient client) {
  if (svelge.update || falle.update || ligge.update_alarm || ligge.update_pos) {
    if (client.connect(serverAddress, serverPort)) {
      data = "data=";

      data += String (svelge.update) + ',' + String (svelge.send_data()) + ',';            // svelgedata
      data += String (ligge.update_alarm) + ',' + String (ligge.get_alarm()) + ',';        // ligge data
      data += String (ligge.update_pos) + ',' + String (ligge.get_current_pos()) + ',';
      data += String (ligge.get_current_pos_ts()) + ',';
      data += String (falle.update) + ',' + String (falle.get_fall()) + ',';               // falle data

      client.println ("POST /receiver_path HTTP/1.1");
      client.println ("Host: " + String(serverAddress) + ":" + String(serverPort));
      client.println ("Content-Type: application/x-www-form-urlencoded");
      client.println ("Content-Length: " + String(data.length()));
      client.println ();
      client.println (data);
      client.stop();
      
      return 0;
    }
    return 1;
  }
  return 0;
}
