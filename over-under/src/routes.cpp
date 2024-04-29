#include "global_defs.h"
#include <math.h>
#include "pros/imu.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "auton.h"
#include "pid.h"

void shoot(int num){
    intakeRight = 127;
    intakeLeft = -127;
    leftFly = .71 * -127;
	rightFly = .71 * 127; 
    pros::delay(600);
    for(int i = 0; i < num; i++){
        intakeActuation.move_relative(red_ticks_per_rev / 2.0, 85);
        pros::delay(1200); // <- decreased from 1500
    }
    leftFly = 0;
    rightFly = 0;
    intakeRight = 0;
    intakeLeft = 0;
}

void activateIntake180GO(){
    intakeActuation.move_relative(red_ticks_per_rev / 2.0, 65);
}

void activateIntake90GO(){
    intakeActuation.move_relative(red_ticks_per_rev / 4.0, 65);
}

void intake_in(int time, int velo = 127){
    intakeRight = velo;
    intakeLeft = -velo;
    pros::delay(time);
    intakeRight = 0;
    intakeLeft = 0;
}

void intake_out(int time, int velo = 127){
    intake_in(time, -velo);
}

void flywheel_in(int time, int velo = 127){
    leftFly = velo;
    rightFly = -velo;
    pros::delay(time);
    leftFly = 0;
    rightFly = 0;
}

void flywheel_out(int time, int velo = 127){
    leftFly = -velo;
    rightFly = velo;
    pros::delay(time);
    leftFly = 0;
    rightFly = 0;
}



void match_tanner(int start_voltage, PID leftPID, PID rightPID, PID turnPID){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(135));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    intakeActuation.move_relative(-red_ticks_per_rev / 4.0, 95);//Started at 150
    pros::delay(825);
    intakeRight = 90;
    intakeLeft = -90;
    pros::delay(275);  // <- decrease
    intakeRight = 0;
    intakeLeft = 0;

    go(-2, leftPID, rightPID);
    turn(255, turnPID);
    go(8, leftPID, rightPID);
    //WE RELY ON THIS BEING INACCURATE AND PUTTING US TO THE LEFT
    //potentially need a turn(255)
    leftFly = 70;
    rightFly = -70;
    intakeRight = -80;
    intakeLeft = 80;
    intakeActuation.move_relative(red_ticks_per_rev / 4.0, 150);
    pros::delay(500);
    leftFly = 0;
    rightFly = 0;
    intakeRight = 0;
    intakeLeft = 0;
    turn(85, turnPID);
    go(-20, leftPID, rightPID);
    go(6, leftPID, rightPID);
    turn(45, turnPID);
    go(13, leftPID, rightPID);
    turn(135, turnPID);
    flippers.set_value(1);
    leftPID.P_weight *= 0.6;
    rightPID.P_weight *= 0.6;
    leftPID.I_weight *= 2;
    rightPID.I_weight *= 2;
    go(10, leftPID, rightPID);
    //turn(80, turnPID); // <- new addition
    leftPID.P_weight /= 0.6;
    rightPID.P_weight /= 0.6;
    leftPID.I_weight /= 2;
    rightPID.I_weight /= 2;
    shoot(11); // <- 
    flippers.set_value(0);
    go(-8.145870119269029, leftPID, rightPID);
    turn(32.57405713457251, turnPID);
    go(24.60673078651449, leftPID, rightPID);
    turn(354.1736579704442, turnPID);
    leftPID.P_weight *= 0.6;
    rightPID.P_weight *= 0.6;
    leftPID.I_weight *= 2;
    rightPID.I_weight *= 2;
    intakeActuation.move_relative(red_ticks_per_rev / 4.0, 150);
    go(18.91370550685402, leftPID, rightPID);
}


void tanner_touch_elevation(PID leftPID, PID rightPID, PID turnPID){
    while(imu.is_calibrating()){pros::delay(10);}
    imu.set_heading(convert(135));
    while(imu.is_calibrating()){pros::delay(10);}
    go(-8.145870119269029, leftPID, rightPID);
    turn(32.57405713457251, turnPID);
    go(24.60673078651449, leftPID, rightPID);
    turn(354.1736579704442, turnPID);
    go(18.91370550685402, leftPID, rightPID);
}
