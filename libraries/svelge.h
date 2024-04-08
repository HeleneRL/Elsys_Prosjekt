//Author
//Denne koden sender data via internett
//den printer ingenting!! all serial er tatt ut
//Helene og Nikolai eier koden 
//Sist fungerende: 04.04 kl 15:05

# ifndef SVELGE
# define SVELGE

# include "libraries.h"

#define ARRAY_LENGTH 1000                               // Må trolig endres
#define PIEZO_SENSOR A0                                 // the piezo is connected to analog pin 0
#define PIEZO_THRESHOLD 3000                            //must be set through testing
#define GYRO_THRESHOLD 0.1                              //must be set through testing


class Svelge 
{
    private:
        sensors_event_t a;
        sensors_event_t g;
        sensors_event_t t;


// usikker hvordan initialisering av listene skal gjøres
        int piezo_reading;                              // variable to store the value read from the sensor pin
        unsigned long time_now;                         //setter nåværende tid til 0
        int piezo[ARRAY_LENGTH];                   //Making an array for Piezo values
        int p_index;                                    // Initialize the piezo index tracker
        unsigned long time_values[ARRAY_LENGTH];    // Define the time array
        float gyro_values[ARRAY_LENGTH];            //Make array for gyro values of rotation in y-axis
        unsigned long swallow_times[ARRAY_LENGTH];      //lag en array som skal holde alle svelgetidspunkt
        int s_index;                                    // Index for the swallow_times array

        // private funksjoner
        void append_data();
        void detect_swallows();

    public:
        Svelge();
        void loop(sensors_event_t accel, sensors_event_t gyro, sensors_event_t temp);
        unsigned long send_data();
        bool update;

};

# endif