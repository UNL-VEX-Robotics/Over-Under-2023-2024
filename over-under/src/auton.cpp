#include "main.h"
#include "global_defs.h"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include <cmath>
#include <string>
#include <math.h>

using namespace pros;

const int circum = radius*2*M_PI;

Motor lf(TOP_LEFT_DRIVE, -1);
Motor lm(MID_LEFT_DRIVE);
Motor lb(BOT_LEFT_DRIVE, -1);

Motor rf(TOP_RIGHT_DRIVE);
Motor rm(MID_RIGHT_DRIVE, -1);
Motor rb(BOT_RIGHT_DRIVE);

void reset_positions(){
    std::cout << "settings positions to 0";
    std::cout << lf.set_zero_position(0);
    std::cout << lm.set_zero_position(0);
    std::cout << lb.set_zero_position(0);
    std::cout << rf.set_zero_position(0);
    std::cout << rm.set_zero_position(0);
    std::cout << rb.set_zero_position(0);
    return;
}

void set_all_velocity(int velocity){
    lf.move_velocity(velocity);
    lm.move_velocity(velocity);
    lb.move_velocity(velocity);
    rf.move_velocity(velocity);
    rm.move_velocity(velocity);
    rb.move_velocity(velocity);
}

void all_brake(){
    lf.brake();
    lm.brake();
    lb.brake();
    rf.brake();
    rm.brake();
    rb.brake();
}

//motor_encoder_units_e_t: 2 for stank bot, so ticks (you can change this but this is the most granular so is gud)
void move_distance_proportional(float inches, float p){
    double revolutions = inches / circum;
    double encoder_units = revolutions * blue_ticks_per_rev;
    reset_positions();
    float avg_error = encoder_units - (lf.get_position() + lm.get_position() + lb.get_position() + rf.get_position() + rm.get_position() + rb.get_position()) / 4.0;
    while(avg_error > 0){
        avg_error = encoder_units - (lf.get_position() + lm.get_position() + lb.get_position() + rf.get_position() + rm.get_position() + rb.get_position()) / 4.0;
        if(avg_error*p > 200){
            set_all_velocity(200);
        } else {
            set_all_velocity(avg_error*p);
        }
    }
}