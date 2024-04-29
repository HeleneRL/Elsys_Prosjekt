//Author
//Denne koden sender data via internett
//den printer ingenting!! all serial er tatt ut
//Helene og Nikolai eier koden 
//Sist fungerende: 04.04 kl 15:05

# ifndef SVELGE
# define SVELGE

# include "libraries.h"

#define ARRAY_LENGTH 100                              // Må trolig endres
#define PIEZO_SENSOR A0                                 // the piezo is connected to analog pin 0
#define PIEZO_THRESHOLD 2000                           //must be set through testing
#define GYRO_THRESHOLD_X 0.5                              //must be set through testing
#define GYRO_THRESHOLD_Y 0.2                              //must be set through testing
#define GYRO_THRESHOLD_Z 0.9                              //must be set through testing


class Svelge 
{
    private:
        sensors_event_t a;
        sensors_event_t g;
        sensors_event_t t;

        int piezo_reading;                              // variable to store the value read from the sensor pin
        unsigned long time_now;                         //setter nåværende tid til 0
        int piezo[ARRAY_LENGTH] = {0};                   //Making an array for Piezo values
        int p_index;                                    // Initialize the piezo index tracker
        unsigned long time_values[ARRAY_LENGTH] = {0};    // Define the time array
        float gyro_values_x[ARRAY_LENGTH] = {0};            //Make array for gyro values of rotation in y-axis
        float gyro_values_y[ARRAY_LENGTH] = {0};            //Make array for gyro values of rotation in y-axis
        float gyro_values_z[ARRAY_LENGTH] = {0};            //Make array for gyro values of rotation in y-axis
        unsigned long swallow_times[ARRAY_LENGTH] = {0};      //lag en array som skal holde alle svelgetidspunkt
        int s_index;                                    // Index for the swallow_times array
        unsigned long gyro_to = 0;                          // Timeout for gyro. When gyro goes above threshold, do not detect swallows

        void append_data();
        void detect_swallows();

    public:
        Svelge();
        void loop(sensors_event_t accel, sensors_event_t gyro, sensors_event_t temp);
        void test_loop(sensors_event_t accel, sensors_event_t gyro, sensors_event_t temp);
        unsigned long send_data();
        bool update;
        int get_piezo();
        float get_gyro_x();
        float get_gyro_y();
        float get_gyro_z();
        unsigned long get_ts();
        

};

# endif
