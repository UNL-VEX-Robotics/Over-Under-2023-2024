//pid.hpp
#pragma once

class PID{
    public:
        PID(double P_weight, double I_weight, double D_weight, double toleranceMargin, double integralMaxError);
        void setMargin(double margin);
        void tune(double p, double i, double d);
        void setError(double value);
        double getNextValue(double currentValue);
        bool isSettled();
    private:
        double toleranceMargin;
        double integralMaxError;
        double P_weight, I_weight, D_weight;
        double error;
        double previousError;
        double P_value, I_value, D_value;
        double outputLimit;
};