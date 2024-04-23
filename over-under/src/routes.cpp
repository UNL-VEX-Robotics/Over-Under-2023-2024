#include "global_defs.h"
#include <math.h>
#include "pros/imu.hpp"
#include "pros/motors.h"
#include "pros/motors.hpp"
#include "auton.h"
#include "pid.h"


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
    activateIntake90GO();
    turn(359.683451952727, turnPID);
    go(69.505060765386, leftPID, rightPID);
    activateIntake90GO();
    turn(327.4028831217652, turnPID);
    go(33.50111162334766, leftPID, rightPID);
    turn(270.6986943829835, turnPID);
    go(15.745170688182457, leftPID, rightPID);
    turn(124.42647658143909, turnPID);
    go(20.71643521458265, leftPID, rightPID);
    turn(61.020292302071226, turnPID);
    go(14.266258934983622, leftPID, rightPID);
    activateIntake90GO();
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

void match_tanner(PID leftPID, PID rightPID, PID turnPID){
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    imu.set_heading(convert(135));
    while (imu.is_calibrating()){
        pros::delay(10);
    }
    intakeActuation.move_relative(-red_ticks_per_rev / 8.0, 150);
    intakeRight = 80;
    intakeLeft = -80;
    pros::delay(1500);
    intakeRight = 0;
    intakeLeft = 0;

    go(-1, leftPID, rightPID);
    turn(260, turnPID);
    go(8, leftPID, rightPID);
    leftFly = 70;
    rightFly = -70;
    intakeRight = -80;
    intakeLeft = 80;
    pros::delay(2000);
    leftFly = 0;
    rightFly = 0;
    intakeRight = 0;
    intakeLeft = 0;
    turn(85, turnPID);
    go(-20, leftPID, rightPID);
    go(18, leftPID, rightPID);
    pros::Motor_Group left( {topLeftDrive, midLeftDrive, botLeftDrive});
    left.move_relative(4*10.0/(4 *wheel_radius* M_PI) * blue_ticks_per_rev, 75);
}
