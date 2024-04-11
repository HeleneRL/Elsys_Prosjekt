# ifndef LIGGE
# define LIGGE
# include "libraries.h"

enum ligge_pos {
    other = 0,
    right_side = 1,
    left_side = 2,
    back_side = 3,
    sitting = 4
};

class Ligge
{
    private:
        sensors_event_t a;
        sensors_event_t g;
        sensors_event_t t;
        
        const int time_limit = 10 * (60 * 1000);         // 10 min (i ms)
        int current_pos;                       //current position: right = 1, left = 2, back = 3, sitting = 4, other = 0
        unsigned long current_pos_ts;                    //timestamp for time in current position
        bool alarm;

        int check_current_pos ();
        bool right_side ();
        bool left_side ();
        bool back_side ();
        bool sitting ();
        
    public:
        bool update_pos;
        bool update_alarm;
        
        Ligge ();
        void loop (sensors_event_t accel, sensors_event_t gyro, sensors_event_t temp);
        bool get_alarm () { return alarm; };
        int get_current_pos() { return current_pos; };
        unsigned long get_current_pos_ts() { return current_pos_ts; };
};

# endif