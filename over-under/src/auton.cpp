#include "main.h"
#include "global_defs.h"
#include "pros/imu.hpp"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include <cmath>
#include <string>
#include <math.h>

using namespace pros;

const int circum = radius*2*M_PI;

Imu imu(IMU);
Motor lf(TOP_LEFT_DRIVE, -1);
Motor lm(MID_LEFT_DRIVE);
Motor lb(BOT_LEFT_DRIVE, -1);

Motor rf(TOP_RIGHT_DRIVE);
Motor rm(MID_RIGHT_DRIVE, -1);
Motor rb(BOT_RIGHT_DRIVE);

void reset_positions(){
    lf.set_zero_position(0);
    lm.set_zero_position(0);
    lb.set_zero_position(0);
    rf.set_zero_position(0);
    rm.set_zero_position(0);
    rb.set_zero_position(0);
    return;
}

void set_all_velocity(int voltage){
    lf = voltage;
    lm = voltage;
    lb = voltage;
    rf = voltage;
    rm = voltage;
    rb = voltage;
    return;
}

void set_left_velocity(int voltage){
    lf = voltage;
    lm = voltage;
    lb = voltage;
    return;
}

void set_right_velocity(int voltage){
    rf = voltage;
    rm = voltage;
    rb = voltage;
    return;
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
    float avg_error = encoder_units - (lf.get_position() + lm.get_position() + lb.get_position() + rf.get_position() + rm.get_position() + rb.get_position()) / 6.0;
    while(avg_error > 0){
        avg_error = encoder_units - (lf.get_position() + lm.get_position() + lb.get_position() + rf.get_position() + rm.get_position() + rb.get_position()) / 6.0;
<<<<<<< HEAD
        float normalized_error = avg_error / 2400;
        set_all_velocity((int) (normalized_error*p*127));
    }
    return;
}


//Turns the robot to the absolute given heading
void turn_absolute_proportional(int degrees, float p){
    int start_heading = imu.get_heading(); 
    //logic to get relative turn, and whether turning left or right is quicker
    int x = 0;
    turn_absolute_proportional(0, p);
    return;
}

void turn_right_relative(int degrees, float p){
    return;
}

void turn_left_relative(int degrees, float p){
    return;
=======
        float normalized_error = avg_error / 3000;
        set_all_velocity((int) (normalized_error*p*127));
    }
>>>>>>> parent of cd4e77e (Will this fix?)
}