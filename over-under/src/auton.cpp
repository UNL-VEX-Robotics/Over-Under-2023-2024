#include "main.h"
#include "global_defs.h"
#include "pros/imu.h"
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

void set_all_voltage(int voltage){
    lf = voltage;
    lm = voltage;
    lb = voltage;
    rf = voltage;
    rm = voltage;
    rb = voltage;
    return;
}

void set_left_voltage(float voltage){
    lf = voltage;
    lm = voltage;
    lb = voltage;
    return;
}

void set_right_voltage(float voltage){
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
        float normalized_error = avg_error / 2400;
        set_all_voltage((int) (normalized_error*p*127));
    }
    return;
}

void move_distance_individual_sides(float inches, float p){
    double revolutions = inches / circum;
    double encoder_units = 10.0/6.0 * revolutions * blue_ticks_per_rev;
    reset_positions();
    int i = 0;
    double left_avg_error = encoder_units - (lf.get_position() + lm.get_position() + lb.get_position()) / 3.0;
    double right_avg_error = encoder_units - (rf.get_position() + rm.get_position() + rb.get_position()) / 3.0;
    while(( abs(left_avg_error) > 20 ) && (abs(right_avg_error) > 20 )){
        left_avg_error = encoder_units - (lf.get_position() + lm.get_position() + lb.get_position()) / 3.0;
        right_avg_error = encoder_units - (rf.get_position() + rm.get_position() + rb.get_position()) / 3.0;
        if(left_avg_error*p > 127){
            set_left_voltage(127);
        } else{
            set_left_voltage((left_avg_error * p));
        }
        if(right_avg_error*p > 127) {
            set_right_voltage(127);
        } else {
            set_right_voltage((right_avg_error * p));
        }
        if(i % 15000 == 0){
            std::cout << "\nleft: ";
            std::cout << left_avg_error;
            std::cout << "\nright: ";
            std::cout << right_avg_error;
        }
        i++;
    }
    std::cout << "\nend reached\n";    
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
    double end_heading = degrees + imu.get_heading();
    double error = degrees;
    if(end_heading >= 360){
        while(imu.get_heading() > 15){
            error = end_heading - imu.get_heading();
            if(error * p > 127){
                set_left_voltage(127);
                set_right_voltage(-127);
            } else {
                set_left_voltage(error*p);
                set_right_voltage(-error*p);
            }
        }
        end_heading-=360;
        error = end_heading - imu.get_heading();
        while(abs(error) > 5){
            error = end_heading - imu.get_heading();
            if(error * p > 127){
                set_left_voltage(127);
                set_right_voltage(-127);
            } else {
                set_left_voltage(error*p);
                set_right_voltage(-error*p);
            }
        }
        return;
    }
    
    while(abs(error) > 3){
        error = end_heading - imu.get_heading();
        if(error * p > 127){
            set_left_voltage(127);
            set_right_voltage(-127);
        } else {
            set_left_voltage(error*p);
            set_right_voltage(-error*p);
        }
    }
    return;
}

void turn_left_relative(int degrees, float p){
    double end_heading = imu.get_heading() - degrees;
    double error = degrees;
    if(end_heading < 0){
         while(imu.get_heading() < 345){
            error = imu.get_heading() - end_heading;
            if(error * p > 127){
                set_left_voltage(-127);
                set_right_voltage(127);
            } else {
                set_left_voltage(-error*p);
                set_right_voltage(error*p);
            }
        }
        end_heading+=360;
        error = imu.get_heading() - end_heading;
        while(abs(error) > 3){
            error = imu.get_heading() - end_heading;
            if(error * p > 127){
                set_left_voltage(-127);
                set_right_voltage(127);
            } else {
                set_left_voltage(-error*p);
                set_right_voltage(error*p);
            }
        }
        return;
    } else {
        while(abs(error) > 3){
        error = imu.get_heading()- end_heading;
        if(error * p > 127){
            set_left_voltage(-127);
            set_right_voltage(127);
        } else {
            set_left_voltage(-error*p);
            set_right_voltage(error*p);
        }
    }
    return;
    }
}