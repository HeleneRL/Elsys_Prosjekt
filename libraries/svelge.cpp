# include "svelge.h"

Svelge::Svelge()
{
  pinMode(PIEZO_SENSOR, INPUT);
}

int Svelge::get_piezo() {
  return ((p_index == 0) ? 0 : piezo[p_index - 1]);
}

float Svelge::get_gyro_x() {
  return ((p_index == 0) ? 0 : gyro_values_x[p_index - 1]);
}

float Svelge::get_gyro_y() {
  return ((p_index == 0) ? 0 : gyro_values_y[p_index - 1]);
}

float Svelge::get_gyro_z() {
  return ((p_index == 0) ? 0 : gyro_values_z[p_index - 1]);
}

unsigned long Svelge::get_ts() {
  return ((p_index == 0) ? 0 : time_values[p_index - 1]);
}


void Svelge::loop(sensors_event_t gyro) {
  g = gyro;              // oppdatering av gyroskopvariable
  
  if (update) { update = 0; }

  piezo_reading = analogRead(PIEZO_SENSOR);
  append_data();
  detect_swallows();
}

void Svelge::test_loop(sensors_event_t gyro) {
  g = gyro;              // oppdatering av gyroskopvariable
  
  if (update) { update = 0; }

  piezo_reading = analogRead(PIEZO_SENSOR);
  append_data();
}

void Svelge::append_data(){
  if (p_index >= ARRAY_LENGTH-1) {
    p_index = 0;
  }
  // Append data to the arrays
  piezo[p_index] = piezo_reading;
  time_values[p_index] = millis();
  gyro_values_x[p_index] = g.gyro.x; 
  gyro_values_y[p_index] = g.gyro.y; 
  gyro_values_z[p_index] = g.gyro.z; 
  if(abs(gyro_values_x[p_index]) > GYRO_THRESHOLD_X
  || abs(gyro_values_y[p_index]) > GYRO_THRESHOLD_Y
  || abs(gyro_values_z[p_index]) > GYRO_THRESHOLD_Z) {
    gyro_to = time_values[p_index] + 50;     // 50ms timeout when gyro measures above threshold
  }
  p_index++; // Increment the index tracker
}

void Svelge::detect_swallows() {
  if ((piezo[p_index] > PIEZO_THRESHOLD) && (time_values[p_index] >= gyro_to)){
    if(s_index == 0){
      swallow_times[s_index++] = time_values[p_index];
      update = 1;
    }
    //check if the time or the time one second before the time we want to append is already in swallow_times
    else if ((time_values[p_index] - swallow_times[s_index-1]) > 2000){
      swallow_times[s_index++] = time_values[p_index];
      update = 1;
    }
  }
}

unsigned long Svelge::send_data (){
  return ((s_index == 0) ? 0 : swallow_times[s_index - 1]);
}