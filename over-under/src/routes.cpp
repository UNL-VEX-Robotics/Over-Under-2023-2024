#include "global_defs.h"
#include "pros/imu.hpp"
#include "pros/motors.hpp"
#include "auton.h"
#include "pid.h"

PID leftPID = PID(0.225, 0.01, 0, 15, 1000);
PID rightPID = PID(0.225, 0.01, 0, 15, 1000);

PID turnPID = PID(1.1, 0.00001, 0, 1, 14);

void shoot(int num){
    intakeRight = 127;
    intakeLeft = 127;
    leftFly = -127;
    rightFly = -127;
    for(int i=0; i < num; i++){
        intakePneu.set_value(1);
        pros::delay(600);
        intakePneu.set_value(0);
        pros::delay(1000);
    }
    intakePneu.set_value(0);
    intakeRight = 0;
    intakeLeft = 0;
    leftFly = 0;
    rightFly = 0;
    pros::delay(500);
}


void match_drew(){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(convert(135)));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    intakeRight = 127;
    intakeLeft = -127;
    pros::delay(500);
    intakePneu.set_value(1);
    pros::delay(500);
    pros::delay(500);
    intakePneu.set_value(0);
    intakeRight = 0;
    intakeLeft = 0;
    pros::delay(500);
    go(-1, leftPID, rightPID);
    turn(convert(260), turnPID);
    leftFly=-100;
    intakeRight = -100;
    intakeLeft = 100;
    pros::delay(1000);
    leftFly=0;
    intakeRight = 0;
    intakeLeft = 0;
    turn(convert(85), turnPID);
    go(-20, leftPID, rightPID);
    go(12, leftPID, rightPID);
    turn(convert(45), turnPID);
    go(15, leftPID, rightPID);
    turn(convert(180), turnPID);
    rightElevation.set_zero_position(0);
    leftElevation.set_zero_position(0);
    while((leftElevation.get_position() < (double)red_ticks_per_rev*0.1)){
        rightElevation = 127;
        leftElevation = 127;
    } 
    rightElevation = 0;
    leftElevation = 0;
    intakePneu.set_value(0);
    pros::delay(1000);
    go(-28, leftPID, rightPID);

}

void full_skills_route_part1(){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(135));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    shoot(22);
    go(-6, leftPID, rightPID);
    turn(45, turnPID);
    go(24, leftPID, rightPID);
    turn(0, turnPID);
    intakePneu.set_value(true);
    go(72, leftPID, rightPID);
    intakePneu.set_value(false);
    turn(315, turnPID);
}

void full_skills_route_part2(){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(315));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    flippers.set_value(true);
    go(24, leftPID, rightPID);
    turn(270, turnPID);
    go(24, leftPID, rightPID);
    go(-12, leftPID, rightPID);
    go(16, leftPID, rightPID);
    go(-12, leftPID, rightPID);
    flippers.set_value(false);
    turn(180, turnPID);
    flippers.set_value(true);
    go(24, leftPID, rightPID);
}

void full_skills_route_part3(){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(180));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    turn(225, turnPID);
    go(24, leftPID, rightPID);
    turn(0, turnPID);
    go(36, leftPID, rightPID);
    go(-12, leftPID, rightPID);
    flippers.set_value(false);
    turn(200, turnPID);
    go(36, leftPID, rightPID);
    turn(0, turnPID);
    flippers.set_value(true);
    go(36, leftPID, rightPID);
    go(-12, leftPID, rightPID);
    flippers.set_value(false);
    turn(100, turnPID);
}

void full_skills_route_part4(){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(100));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    go(72, leftPID, rightPID);
    turn(180, turnPID);
    rightElevation = 100;
    leftElevation = 100;
    pros::delay(2000);
    intakePneu.set_value(true);
    go(24, leftPID, rightPID);
    eleLock.set_value(true);
    rightElevation = -100;
    leftElevation = -100;
    pros::delay(2000);
    rightElevation = 0;
    leftElevation = 0;
    intakePneu.set_value(false);
}