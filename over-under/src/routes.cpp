#include "global_defs.h"
#include "pros/imu.hpp"
#include "pros/llemu.hpp"
#include "pros/motors.hpp"
#include "auton.h"
#include "pros/rtos.hpp"
#include <string>

void shoot(int num){
    intakeRight = 127;
    intakeLeft = 127;
    leftFly = 127;
    rightFly = -127;
    pros::delay(2000);
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
    imu.set_heading(convert(0));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    // go(48);
    // turn(0);
    // set_all_voltage(0);
    // pros::lcd::set_text(5, "Drive Complete");


    pros::lcd::set_text(4, std::to_string(imu.get_heading()));
    turn(180);
    pros::lcd::set_text(5, std::to_string(imu.get_heading()));
    pros::delay(2000);
    turn(0);
    pros::lcd::set_text(6, std::to_string(imu.get_heading()));


    // intakePneu.set_value(1);
    // flywheel_in(500, 60);
    // intake_in(1200, 100);
    // intakePneu.set_value(0);
    // go(-5);
    // turn(220);
    // pros::lcd::set_text(4, "Completed Turn");
    // go(10);
    // pros::lcd::set_text(4, "Drive Finished");
    // turn(270);
    // go(5);
    // intake_out(1000);
    // 
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
    go(68);
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
    go(24);
    turn(270);
    go(24);
    go(-12);
    go(16);
    go(-12);
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
    go(-4);
    imu.set_heading(convert(0));
    go(36);
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
    go(24);
    eleLock.set_value(true);
    rightElevation = -100;
    leftElevation = -100;
    pros::delay(2000);
    rightElevation = 0;
    leftElevation = 0;
}