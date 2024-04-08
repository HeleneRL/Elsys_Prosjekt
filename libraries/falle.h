// Dokumentasjon i github:      https://github.com/adafruit/Adafruit_MPU6050
// klassebeskrivelse og opcode: https://adafruit.github.io/Adafruit_MPU6050/html/class_adafruit___m_p_u6050.html
// referanse kode:              https://maker.pro/arduino/tutorial/how-to-build-a-fall-detector-with-arduino

# ifndef FALLE
# define FALLE

# include "libraries.h"
# define RESET_BUTTON_PIN 4
# define LED_BUILTIN 2 

class Falle 
{
    private:
        sensors_event_t a;                      // Fresh values from gyroscope
        sensors_event_t g;
        sensors_event_t t;
        sensors_event_t angle_acc1;
        sensors_event_t angle_acc2;
        const int lower_threshold = 7;          // 0.5g m/s**2
        const int upper_threshold = 10;         // 1g   m/s**2
        const int rotation_threshold = 50;      // 50 grader rotering fra vertikal akse
        
        bool fall;                          // Variabel for fall er detektert eller ikke
        float abs_accel;
        float abs_gyro;
        bool reset;                         // Tilbakestillingsknapp
        bool trigger1;                      // Fall i akselerasjonsverdier
        bool trigger2;                      // økning tilbake til normal av akselerasjonsverdier
        bool trigger3;                      // Rotering av personen i forhold til orientering før fall
        int trigger1count;
        int trigger2count;
        int trigger3count;

        float abs_acceleration ();
        float abs_gyroscope ();
        void handel_trigger1 ();
        void handel_trigger2 ();
        void reset_values();
        void read_reset_button();


    public:
        bool update;
        
        Falle();
        void loop(sensors_event_t accel, sensors_event_t gyro, sensors_event_t temp);
        bool get_fall() { return fall; };
};

# endif