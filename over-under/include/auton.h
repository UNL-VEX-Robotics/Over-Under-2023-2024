void turn(double degrees);
void go(double inches);
void push(double inches);
double convert(double degrees);

void reset_positions();
void set_all_voltage(int voltage);
void set_left_voltage(float voltage);
void set_right_voltage(float voltage);
void all_brake();

void turn_absolute_debug(int degrees, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd);
void turn_right_relative_debug(int degrees, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd);
void turn_left_relative_debug(int degrees, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd);
