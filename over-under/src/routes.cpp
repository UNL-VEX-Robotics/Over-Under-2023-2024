#include "global_defs.h"
#include "pros/imu.hpp"
#include "pros/motors.hpp"
#include "auton.h"
#include "pid.h"
#include "math.h"

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

double relative_math(double inches){
    double e_units = (10.0/6.0)*300*(inches/(2.0*1.625*M_PI));
    return e_units;
}

void moveRelative(double inches, double velo = 100){
    pros::Motor_Group leftMotors({botLeftDrive, midLeftDrive, topLeftDrive});
    pros::Motor_Group rightMotors({botRightDrive, midRightDrive, topRightDrive});
    leftMotors.move_relative(relative_math(inches), velo);
    rightMotors.move_relative(relative_math(inches), velo);
}

void match_drew(){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(270));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    moveRelative(60, 75);
    turn(0, turnPID);
    moveRelative(24, 75);
}

void match_drew(PID left, PID right, PID turnP){
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
    imu.reset(true);
    imu.set_heading(convert(270));
    while(imu.is_calibrating()){pros::delay(10);}
    go(54.53340818250772, leftPID, rightPID);
    turn(0.7690246825780491, turnPID);
    go(34.610577065134493, leftPID, rightPID);
    go(-6, leftPID, rightPID);
    turn(173.46786331214756, turnPID);
    go(18.31634855187454, leftPID, rightPID);
    turn(96.5646842913341, turnPID);
    go(48.703327196404146, leftPID, rightPID);
    intakeUpDown();
    turn(359.683451952727, turnPID);
    go(69.505060765386, leftPID, rightPID);
    //entered other side
    turn(327.4028831217652, turnPID);
    go(26.50111162334766, leftPID, rightPID);
    turn(270.6986943829835, turnPID);
    //push in first time |
    go(24.745170688182457, leftPID, rightPID);
    turn(124.42647658143909, turnPID);
    go(15.71643521458265, leftPID, rightPID);
    turn(45.020292302071226, turnPID);
    go(11.266258934983622, leftPID, rightPID);
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