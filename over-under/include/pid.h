#pragma once

class PID{
    public:
        PID(double P_weight, double I_weight, double D_weight, double toleranceMargin, double integralMaxError);
        void setMargin(double margin);
        void tuneD(double incr);
        void tuneP(double incr);
        void tuneI(double incr);
        void setError(double value);
        void reset();
        double getNextValue(double currentValue);
        bool isSettled();
        double P_weight, I_weight, D_weight;
    private:
        int i;
        double toleranceMargin;
        double integralMaxError;
        double error;
        double previousError;
        double P_value, I_value, D_value;
        double outputLimit;
};