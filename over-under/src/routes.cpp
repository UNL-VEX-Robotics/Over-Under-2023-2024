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
    intakeActuation.move_relative(-red_ticks_per_rev / 2.0, 65);
}

void intakeUpDown(){
    intakeActuation.move_relative(-red_ticks_per_rev / 4.0, 65);
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
    go(72.505060765386, leftPID, rightPID); //<- decreased by 4?
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
    turn(145.94421969906756, turnPID);
    go(25.028582700584543, leftPID, rightPID);
    turn(234.41262943686615, turnPID);
    go(25.73444695345132, leftPID, rightPID);
    turn(135.68206039317266, turnPID);
    intakeUpDown();
    leftPID.P_weight *=0.6;
    leftPID.I_weight*=2;
    rightPID.P_weight *=0.6;
    rightPID.I_weight*=2;
    go(22.810052520763737, leftPID, rightPID);
}
void turn_left(int abs_x, PID p){
    double start = imu.get_heading();
    double rel = start - abs_x;
    turn_left_relative_debug(rel+23, p);
}

void get_over_and_score(PID leftPID, PID rightPID, PID turnPID){
    intakeUpDown();
    while(imu.is_calibrating()){pros::delay(10);}
    imu.reset(true);
    imu.set_heading(convert(0));
    while(imu.is_calibrating()){pros::delay(10);}
    go(70.27304917249572, leftPID, rightPID);
    turn(319.7636416907262, turnPID);
    intakeUpDown();
    go(29.426779640320824, leftPID, rightPID);
    turn(270.0, turnPID);
    go(12.2, leftPID, rightPID);
    go(-12.485906294698834, leftPID, rightPID);
    turn(270, turnPID);
    go(14.10511909010127, leftPID, rightPID);
}


void drew_AWP(PID leftPID, PID rightPID, PID turnPID){
    while(imu.is_calibrating()){pros::delay(10);}
    imu.reset();
    imu.set_heading(convert(0));
    while(imu.is_calibrating()){pros::delay(10);}
    intakeUpDown();
    leftPID.P_weight *=1.6;
    rightPID.P_weight *=1.6;
    go(12, leftPID, rightPID);
    leftPID.P_weight /=1.6;
    rightPID.P_weight /=1.6;
    go(51.75122850580998, leftPID, rightPID);
    turn(315.0, turnPID);
    go(20.636204302148204, leftPID, rightPID);
    intakeLeft =-127; 
    intakeRight =-127; 
    go(8, leftPID, rightPID);
    go(-8, leftPID, rightPID);
    intakeUpDown();
    turn(45.0, turnPID);
    intakeRight = 90;
    intakeLeft = 90;
    leftPID.P_weight *=0.6;
    leftPID.I_weight*=2;
    rightPID.P_weight *=0.6;
    rightPID.I_weight*=2;
    go(11.675747170952274, leftPID, rightPID);
    pros::delay(777);  // <- decrease
    flippers.set_value(0);
    intakeLeft = 0;
    intakeRight= 0;
    leftPID.P_weight /=0.6;
    leftPID.I_weight /=2;
    rightPID.P_weight /=0.6;
    rightPID.I_weight /=2;
    go(-4.344464063610149, leftPID, rightPID);
    turn(311.18592516570965, turnPID);
    go(14.28691597231537, leftPID, rightPID);
    intakeLeft = -127;
    intakeRight = -127;
    pros::delay(800);
    intakeLeft = 0;
    intakeRight = 0;
    //turn(90.0, turnPID); //270
    turn_left(90, turnPID);
    go(-10.368, leftPID, rightPID); //+
    go(-3, leftPID, rightPID);
    turn(123.84005613496726, turnPID);
    go(20.342035296400407, leftPID, rightPID);
    turn(222.31622484053102, turnPID);
    go(34.79387532310823, leftPID, rightPID);
    turn(132.3974377975002, turnPID);
    go(23.919224067682464, leftPID, rightPID);
}