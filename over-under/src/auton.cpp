#include "main.h"
#include "global_defs.h"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include <cmath>
#include <string>

using namespace pros;

const int circum = radius*2*M_PI;
//supposedly 900, but with testing appears to be twice
const int ticks_per_rev = 900*2;

Motor lf(left_front);
Motor lb(left_back);
Motor rf(right_front, -1);
Motor rb(right_back, -1);

void reset_positions(){
    std::cout << "settings positions to 0";
    std::cout << lf.set_zero_position(0);
    std::cout << lb.set_zero_position(0);
    std::cout << rf.set_zero_position(0);
    std::cout << rb.set_zero_position(0);
    return;
}

void set_all_velocity(int velocity){
    lf.move_velocity(velocity);
    lb.move_velocity(velocity);
    rf.move_velocity(velocity);
    rb.move_velocity(velocity);
}
void all_brake(){
    lf.brake();
    lb.brake();
    rf.brake();
    rb.brake();
}

//motor_encoder_units_e_t: 2 for stank bot, so ticks (you can change this but this is the most granular so is gud)
void move_distance_proportional(float inches, float p){
    double revolutions = inches / circum;
    double encoder_units = revolutions * ticks_per_rev;
    reset_positions();
    float avg_error = encoder_units - (lf.get_position() + lb.get_position() + rf.get_position() + rb.get_position()) / 4.0;
    while(avg_error > 0){
        avg_error = encoder_units - (lf.get_position() + lb.get_position() + rf.get_position() + rb.get_position()) / 4.0;
        if(avg_error*p > 200){
            set_all_velocity(200);
        } else {
            set_all_velocity(avg_error*p);
        }
    }

}