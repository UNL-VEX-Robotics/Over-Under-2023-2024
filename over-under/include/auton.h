void reset_positions();
void set_all_voltage(int voltage);
void set_left_voltage(float voltage);
void set_right_voltage(float voltage);
void all_brake();

void move_distance_proportional(float inches, float p);
void move_individual_sides_debug(float inches, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd);
void move_individual_sides(float inches, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd);

void turn_absolute(int degrees, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd);
void turn_right_relative(int degrees, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd);
void turn_left_relative(int degrees, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd);
