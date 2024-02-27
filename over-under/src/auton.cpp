#include "main.h"
#include <chrono>
#include "global_defs.h"
#include "pros/imu.h"
#include "pros/imu.hpp"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "auton.h"
#include <cmath>
#include <string>
#include <math.h>

using namespace pros;

const int circum = wheel_radius * 2 * M_PI;

//straight params
int tick_margin = 15;
int integral_max_error_s= 1000;
//PID values for when we are moving straight
float Kps = 0.225;
float Kis = 0.01;
float Kds = 0;

//turning params
int degree_margin = 1;
int integral_max_error_t = 14;
//PID values for when we are turning
float Kpt = 1.1;
float Kit = 0.00001;
float Kdt = 0;

double convert(double degrees){
    return degrees + 23;
}

void reset_motors(){
    topLeftDrive.set_zero_position(0);
    midLeftDrive.set_zero_position(0);
    botLeftDrive.set_zero_position(0);
    topRightDrive.set_zero_position(0);
    midRightDrive.set_zero_position(0);
    botRightDrive.set_zero_position(0);
    return;
}

void set_all_voltage(int voltage){
    topLeftDrive = voltage;
    midLeftDrive = voltage;
    botLeftDrive = voltage;
    topRightDrive = voltage;
    midRightDrive = voltage;
    botRightDrive = voltage;
    return;
}

void set_left_voltage(float voltage){
    topLeftDrive = voltage;
    midLeftDrive = voltage;
    botLeftDrive = voltage;
    return;
}

void set_right_voltage(float voltage){
    topRightDrive = voltage;
    midRightDrive = voltage;
    botRightDrive = voltage;
    return;
}

void all_brake(){
    topLeftDrive.brake();
    midLeftDrive.brake();
    botLeftDrive.brake();
    topRightDrive.brake();
    midRightDrive.brake();
    botRightDrive.brake();
}

int convert(int degrees){
	return degrees + 23;
}

void shoot(int num){
    intake = 127;
    leftFly = -127;
    for(int i=0; i < num; i++){
        intakePneu.set_value(1);
        pros::delay(600);
        intakePneu.set_value(0);
        pros::delay(1000);
    }
    intakePneu.set_value(0);
    pros::delay(500);
}

double calculateDrivetrainVoltage(double error, double previousError, double PID_Integral, double PID_Derivative){
    //if each iteration of the loop doesn't take a uniform amount of time, derivative and integral calculations need to consider time.
    PID_Derivative = error - previousError;
    if(abs(error) > integral_max_error_s){
        PID_Integral = integral_max_error_s;
    }
    else {
        PID_Integral += Kis * error;
    }
    double voltage = Kps * error + Kis * PID_Integral + Kds * PID_Derivative; //error is the proportional factor of the PID calculation
    return voltage;
}

double constrainVoltage(double voltage, int i){
    //keeps voltage <= 127, and gives a continuous ramping effect
    //TODO: make the ramping scale factor a variable t rather than just a magic number
    if(voltage > 127){
        double x = 127.0 * (i+100)/25000;
        return x > 127 ? 127 : x;
    }
    return voltage;
}

