#include "main.h"
#include "auton.h"
#include "pid.hpp"
#include <chrono>
#include "global_defs.h"
#include "pros/imu.h"
#include "pros/imu.hpp"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include <cmath>
#include <functional>
#include <string>
#include <math.h>

using namespace pros;

const int circum = wheel_radius * 2 * M_PI;
PID leftStraightPID = PID(0.225, 0.01, 0, 15, 1000, 127);
PID rightStraightPID = PID(0.225, 0.01, 0, 15, 1000, 127);

PID turnPID = PID(1.1, 0.00001, 0, 1, 14, 127);

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
}

void set_all_voltage(float voltage){
    set_left_voltage(voltage);
    set_right_voltage(voltage);
}

void set_left_voltage(float voltage){
    topLeftDrive = voltage;
    midLeftDrive = voltage;
    botLeftDrive = voltage;
}

void set_right_voltage(float voltage){
    topRightDrive = voltage;
    midRightDrive = voltage;
    botRightDrive = voltage;
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

double calculateRequiredPushDistance(double inches){
    double revolutions = inches / circum;
    return 10.0/6.0 * revolutions * blue_ticks_per_rev; //TODO: is 10.0/6.0 the gear ratio? define this as a variable
}

void push(double inches){
    double now, start = pros::millis();
    double encoder_units = calculateRequiredPushDistance(inches); //number of encoder units to spin until we reach our destination.
    reset_motors(); //sets all motor encodor absolute positions to zero
    leftStraightPID.setError(encoder_units); 
    rightStraightPID.setError(encoder_units);
    double leftVoltage = 0;
    double rightVoltage = 0;
    double leftPosition = 0;
    double rightPosition = 0;
    int i = 0;
    while(!leftStraightPID.isSettled() || !rightStraightPID.isSettled()){
        leftPosition = (topLeftDrive.get_position() + midLeftDrive.get_position() + botLeftDrive.get_position()) / 3.0;
        rightPosition = (topRightDrive.get_position() + midRightDrive.get_position() + botRightDrive.get_position()) / 3.0;
        leftVoltage = leftStraightPID.getNextValue(leftPosition);
        rightVoltage = rightStraightPID.getNextValue(rightPosition);
        //pid already limits voltage. Might not ramp properly because it isnt as sophisticated. PID algorithm is supposed to do the ramping though.
        set_left_voltage(leftVoltage);
        set_right_voltage(rightVoltage);

        //exit the push function after 5 seconds.
        now = pros::millis();
            if(now - start > 5*1000){
                set_all_voltage(0);
                return;     
            }
        if(i % 5000 == 0){ 
            //TODO: will this act as intended if each iteration of the while loop takes longer than 1 millisecond? If it is shorter, it will take 10000 iterations to to the timeout
            std::cout << "\nTarget: ";
            std::cout << encoder_units;
            std::cout << "   L: ";
            std::cout << leftPosition;
            std::cout << "   R: ";
            std::cout << rightPosition;
            std::cout <<"\n";
        }
        i++;
    }
}

//clockwise is positive
void turnRelative(double deltaTheta){
    double initialHeading = imu.get_heading();
    double endHeading = initialHeading + deltaTheta;
    turnPID.setError(deltaTheta);
    double voltage = 0;
    double degreesTurned = 0;
    int i = 0;
    //This seems way too simple something aught to be wrong.
    while(!turnPID.isSettled()){
        degreesTurned = imu.get_heading() - initialHeading;
        voltage = turnPID.getNextValue(degreesTurned);
        set_right_voltage(voltage);
        set_left_voltage(-voltage);
    }
}

//Turns the robot to the given heading
void turnToHeading(double currentHeading, double targetHeading){
    //TODO: make sure this turns counterclockwise when the delta_theta %360 > 180
    //TODO: make this keep track of change in direction throughout the entire match to limit drift.
    //currently this assumes it is exactly where we say it is, so deviations add up over time.
    double deltaTheta = fmod(targetHeading - currentHeading, 360);
    //If it is faster to turn counter clockwise, make deltaTheta = 360 - deltaTheta.
    double fastestTurn = abs(deltaTheta) > 180 ? 360 - deltaTheta : deltaTheta;
    turnRelative(fastestTurn);    
}
