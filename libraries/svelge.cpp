# include "svelge.h"

Svelge::Svelge() :
// usikker hvordan initialisering av listene skal gjøres
  piezo_reading{0},
  s_index{0},
  p_index{0},
  time_now{0},
  piezo{0},
  time_values{0},
  gyro_values{0},
  update{0}
{
  pinMode(PIEZO_SENSOR, INPUT);
}


void Svelge::loop(sensors_event_t accel, sensors_event_t gyro, sensors_event_t temp) {
  a = accel; g = gyro; t = temp;              // oppdatering av gyroskopvariable
  
  if (update) { update = 0; }

  piezo_reading = analogRead(PIEZO_SENSOR);
  append_data();
  detect_swallows();
}

void Svelge::append_data(){
  if (p_index >= ARRAY_LENGTH-1) {
    p_index = 0;
  }
  // Append data to the arrays
  piezo[p_index] = piezo_reading;
  time_values[p_index] = millis()/1000;
  gyro_values[p_index] = g.gyro.y; 
  p_index++; // Increment the index tracker
}

void Svelge::detect_swallows() {
  if (piezo[p_index] > PIEZO_THRESHOLD && abs(gyro_values[p_index]) < GYRO_THRESHOLD){
    if(s_index == 0){
      swallow_times[s_index] = time_values[p_index];
      s_index++;
      update = 1;
    }
    //check if the time or the time one second before the time we want to append is already in swallow_times
    else if ((time_values[p_index] - swallow_times[s_index-1]) > 1){
      swallow_times[s_index] = time_values[p_index];
      s_index++;
      update = 1;
    }
  }
}

unsigned long Svelge::send_data (){
  return swallow_times[s_index];
}
