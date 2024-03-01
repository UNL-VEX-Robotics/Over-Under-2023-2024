//pid.cpp
#include "pid.hpp"
#include <math.h>

//returns the sign of a number. Thanks Stack Overflow!
template <typename T> int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

//P stands for Proportional. How big of changes should we make? Is it going far enough?
//I stands for Integral. Ajust this if error isn't being compensated for over time. Helps prevent oscillations.
//D stands for Derivative. Adjust this if the values aren't changing fast enough.
PID::PID(double P_weight, double I_weight, double D_weight, double toleranceMargin, double integralMaxError){
    this->i = 0;
    this->error = 0;
    this->P_weight = P_weight;
    this->I_weight = I_weight;
    this->D_weight = D_weight;
    this->toleranceMargin = toleranceMargin;
    this->integralMaxError = integralMaxError;
    this->outputLimit = 127;
}

//Set distance from target value
void PID::setError(double value){
    error = value;
}

void PID::tune(double p, double i, double d){
    P_weight = p;
    I_weight = i;
    D_weight = d;
}

void PID::limit(double value){
    outputLimit = value;
}

//any error smaller than the margin is considered negligeable.
void PID::setMargin(double margin){
    this->toleranceMargin = margin;
}

void PID::reset(){
    I_value = 0;
    previousError = 0;
    i=0;
}

double PID::getNextValue(double currentError){
    i++;
    if(i == 1)
        previousError = currentError;
    else
        previousError = error;
    error = currentError;
    double P_value = error;
    double I_value;
    double D_value = error - previousError;

    I_value += I_weight * error;
    I_value = fmin(abs(I_value), integralMaxError);
    double output = P_weight * P_value + I_weight * I_value + D_weight * D_value;
    //ramp up
    output = output * (i+100)/25000;
    //constrains the outputs to [-outputLimit, outputLimit]
    return fmin(abs(output), 127) * sign(output); 
}

bool PID::isSettled(){
    return abs(error) < toleranceMargin;
}
