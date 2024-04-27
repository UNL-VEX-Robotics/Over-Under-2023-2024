#include "global_defs.h"
#include "pid.h"
#include "pros/imu.hpp"
#include "pros/motors.hpp"
#include "auton.h"
#include <cmath>
#include <iostream>
#include <math.h>

using namespace pros;

const int circum = wheel_radius * 2 * M_PI;

double convert(double degrees){
    return degrees + 23;
}
int debug_frequency = 4000;
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


int convert(int degrees){
	return degrees + 23;
}

double inchesToEncoderUnits(double inches){
    double revolutions = inches / circum;
    return 10.0/6.0 * revolutions * blue_ticks_per_rev; 
}

void go(double inches, PID leftPID, PID rightPID){
    double now, start = pros::millis();
    double encoder_units = inchesToEncoderUnits(inches); 
    leftPID.reset();
    rightPID.reset();
    reset_motors(); 
    leftPID.setError(encoder_units); 
    rightPID.setError(encoder_units);
    double leftVoltage = 0;
    double rightVoltage = 0;
    double leftError = 0;
    double rightError = 0;
    int i = 0;
    while(!leftPID.isSettled() || !rightPID.isSettled()){
        leftError = encoder_units - (topLeftDrive.get_position() + midLeftDrive.get_position() + botLeftDrive.get_position()) / 3.0;
        rightError = encoder_units - (topRightDrive.get_position() + midRightDrive.get_position() + botRightDrive.get_position()) / 3.0;

        leftVoltage = leftPID.getNextValue(leftError);
        rightVoltage = rightPID.getNextValue(rightError);

        set_left_voltage(leftVoltage);
        set_right_voltage(rightVoltage);

        //exit the push function after 3 seconds.
        if(i % (debug_frequency + 24000) == 0){ 
            now = pros::millis();
            if(now - start > 1.5*1000){
                set_all_voltage(0);
                return;     
            }
            std::cout << " L: ";
            std::cout << leftError;
            std::cout << " R: ";
            std::cout << rightError;
            std::cout <<"\n";
        }
        i++;
    }
}

void turn_right_relative_debug(double degrees, PID turnPID){
    int i = 0;
    double end_heading = imu.get_heading() + degrees;
    double error = degrees;
    double voltage = 0;
    turnPID.reset();
    turnPID.setError(degrees);
    if(end_heading >= 360){
        while(imu.get_heading() > 10){
            error = end_heading - imu.get_heading();
            voltage = turnPID.getNextValue(error);
            set_left_voltage(voltage);
            set_right_voltage(-voltage);
            if(i%debug_frequency== 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
        end_heading -= 360;
        std::cout << "flipped to 0\n";
        while(!turnPID.isSettled()){
            error = end_heading - imu.get_heading();
            voltage = turnPID.getNextValue(error);
            set_left_voltage(voltage);
            set_right_voltage(-voltage);
            if(i%debug_frequency == 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
        return;
    } else {
        while(!turnPID.isSettled()){
            error = end_heading - imu.get_heading();
            voltage = turnPID.getNextValue(error);
            set_left_voltage(voltage);
            set_right_voltage(-voltage);
            if(i%debug_frequency == 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
        return;
    }
}

void turn_left_relative_debug(double degrees, PID turnPID){
    int i = 0;
    double end_heading = imu.get_heading() - degrees;
    double error = degrees;
    double voltage = 0;
    turnPID.reset();
    turnPID.setError(degrees);
    if(end_heading < 0){
        while(imu.get_heading() < 350){
            error = imu.get_heading() - end_heading;
            voltage = turnPID.getNextValue(error);
            set_left_voltage(-voltage);
            set_right_voltage(voltage);
            if(i%debug_frequency == 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
        std::cout << "\nflipped to 0.\n\tcurrent heading:";
        std::cout << imu.get_heading();
        std::cout << "\n\tcurrent error: ";
        std::cout << error;
        end_heading+=360;
        error = imu.get_heading() - end_heading;
        std::cout << "\n\tcorrected error: ";
        std::cout << error;
        while(!turnPID.isSettled()){
            error = imu.get_heading() - end_heading;
            voltage = turnPID.getNextValue(error);
            set_left_voltage(-voltage);
            set_right_voltage(voltage);
            if(i%debug_frequency == 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
        return;
    } else {
        while(!turnPID.isSettled()){
            error = imu.get_heading() - end_heading;
            voltage = turnPID.getNextValue(error);
            set_left_voltage(-voltage);
            set_right_voltage(voltage);
            if(i%debug_frequency == 0){
                std::cout << error;
                std::cout << "  ";
            }
            i++;
        }
        return;
    }
}


//turns the robot to the given heading
void turn(double degrees, PID turnPID){
    degrees = convert(degrees);
    turnPID.reset();
    std::cout << "current heading: ";
    std::cout << imu.get_heading();
    std::cout << "\nturning to: ";
    std::cout << degrees;
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
            std::cout << "\nright turn started: ";
            std::cout << x;
            std::cout << "\n";
            turn_right_relative_debug(x, turnPID);
        } else {
            int x = ((start_heading-degrees) > 0 ) ? (start_heading - degrees) : (start_heading - degrees + 360);
            std::cout << "\nleft turn started: ";
            std::cout << x;
            std::cout << "\n";
            turn_left_relative_debug(x, turnPID);
        }
    } else {
        low_bound_right_range = start_heading;
        high_bound_right_range = start_heading - 180;
        if(degrees > low_bound_right_range || degrees < high_bound_right_range) {
            int x = ((degrees - start_heading) > 0 ) ? (degrees - start_heading) : (degrees - start_heading + 360);
            std::cout << "\nright turn started: ";
            std::cout << x;
            std::cout << "\n";
            turn_right_relative_debug(x, turnPID);
        } else {
            int x = ((start_heading -degrees) > 0 ) ? (start_heading - degrees) : (start_heading - degrees + 360);
            std::cout << "\nleft turn started: ";
            std::cout << x;
            std::cout << "\n";
            turn_left_relative_debug(x, turnPID);
        }
    }    
    return;
}
