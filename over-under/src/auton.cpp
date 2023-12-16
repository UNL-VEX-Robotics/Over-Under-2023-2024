#include "display/lv_core/lv_obj.h"
#include "main.h"
#include "global_defs_v1.h"
#include "pros/imu.h"
#include "pros/imu.hpp"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "auton.h"
#include "pros/rtos.h"
#include <cmath>
#include <string>
#include <math.h>

using namespace pros;

const int circum = radius*2*M_PI;

Imu imu(IMU);
Motor lf(TOP_LEFT_DRIVE, true);
Motor lm(MID_LEFT_DRIVE);
Motor lb(BOT_LEFT_DRIVE, true);

Motor rf(TOP_RIGHT_DRIVE);
Motor rm(MID_RIGHT_DRIVE, true);
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


void move_individual_sides_debug(float inches, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd){
    double revolutions = inches / circum;
    double encoder_units = 10.0/6.0 * revolutions * blue_ticks_per_rev;
    reset_positions();
    int i = 0;
    double left_error = encoder_units - (lf.get_position() + lm.get_position() + lb.get_position()) / 3.0;
    double right_error = encoder_units - (rf.get_position() + rm.get_position() + rb.get_position()) / 3.0;
    double prev_left_error = left_error;
    double prev_right_error=right_error;
    double left_integral = 0;
    double right_integral = 0;
    double left_derivative = 0;
    double right_derivative = 0;

    double left_voltage = 0;
    double right_voltage = 0;
    while(( abs(left_error) > settled_margin ) && (abs(right_error) > settled_margin )){
        //TODO: maintain linked list of n previous errors, if std deviation is below 10, exit with error code. 
        //In auton route, if error code is returned, go some kind of reset.
        prev_left_error = left_error;
        prev_right_error = right_error;
        left_error = encoder_units - (lf.get_position() + lm.get_position() + lb.get_position()) / 3.0;
        right_error = encoder_units - (rf.get_position() + rm.get_position() + rb.get_position()) / 3.0;
        
        //if each iteration of the loop doesn't take a uniform amount of time, derivative and integral calculations need
        //to consider time
        left_derivative = left_error - prev_left_error;
        right_derivative = right_error - prev_right_error;

        if(abs(left_error) > integral_max_error){
            left_integral = 0;    
        } else {
            left_integral = left_integral + Ki * left_error;
        }
        if(abs(right_error) > integral_max_error){
            right_integral = 0;
        } else {
            right_integral = right_integral + Ki * right_error;
        }
        
        left_voltage = Kp * left_error + Ki * left_integral + Kd * left_derivative;
        right_voltage = Kp * right_error + Ki * right_integral + Kd * right_derivative;
        if(left_voltage > 127){
            set_left_voltage(127.0 * (i+100)/25000);
        } else{
            set_left_voltage(left_voltage);
        }
        if(right_voltage > 127) {
            set_right_voltage(127.0 * (i+100)/25000);
        } else {
            set_right_voltage(right_voltage);
        }

        if(i % 15000 == 0 && i < 120000){
            std::cout << "\n   left: ";
            std::cout << left_error;
            std::cout << "\n   right: ";
            std::cout << right_error;
        }
        i++;
    }     
    return;
}

void move_individual_sides(float inches, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd){
    double revolutions = inches / circum;
    double encoder_units = 10.0/6.0 * revolutions * blue_ticks_per_rev;
    reset_positions();
    double left_error = encoder_units - (lf.get_position() + lm.get_position() + lb.get_position()) / 3.0;
    double right_error = encoder_units - (rf.get_position() + rm.get_position() + rb.get_position()) / 3.0;
    double prev_left_error = left_error;
    double prev_right_error=right_error;
    double left_integral = 0;
    double right_integral = 0;
    double left_derivative = 0;
    double right_derivative = 0;

    double left_voltage = 0;
    double right_voltage = 0;
    while(( abs(left_error) > settled_margin ) && (abs(right_error) > settled_margin )){
        prev_left_error = left_error;
        prev_right_error = right_error;
        left_error = encoder_units - (lf.get_position() + lm.get_position() + lb.get_position()) / 3.0;
        right_error = encoder_units - (rf.get_position() + rm.get_position() + rb.get_position()) / 3.0;
        
        //if each iteration of the loop doesn't take a uniform amount of time, derivative and integral calculations need
        //to consider time
        left_derivative = left_error - prev_left_error;
        right_derivative = right_error - prev_right_error;

        if(abs(left_error) > integral_max_error){
            left_integral = 0;    
        } else {
            left_integral = left_integral + left_error;
        }
        if(abs(right_error) > integral_max_error){
            right_integral = 0;
        } else {
            right_integral = right_integral + right_error;
        }
        
        left_voltage = Kp * left_error + Ki * left_integral + Kd * left_derivative;
        right_voltage = Kp * right_error + Ki * right_integral + Kd * right_derivative;
        if(left_voltage> 127){
            set_left_voltage(127);
        } else{
            set_left_voltage(left_voltage);
        }
        if(right_voltage > 127) {
            set_right_voltage(127);
        } else {
            set_right_voltage(right_voltage);
        }
    }
    return;
}

