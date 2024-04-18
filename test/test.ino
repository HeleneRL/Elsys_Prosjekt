#include "falle.h"
#include "ligge.h"
#include "svelge.h"
#include "libraries.h"


// deklarasjon av globale variable
sensors_event_t a, g, t;
Adafruit_MPU6050 mpu;
Ligge l;
Svelge s;
Falle f;
WiFiClient client;

// WiFi
const char* SSID = "Arduino";
const char* password = "12345678";
const char* serverAddress = "192.168.104.232";            // må oppdateres
const int serverPort = 5000;                            // Change to your server's port

String data;

int sample_period = 100;
int timer = 0;

void setup() {
  //starting terminal for testing
  Serial.begin(115200);
  WiFi.begin(SSID, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  delay(2000);
  Serial.println("Looking for MPU6050...");

  //initializing, error if not
  while (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip!");
  }

  //range of accelerometer
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);

  //range of gyroscope
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);

  //bandwith of digital anti aliasing low pass filter
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  pinMode(13, OUTPUT); // Built-in LED
  digitalWrite(13, HIGH);
  Serial.println("Setup complete!");
}



void loop () {
    // oppdaterer variable fra gyroskop
    /* if(millis() >= timer) { */
      mpu.getEvent(&a, &g, &t);

      s.test_loop(a, g ,t); 
      send_test_data(s, client);/* 
      timer = millis() + sample_period;
    } */
  
}



// data som skal sendes
void send_data(Svelge svelge, Falle falle, Ligge ligge, WiFiClient client) {
  if (svelge.update || falle.update || ligge.update_alarm || ligge.update_pos) {
    if (client.connect(serverAddress, serverPort)) {
      data = "data=";

      data += String (svelge.update) + ',' + String (svelge.send_data()) + ',';            // svelgedata
      data += String (ligge.update_alarm) + ',' + String (ligge.get_alarm()) + ',';        // liggedata
      data += String (ligge.update_pos) + ',' + String (ligge.get_current_pos()) + ',';
      data += String (ligge.get_current_pos_ts()) + ',';
      data += String (falle.update) + ',' + String (falle.get_fall());               // falledata

      client.println ("POST /receiver_path HTTP/1.1");
      client.println ("Host: " + String(serverAddress) + ":" + String(serverPort));
      client.println ("Content-Type: application/x-www-form-urlencoded");
      client.println ("Content-Length: " + String(data.length()));
      client.println ();
      client.println (data);
      client.stop();
      
      Serial.println("Data packet sent");
    }
    else {
      Serial.println("Could not connect to server! Data not sent");
    }
  }
}

void send_test_data(Svelge svelge, WiFiClient client) {
  if (1) {
    if (client.connect(serverAddress, serverPort)) {
      data = "data=";

      data += String (svelge.get_piezo()) + ',' + String (svelge.get_gyro_x()) + ',' + String (svelge.get_gyro_y()) + ',' + String (svelge.get_gyro_z()) + ',' + String(svelge.get_ts());            // svelgedata

      client.println ("POST /receiver_path HTTP/1.1");
      client.println ("Host: " + String(serverAddress) + ":" + String(serverPort));
      client.println ("Content-Type: application/x-www-form-urlencoded");
      client.println ("Content-Length: " + String(data.length()));
      client.println ();
      client.println (data);
      client.stop();
      
      Serial.println("Data packet sent");
    }
    else {
      Serial.println("Could not connect to server! Data not sent");
    }
  }
}