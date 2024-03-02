#include "global_defs.h"
#include "pros/imu.hpp"
#include "pros/motors.hpp"
#include "auton.h"
#include "pros/rtos.hpp"

void shoot(int num){
    intakeRight = 127;
    intakeLeft = 127;
    leftFly = -127;
    rightFly = -127;
    pros::delay(2000);
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

void intake_in(int time){
    intakeRight = 127;
    intakeLeft = 127;
    pros::delay(time);
    intakeRight = 0;
    intakeLeft = 0;
}

void intake_out(int time){
    intakeRight = -127;
    intakeLeft = -127;
    pros::delay(time);
    intakeRight = 0;
    intakeLeft = 0;
}

void flywheel_in(int time){
    leftFly = 127;
    rightFly = -127;
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

void skills(double start_heading){
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
    pros::delay(500);
    rightElevation = 0;
    leftElevation = 0;
}

void match_tanner(){
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

void match_drew(){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(135));
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
    go(-1);
    turn(convert(260));
    leftFly=-100;
    intakeRight = -100;
    intakeLeft = 100;
    pros::delay(1000);
    leftFly=0;
    intakeRight = 0;
    intakeLeft = 0;
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

void full_skills_route_part1(){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(135));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    //shoot(22);
    go(-6);
    pros::delay(500);
    turn(45);
    go(17);
    turn(353);
    intakePneu.set_value(true);
    go(68);
    intakePneu.set_value(false);
    turn(315);
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
    go(24);
    turn(270);
    go(24);
    go(-12);
    go(16);
    go(-12);
    flippers.set_value(false);
    turn(210);
    go(24);
    flippers.set_value(true);
}

void full_skills_route_part3(){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(210));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    go(24);
    turn(0);
    // go(36);
    // go(-12);
    // flippers.set_value(false);
    // turn(200);
    // go(36);
    // turn(0);
    // flippers.set_value(true);
    // go(36);
    // go(-12);
    // flippers.set_value(false);
    // turn(100);
}

void full_skills_route_part4(){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(100));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    go(72);
    turn(180);
    rightElevation = 100;
    leftElevation = 100;
    pros::delay(2000);
    intakePneu.set_value(true);
    go(24);
    eleLock.set_value(true);
    rightElevation = -100;
    leftElevation = -100;
    pros::delay(2000);
    rightElevation = 0;
    leftElevation = 0;
    intakePneu.set_value(false);
}