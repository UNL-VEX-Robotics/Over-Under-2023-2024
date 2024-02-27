#include "main.h"
#include <chrono>
#include "global_defs.h"
#include "pros/imu.h"
#include "pros/imu.hpp"
#include "pros/llemu.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "auton.h"
#include "pros/rtos.h"
#include <string>
#include <math.h>

void route_skills(double start_heading){
    int i = 0;
    shoot(5);
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(start_heading));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    go(-2);
    turn(convert(0));
    go(7*12);
    //deployFlaps();
    turn(convert(45));
    go(2.25*12);
    turn(convert(90));
    go(1.75*12);
    //unDeployFlaps();
    //reset??
    go(-1*12);
    turn(convert(180));
    go(4*12);
    //deployFlaps();
    turn(convert(90));
    go(3*12);
    turn(convert(0));
    go(3.5*12);
}


void ez_skills_start_on_left(){
    shoot(15);
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(45));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    go(-1);
    turn(convert(315));
    go(1.5*12);
    turn(convert(0));
    rightElevation.set_zero_position(0);
    double num_revolutions_to_elevate = 1.5;
    while(rightElevation.get_position() < (double) red_ticks_per_rev*num_revolutions_to_elevate){
        rightElevation = 127;
        leftElevation = 127;
    }
    rightElevation = 0;
    leftElevation = 0;
    go(4*12+3);
    while(rightElevation.get_position() < (double) red_ticks_per_rev*num_revolutions_to_elevate){
        rightElevation = -127;
        leftElevation = -127;
    }
    eleLock.set_value(1);
    pros::delay(500);
    rightElevation = 0;
    leftElevation = 0;
}

void ez_skills_start_on_right(){
    shoot(15);
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(315));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    go(-1);
    turn(convert(45));
    go(1.5*12);
    turn(convert(0));
    rightElevation.set_zero_position(0);
    double num_revolutions_to_elevate = 1.5;
    while(rightElevation.get_position() < (double) red_ticks_per_rev*num_revolutions_to_elevate){
        rightElevation = 127;
        leftElevation = 127;
    }
    rightElevation = 0;
    leftElevation = 0;
    go(4*12+3);
    while(rightElevation.get_position() < (double) red_ticks_per_rev*num_revolutions_to_elevate){
        rightElevation = -127;
        leftElevation = -127;
    }
    eleLock.set_value(1);
}



void route_match_tanner(){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(0));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    go(50);
    turn(convert(45));
    go(12);
    intakePneu.set_value(1);
    go(-6);
    turn(convert(270));
    go(30);
    go(-12);
    turn(convert(135));
    go(24);
    turn(convert(180));
    go(18);
}

void route_match_drew(){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(135));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    intake = 127;
    pros::delay(500);
    intakePneu.set_value(1);
    pros::delay(500);
    pros::delay(500);
    intakePneu.set_value(0);
    intake = 0;
    pros::delay(500);
    go(-1);
    turn(convert(260));
    leftFly=-100;
    intake=-100;
    pros::delay(1000);
    leftFly=0;
    intake=0;
    turn(convert(85));
    go(-20);
    go(12);
    turn(convert(45));
    go(15);
    turn(convert(180));
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
    go(-28);
}