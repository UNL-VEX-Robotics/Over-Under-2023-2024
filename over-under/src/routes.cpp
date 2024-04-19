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

void activateIntake180GO(){
    intakeActuation.move_relative(red_ticks_per_rev / 2.0, 65);
}

void intakeUpDown(){
    intakeActuation.move_relative(red_ticks_per_rev / 4.0, 65);
}

void intake_in(int time, int velo = 127){
    intakeRight = velo;
    intakeLeft = velo;
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

void flywheel_out(int time){
    leftFly = -127;
    rightFly = 127;
    pros::delay(time);
    leftFly = 0;
    rightFly = 0;
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

void match_drew_MONEY(PID leftPID, PID rightPID, PID turnPID){
    while(imu.is_calibrating()){pros::delay(10);}
    imu.set_heading(convert(270));
    while(imu.is_calibrating()){pros::delay(10);}
    go(54.53340818250772, leftPID, rightPID);
    turn(0.7690246825780491, turnPID);
    go(28.610577065134493, leftPID, rightPID);
    go(-6, leftPID, rightPID);
    turn(173.46786331214756, turnPID);
    go(18.31634855187454, leftPID, rightPID);
    turn(96.5646842913341, turnPID);
    go(48.703327196404146, leftPID, rightPID);
    intakeUpDown();
    turn(359.683451952727, turnPID);
    go(69.505060765386, leftPID, rightPID);
    intakeUpDown();
    turn(327.4028831217652, turnPID);
    go(33.50111162334766, leftPID, rightPID);
    turn(270.6986943829835, turnPID);
    go(15.745170688182457, leftPID, rightPID);
    turn(124.42647658143909, turnPID);
    go(20.71643521458265, leftPID, rightPID);
    turn(61.020292302071226, turnPID);
    go(14.266258934983622, leftPID, rightPID);
    intakeUpDown();
    turn(294.26129085247777, turnPID);
    go(14.95259709883203, leftPID, rightPID);
    turn(263.74596725608353, turnPID);
    go(14.099913191222138, leftPID, rightPID);
    turn(154.71203173817696, turnPID);
    go(26.96831207176304, leftPID, rightPID);
    turn(190.76253722314246, turnPID);
    go(23.647978349110524, leftPID, rightPID);
    turn(266.5167285310023, turnPID);
    go(22.120866529139406, leftPID, rightPID);
    turn(1.0809241866606953, turnPID);
    go(30.533433478729506, leftPID, rightPID);
}