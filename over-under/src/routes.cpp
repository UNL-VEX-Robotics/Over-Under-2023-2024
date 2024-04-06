#include "global_defs.h"
#include "pros/imu.hpp"
#include "pros/motors.hpp"
#include "auton.h"
#include "pid.h"

PID leftPID = PID(0.12, 0.01, 0, 15, 1000);
PID rightPID = PID(0.12, 0.01, 0, 15, 1000);

PID turnPID = PID(0.79, 0.00001, 0, 1, 14);


void shoot(int num){
    intakeRight = 127;
    intakeLeft = 127;
    leftFly = -127;
    rightFly = -127;
    for(int i=0; i < num; i++){
        //intake.set_value(1);
        pros::delay(600);
        //intake.set_value(0);
        
        pros::delay(1000);
    }
    //intake.set_value(0);
    intakeRight = 0;
    intakeLeft = 0;
    leftFly = 0;
    rightFly = 0;
    pros::delay(500);
}

void intake_in(int time, int velo = 127){
    intakeRight = velo;
    intakeLeft = velo;
    pros::delay(time);
    intakeRight = 0;
    intakeLeft = 0;
}

void intake_out(int time, int velo = 127){
    intakeRight = -velo;
    intakeLeft = -velo;
    pros::delay(time);
    intakeRight = 0;
    intakeLeft = 0;
}

void flywheel_in(int time, int velo = 127){
    leftFly = velo;
    rightFly = -velo;
    pros::delay(time);
    leftFly = 0;
    rightFly = 0;
}

void flywheel_out(int time){
    leftFly = -127;
    rightFly = 127;
    pros::delay(time);
    leftFly = 0;
    rightFly = 0;
}

void elevate_up(int time){
    leftElevation = 100;
    rightElevation = 100;
    pros::delay(time);
    leftElevation = 0;
    rightElevation = 0;
}

void elevate_down(int time){
    leftElevation = -100;
    rightElevation = -100;
    pros::delay(time);
    leftElevation = 0;
    rightElevation = 0;
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
    //intake.set_value(1);
    pros::delay(500);
    pros::delay(500);
    //intake.set_value(0);
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
    //intake.set_value(0);
    pros::delay(1000);
    go(-28, leftPID, rightPID);

}

void full_skills_route_part1(PID leftPID, PID rightPID, PID turnPID){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(135));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    shoot(1);
    go(-6, leftPID, rightPID);
    turn(45, turnPID);
    go(24, leftPID, rightPID);
    turn(0, turnPID);
    //intake.set_value(true);
    go(72, leftPID, rightPID);
    //intake.set_value(false);
    turn(315, turnPID);
}

void full_skills_route_part2(PID leftPID, PID rightPID, PID turnPID){
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

void full_skills_route_part3(PID leftPID, PID rightPID, PID turnPID){
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

void full_skills_route_part4(PID leftPID, PID rightPID, PID turnPID){
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
    //intake.set_value(true);
    go(24, leftPID, rightPID);
    eleLock.set_value(true);
    rightElevation = -100;
    leftElevation = -100;
    pros::delay(2000);
    rightElevation = 0;
    leftElevation = 0;
    //intake.set_value(false);
}


void test_route(PID left, PID right, PID turnP){
    turn(270, turnP);
    go(24, left, right);
    turn(90, turnP);
    go(24, left, right);
    turn(270, turnP);
    go(12, left, right);
    turn(0, turnP);
    go(-12, left, right);
}