void push(double inches){
    double start = pros::millis();
    double now = pros::millis(); 
    double revolutions = inches / circum;
    double encoder_units = 10.0/6.0 * revolutions * blue_ticks_per_rev;
    reset_motors();
    int i = 0;
    double left_error = encoder_units;
    double right_error = encoder_units;
    double prev_left_error = left_error;
    double prev_right_error = right_error;
    double left_integral = 0;
    double right_integral = 0;
    double left_derivative = 0;
    double right_derivative = 0;
    double left_voltage = 0;
    double right_voltage = 0;
    while(( abs(left_error) > tick_margin ) && (abs(right_error) > tick_margin )){ 
        prev_left_error = left_error;
        left_error = encoder_units - (topLeftDrive.get_position() + midLeftDrive.get_position() + botLeftDrive.get_position()) / 3.0; 
        prev_right_error = right_error;
        right_error = encoder_units - (topRightDrive.get_position() + midRightDrive.get_position() + botRightDrive.get_position()) / 3.0; 
        left_voltage = calculateDrivetrainVoltage(left_error, prev_left_error, left_integral, left_derivative);
        right_voltage = calculateDrivetrainVoltage(right_error, prev_right_error, right_integral, right_derivative);
        set_left_voltage(constrainVoltage(left_voltage, i));
        set_right_voltage(constrainVoltage(right_voltage, i));

        if(i % 5000 == 0){ 
            //TODO: will this act as intended if each iteration of the while loop takes longer than 1 millisecond? If it is shorter, it will take 10000 iterations to to the timeout
            now = pros::millis();
            if(now - start > 5*1000){  //after 5 seconds
                left_error = 0;
                right_error = 0;
                set_all_voltage(0);
            }
            std::cout << "\n   L: ";
            std::cout << left_error;
            std::cout << "   R: ";
            std::cout << right_error;
            std::cout <<"\n";
        }
        i++;

    }
}

void turn_right_relative_debug(double degrees){
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
            if(abs(error) > integral_max_error_t){
                integral = 0;
            } else {
                integral = integral + error;
            }
            derivative = error - prev_error;
            voltage = Kpt * error + Kit * integral + Kdt * derivative;
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
        while(abs(error) > degree_margin){
            prev_error = error;
            error = end_heading - imu.get_heading();
            if(abs(error) > integral_max_error_t){
                integral = 0;
            } else {
                integral = integral + error;
            }
            derivative = error - prev_error;
            voltage = Kpt * error + Kit * integral + Kdt * derivative;
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
    while(abs(error) > degree_margin){
        prev_error = error;
        error = end_heading - imu.get_heading();
        if(abs(error) > integral_max_error_t){
            integral = 0;
        } else {
            integral = integral + error;
        }
        derivative = error - prev_error;
        voltage = Kpt * error + Kit * integral + Kdt * derivative;
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

void turn_left_relative_debug(double degrees){
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
            if(abs(error) > integral_max_error_t){
                integral = 0;
            } else {
                integral = integral + error;
            }
            derivative = error - prev_error;
            voltage = Kpt * error + Kit * integral + Kdt * derivative; 
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
        while(abs(error) > degree_margin){
            prev_error = error;
            error = imu.get_heading() - end_heading;
            if(abs(error) > integral_max_error_t){
                integral = 0;
            } else {
                integral = integral + error;
            }
            derivative = error - prev_error;
            voltage = Kpt * error + Kit * integral + Kdt * derivative; 
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
        while(abs(error) > degree_margin){
            prev_error = error;
            error = imu.get_heading() - end_heading;
            if(abs(error) > integral_max_error_t){
                integral = 0;
            } else {
                integral = integral + error;
            }
            derivative = error - prev_error;
            voltage = Kpt * error + Kit * integral + Kdt * derivative; 
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

//Turns the robot to the given heading
void turn(double degrees){
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
            turn_right_relative_debug( x);
        } else {
            int x = ((start_heading-degrees) > 0 ) ? (start_heading - degrees) : (start_heading - degrees + 360);
            std::cout << "\nleft: ";
            std::cout << x;
            std::cout << "\n";
            turn_left_relative_debug(x);
        }
    } else {
        low_bound_right_range = 360;
        high_bound_right_range = start_heading - 180;
        if(degrees > low_bound_right_range || degrees < high_bound_right_range) {
            int x = ((degrees - start_heading) > 0 ) ? (degrees - start_heading) : (degrees - start_heading + 360);
            std::cout << "\nright: ";
            std::cout << x;
            std::cout << "\n";
            turn_right_relative_debug( x);
        } else {
            int x = ((start_heading -degrees) > 0 ) ? (start_heading - degrees) : (start_heading - degrees + 360);
            std::cout << "\nleft: ";
            std::cout << x;
            std::cout << "\n";
            turn_left_relative_debug(x);
        }
    }    
    return;
}
