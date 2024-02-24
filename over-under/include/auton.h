void turn(double degrees);
void go(double inches);
void push(double inches);
double convert(double degrees);

void route_skills(double degrees);
void route_match_drew();
void route_match_tanner();

void reset_positions();
void set_all_voltage(int voltage);
void set_left_voltage(float voltage);
void set_right_voltage(float voltage);
void all_brake();

void move_distance_proportional(float inches, float p);
void move_distance_proportional_individual_sides(float inches, float p);
void move_individual_sides_debug(float inches, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd);
int move_individual_sides_debug_loop_control(float inches, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd);
void move_individual_sides(float inches, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd);

void turn_absolute_debug(int degrees, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd);
void turn_right_relative_debug(int degrees, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd);
void turn_left_relative_debug(int degrees, int settled_margin, int integral_max_error, float Kp, float Ki, float Kd);
