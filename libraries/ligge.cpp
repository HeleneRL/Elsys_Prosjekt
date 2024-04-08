# include "ligge.h"

Ligge::Ligge () : 
  current_pos{0}, 
  current_pos_ts{millis()},
  update_pos{0}, 
  alarm{0},
  update_alarm{0}
{}

//checking if pasient has switched position
void Ligge::loop(sensors_event_t accel, sensors_event_t gyro, sensors_event_t temp) {
  a = accel; g = gyro; t = temp;    // oppdatering av nye verdier fra gyroskop
  if (update_alarm || update_pos) { update_alarm = 0;  update_pos = 0;}


  int new_pos = check_current_pos();
  if (current_pos != new_pos) {
    alarm = 0;                          // Turns off alarm if patient is laying in new position.
    update_pos = 1;
    current_pos = new_pos;
    current_pos_ts = millis();
  }
  //sending alarm if pasient has layed too long on same side
  else {
    if ((millis() - current_pos_ts) > time_limit) {
      alarm = 1;
      update_alarm = 1;
      current_pos_ts = 0;
    }
  }
}

int Ligge::check_current_pos () {
  if      (right_side()) { return ligge_pos::right_side; }
  else if (left_side())  { return ligge_pos::left_side; }
  else if (back_side())  { return ligge_pos::back_side; }
  else if (sitting())    { return ligge_pos::sitting; }
  else                   { return ligge_pos::other; }
}

// true if pasient is laying on this side
bool Ligge::right_side () {
  if (a.acceleration.x >= -8    && a.acceleration.x <= -2   &&
      a.acceleration.y >= -8.5  && a.acceleration.y <= -2.5 &&
      a.acceleration.z >= 2.5   && a.acceleration.z <= 8.5) { return 1; }
  else { return 0; }
}

bool Ligge::left_side () {
  if (a.acceleration.x >= 2.5   && a.acceleration.x <= 8.5   &&
      a.acceleration.y >= -5    && a.acceleration.y <= 1    &&
      a.acceleration.z >= -12   && a.acceleration.z <= -6) { return 1; }
  else { return 0; }
}

bool Ligge::back_side (){ 
  if (a.acceleration.x >= 3     && a.acceleration.x <= 9    &&
      a.acceleration.y >= -11   && a.acceleration.y <= -4.5 &&
      a.acceleration.z >= -0.5  && a.acceleration.z <= 5.5) { return 1; }
  else { return 0; }
}

bool Ligge::sitting (){
  if (a.acceleration.x >= -2.5  && a.acceleration.x <= 4    &&
      a.acceleration.y >= -13   && a.acceleration.y <= -7   &&
      a.acceleration.z >= -5    && a.acceleration.z <= 2) { return 1; }
  else { return 0; }
}

