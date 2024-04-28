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

void intakeBounce(){
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
    intakeUpDown(); //down
    go(52.03340818250772, leftPID, rightPID);
    turn(0.7690246825780491, turnPID);
    intakeRight = -127;
    intakeLeft = -127;
    flippers.set_value(1);
    pros::delay(100);
    go(31.610577065134493, leftPID, rightPID); //<- decreased by 1 on THURS
    flippers.set_value(0);
    go(-6, leftPID, rightPID);
    turn(173.46786331214756, turnPID);
    intakeRight = 0;
    intakeLeft = 0;
    intakeUpDown(); //up
    go(18.31634855187454, leftPID, rightPID);
    turn(96.5646842913341, turnPID);
    go(47.703327196404146, leftPID, rightPID); //<- 2 very good runs at end of day thurs
    intakeUpDown(); //down
    turn(360.683451952727, turnPID);
    go(65.505060765386, leftPID, rightPID); //<- decreased by 4?
    //entered other side
    turn(315.4028831217652, turnPID);
    //flippers.set_value(1);
    go(26.50111162334766, leftPID, rightPID);
    turn(270.6986943829835, turnPID);
    //push in first time 
    go(24.745170688182457, leftPID, rightPID);
    //flippers.set_value(0);
    //do more hits
    //go(-6, leftPID, rightPID);
    //turn(90, turnPID);
    //go(13, leftPID, rightPID);
    turn(124.42647658143909, turnPID);
    go(15.71643521458265, leftPID, rightPID);
    turn(45.020292302071226, turnPID);
    go(6.266258934983622, leftPID, rightPID);
    //GET BALL
    intakeUpDown(); //up
    intakeRight = 80;
    intakeLeft = 80;
    intakeUpDown(); //down
    pros::delay(1500);
    intakeRight = 0;
    intakeLeft = 0;
    imu.set_heading(convert(135));
    while(imu.is_calibrating()){pros::delay(10);}
    go(-2, leftPID, rightPID);
    turn(255, turnPID);
    go(8, leftPID, rightPID);
    //WE RELY ON THIS BEING INACCURATE AND PUTTING US TO THE LEFT
    //potentially need a turn(255)
    leftFly = 70;
    rightFly = 70;
    intakeRight = 80;
    intakeLeft = 80;
    intakeUpDown(); //up
    pros::delay(500);
    leftFly = 0;
    rightFly = 0;
    intakeRight = 0;
    intakeLeft = 0;
    turn(85, turnPID);
    go(-20, leftPID, rightPID);
    imu.set_heading(convert(90));
    while(imu.is_calibrating()){pros::delay(10);}
    go(2,leftPID, rightPID);
    turn(132.8499736489614, turnPID);
    go(39.80702249603705, leftPID, rightPID);
    turn(179.6373733260375, turnPID);
    intakeUpDown();
    go(30.336607588852118, leftPID, rightPID);
}

void drew_elevate(PID leftPID, PID rightPID, PID turnPID){
    while(imu.is_calibrating()){pros::delay(10);}
    imu.set_heading(convert(90));
    while(imu.is_calibrating()){pros::delay(10);}
    turn(132.8499736489614, turnPID);
    go(39.80702249603705, leftPID, rightPID);
    turn(179.6373733260375, turnPID);
    go(30.336607588852118, leftPID, rightPID);
}