//Turns the robot to the given heading
void turn_absolute(int degrees, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd){
    int start_heading = imu.get_heading(); 
    //right range = start +1, start + 180 % 360
    //left range = start -1, start - 180 % 360
    int low_bound_right_range;
    int high_bound_right_range;
    if(start_heading < 180){
        low_bound_right_range = start_heading;
        high_bound_right_range = start_heading + 180;
        if(degrees > low_bound_right_range && degrees < high_bound_right_range) {
            int x = ((degrees - start_heading) > 0 ) ? (degrees - start_heading) : (degrees - start_heading + 360);
            std::cout << "\nright: ";
            std::cout << x;
            std::cout << "\n";
            turn_right_relative( x, settled_margin, integral_max_error, Kp, Ki, Kd);
        } else {
            int x = ((start_heading-degrees) > 0 ) ? (start_heading - degrees) : (start_heading - degrees + 360);
            std::cout << "\nleft: ";
            std::cout << x;
            std::cout << "\n";
            turn_left_relative(x, settled_margin, integral_max_error, Kp, Ki, Kd);
        }
    } else {
        low_bound_right_range = 360;
        high_bound_right_range = start_heading - 180;
        if(degrees > low_bound_right_range || degrees < high_bound_right_range) {
            int x = ((degrees - start_heading) > 0 ) ? (degrees - start_heading) : (degrees - start_heading + 360);
            std::cout << "\nright: ";
            std::cout << x;
            std::cout << "\n";
            turn_right_relative( x, settled_margin, integral_max_error, Kp, Ki, Kd);
        } else {
            int x = ((start_heading -degrees) > 0 ) ? (start_heading - degrees) : (start_heading - degrees + 360);
            std::cout << "\nleft: ";
            std::cout << x;
            std::cout << "\n";
            turn_left_relative(x, settled_margin, integral_max_error, Kp, Ki, Kd);
        }
    }    
    return;
}

void turn_right_relative(int degrees, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd){
    double end_heading = degrees + imu.get_heading();
    double error = degrees;
    double prev_error = 0;
    double integral = 0;
    double derivative = 0;
    double voltage = 0;
    int i = 0;
    if(end_heading >= 360){
        while(imu.get_heading() > 15){
            prev_error = error;
            error = end_heading - imu.get_heading();
            if(abs(error) > integral_max_error){
                integral = 0;
            } else {
                integral = integral + error;
            }
            derivative = error - prev_error;
            voltage = Kp * error + Ki * integral + Kd * derivative;
            if(voltage > 127){
                set_left_voltage(127);
                set_right_voltage(-127);
            } else {
                set_left_voltage(voltage);
                set_right_voltage(-voltage);
            }
            if(i%5000 == 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
        std::cout << "\nflipped to 0\n";
        end_heading-=360;
        error = end_heading - imu.get_heading();
        int i = 0;
        while(abs(error) > settled_margin){
            prev_error = error;
            error = end_heading - imu.get_heading();
            if(abs(error) > integral_max_error){
                integral = 0;
            } else {
                integral = integral + error;
            }
            derivative = error - prev_error;
            voltage = Kp * error + Ki * integral + Kd * derivative;
            if(voltage > 127){
                set_left_voltage(127);
                set_right_voltage(-127);
            } else {
                set_left_voltage(voltage);
                set_right_voltage(-voltage);
            }
            if(i%5000 == 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
        return;
    }
    while(abs(error) > settled_margin){
        prev_error = error;
        error = end_heading - imu.get_heading();
        if(abs(error) > integral_max_error){
            integral = 0;
        } else {
            integral = integral + error;
        }
        derivative = error - prev_error;
        voltage = Kp * error + Ki * integral + Kd * derivative;
        if(voltage > 127){
            set_left_voltage(127);
            set_right_voltage(-127);
        } else {
            set_left_voltage(voltage);
            set_right_voltage(-voltage);
        }
        if(i%5000 == 0){
            std::cout << error;
            std::cout << "  ";
        }
        i++;
    }
    return;
}

void turn_left_relative(int degrees, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd){
    double end_heading = imu.get_heading() - degrees;
    double prev_error = 0;
    double error = degrees;
    double integral = 0;
    double derivative = 0;
    double voltage = 0;
    int i = 0;
    if(end_heading < 0){
         while(imu.get_heading() < 345){
            prev_error = error;
            error = imu.get_heading() - end_heading;
            if(abs(error) > integral_max_error){
                integral = 0;
            } else {
                integral = integral + error;
            }
            derivative = error - prev_error;
            voltage = Kp * error + Ki * integral + Kd * derivative; 
            if(voltage > 127){
                set_left_voltage(-127);
                set_right_voltage(127);
            } else {
                set_left_voltage(-voltage);
                set_right_voltage(voltage);
            }
            if(i%5000 == 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
        std::cout << "\nflipped to 0\n";
        end_heading+=360;
        error = imu.get_heading() - end_heading;
        while(abs(error) > settled_margin){
            prev_error = error;
            error = imu.get_heading() - end_heading;
            if(abs(error) > integral_max_error){
                integral = 0;
            } else {
                integral = integral + error;
            }
            derivative = error - prev_error;
            voltage = Kp * error + Ki * integral + Kd * derivative; 
            if(voltage > 127){
                set_left_voltage(-127);
                set_right_voltage(127);
            } else {
                set_left_voltage(-voltage);
                set_right_voltage(voltage);
            }
            if(i%5000 == 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
        return;
    } else {
        while(abs(error) > settled_margin){
            prev_error = error;
            error = imu.get_heading() - end_heading;
            if(abs(error) > integral_max_error){
                integral = 0;
            } else {
                integral = integral + error;
            }
            derivative = error - prev_error;
            voltage = Kp * error + Ki * integral + Kd * derivative; 
            if(voltage > 127){
                set_left_voltage(-127);
                set_right_voltage(127);
            } else {
                set_left_voltage(-voltage);
                set_right_voltage(voltage);
            }
            if(i%5000 == 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
    return;
    }
}