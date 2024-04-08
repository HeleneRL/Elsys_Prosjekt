# include "falle.h"

Falle::Falle () : 
        fall {0},                          // Initialiseringsliste
        abs_accel {0},
        abs_gyro {0},
        reset {0},                     
        trigger1 {0},                  
        trigger2 {0},                     
        trigger3 {0},                    
        trigger1count {0},
        trigger2count {0},
        trigger3count {0},
        update {0}
{
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
}

void Falle::loop(sensors_event_t accel, sensors_event_t gyro, sensors_event_t temp) {
  a = accel; g = gyro; t = temp;
  if (update) { update = 0; }

  if (abs_acceleration() <= lower_threshold) {
    trigger1 = 1;
  }

  if (trigger1) {
    handel_trigger1();
  }

  if (trigger2) {
    handel_trigger2();
  }

  if (trigger3) {
    fall = 1;
  }

  if (fall) {
    update = 1;
    read_reset_button();
    if (reset){
      reset_values();
    }
  }
}

float Falle::abs_acceleration (){
  return sqrt(pow(a.acceleration.x,2) + pow(a.acceleration.y,2) + pow(a.acceleration.z,2));
}

float Falle::abs_gyroscope () {
  return sqrt(pow(g.gyro.x,2) + pow(g.gyro.y,2) + pow(g.gyro.z,2));
}
 
void Falle::handel_trigger1(){
    if (trigger1count <= 0){
      abs_gyro = abs_gyroscope();
      angle_acc1 = g;
    }
    trigger1count ++;
    // Timer ut trigger1 etter  sykluser (1.4 sekund) etter fall i akselerasjonsverdi
    if (trigger1count > 10) {
      trigger1 = 0;
      trigger1count = 0;
    }
    // ellers skjer dette:
    else if (trigger1count == 5 && abs_acceleration() >= upper_threshold){
      angle_acc2 = g;
      trigger2 = 1;
    }
}

void Falle::handel_trigger2 () {
  trigger2count ++;
  // Timer ut etter 3 sykluser (0.6 sekunder) etter at personen har truffet bakken
  if (trigger2count > 12) {
    trigger2 = 0;
    trigger2count = 0;
  }
  else if (trigger2count < 12 && abs_gyro >= 3 && abs_gyro <= 400) //abs_gyroscope i seg selv er antagelig ikke nokk i dette tilfellet.
  {
    trigger3 = 1;
  }
}


//Tar inn bool fra reset knapp, eller alternativt time out basert reset.
void Falle::reset_values(){
  trigger1 = 0;    
  trigger2 = 0;   
  trigger3 = 0;    
  trigger1count = 0;
  trigger2count = 0;
  trigger3count = 0;
  fall = 0;
  reset = 0;
  abs_gyro = 0;
  abs_accel = 0;
}

void Falle::read_reset_button(){
  reset = !digitalRead(RESET_BUTTON_PIN);
}