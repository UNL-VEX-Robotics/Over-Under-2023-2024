from tkinter import *
from math import cos, sin, atan, atan2, pi, sqrt

route_name = "initialized"
bot_size = 10 

def rotate(point: tuple[float, float], center_point: tuple[float, float], theta: float) -> tuple[float: float]:
    """
    rotate point round center_point 
    theta in radians
    """
    theta = theta * pi / 180
    x_p = point[0]
    y_p = point[1]
    x_c = center_point[0]
    y_c = center_point[1]
    
    x = cos(theta) * (x_p - x_c) - sin(theta) * (y_p - y_c) + x_c 
    y = sin(theta) * (x_p - x_c) + cos(theta) * (y_p - y_c) + y_c 

    return (x,y)

#gives center point of a rectangle with corner points (a,b), (c,d), (e,f), (g,h)
def center(a,b,c,d,e,f,g,h, half_width): 
    pass

class PopupWindow:
    def __init__(self, master, width):
        self.width = width
        self.master = master
        self.popup = Frame(master)
        self.popup.pack()

        self.label_filename = Label(self.popup, text="Route name:")
        self.label_filename.grid(row=0, column=0, padx=5, pady=5)
        self.entry_filename = Entry(self.popup)
        self.entry_filename.grid(row=0, column=1, padx=5, pady=5)

        self.label_bot_size = Label(self.popup, text="Bot width (inches)")
        self.label_bot_size.grid(row=1, column=0, padx=5, pady=5)
        self.entry_bot_size = Entry(self.popup)
        self.entry_bot_size.grid(row=1, column=1, padx=5, pady=5)

        self.label_heading= Label(self.popup, text="Starting angle (0 is the enemy's goal)")
        self.label_heading.grid(row=2, column=0, padx=5, pady=5)
        self.entry_heading= Entry(self.popup)
        self.entry_heading.grid(row=2, column=1, padx=5, pady=5)

        self.ok_button = Button(self.popup, text="OK", command=self.on_ok)
        self.ok_button.grid(row=3, column=0, padx=5, pady=5)

        self.default_button = Button(self.popup, text="DEFAULT", command=self.on_default)
        self.default_button.grid(row=3, column=1, padx=5, pady=5)
    
    def inch_to_pixel(self, inches):
        return inches * self.width / 144 

    def on_default(self):
        global route_name, bot_size
        route_name = "out"
        bot_size = 60
        heading = 315
        self.popup.destroy()
        self.end_and_draw_field(heading)

    def on_ok(self):
        global route_name, bot_size
        route_name = self.entry_filename.get()
        bot_size = self.inch_to_pixel(float(self.entry_bot_size.get()))
        heading = int(self.entry_heading.get())
        self.popup.destroy()
        self.end_and_draw_field(heading)


    def end_and_draw_field(self, heading):
        field_drawer = FieldDrawer(self.master, heading, self.width, self.width)
        field_drawer.draw_field()

class RouteWriter():
    def __init__(self, file: str, width, height):
        self.file = file
        self.width = width 
        self.height = height
        self.inch_per_pixel = 144 / width  
    
    #field is 12 feet: 144 inches square
    def pixel_to_inch(self, pixel: float):
        return pixel * self.inch_per_pixel
    
    def calc_heading_to_point(self, a: tuple[float, float], b: tuple[float, float]):
        x = b[0] - a[0]
        y = b[1] - a[1]
        return ((atan2(y,x) * 180 / pi) + 90) % 360 

    def remove_route(self):
        start = 0
        end = 0
        with open("src/routes.cpp", "r") as f:
            lines = f.readlines()
            for i, line in enumerate(lines):
                if line.strip().startswith("void " + route_name):
                    start = i
                    for i, line in enumerate(lines[i:]):
                        if line.strip() == "}":
                            end = i + start
                            break
                    break
                
        with open("src/routes.cpp", "w") as f:
            lines = lines[:start] + lines[end+1 if end!= 0 else 0:]
            f.writelines(lines)

    def write_header(self):
        with open("include/routes.h", "r") as f:
            contents = f.read()
        if f"void {route_name}" in contents:
            return
        else:
            with open("include/routes.h", "a") as f:
                f.write(f"\nvoid {route_name}(PID leftPID, PID rightPID, PID turnPID);")
    
    def write_main(self):
        with open("src/main.cpp", "r") as f:
            lines = f.readlines()
        for line in lines:
            if f"std::make_tuple({route_name}" in line:
                return

        for i in range(len(lines)):
            if lines[i].strip() == "//DONT DELETE OR MOVE THIS COMMENT SRSLY":
                index = i
                break
        line1 = "  skills_routes.push_back(\n" 
        line2 = f'      std::make_tuple({route_name}, "{route_name}"));\n'
        lines.insert(index,line2)
        lines.insert(index,line1)
        with open("src/main.cpp", "w") as f:
            f.writelines(lines)
        

    def write_program(self, starting_heading: float, points: list[tuple[float, float], int], utilities: list[str]):
        self.remove_route()
        self.write_header()
        self.write_main()

        points.reverse()
        utilities.reverse()
        print(f"num points: {len(points)}\nnum utilities: {len(utilities)}")
        
        with open("src/routes.cpp", "a+") as f:
            f.write(f"\nvoid {route_name}(PID leftPID, PID rightPID, PID turnPID)"+"{\n")
            f.write("    while(imu.is_calibrating()){pros::delay(10);}\n")
            f.write(f"    imu.set_heading(convert({starting_heading}));\n")
            f.write("    while(imu.is_calibrating()){pros::delay(10);}\n")
            curr_point, _ = points.pop()
            while (points != []) and (x := points.pop()):
                next_point, rotated = x
                heading = self.calc_heading_to_point(curr_point, next_point)
                heading = heading if rotated == 0 else (heading + 180) % 360
                dist = sqrt((curr_point[0] - next_point[0])**2 + (curr_point[1] - next_point[1])**2)
                dist = self.pixel_to_inch(dist)
                if rotated:
                    dist *= -1
                utility = utilities.pop()
                if utility:
                    f.write(f"    {utility};\n") 
                f.write(f"    turn({heading}, turnPID);\n")
                f.write(f"    go({dist}, leftPID, rightPID);\n")
                curr_point = next_point
            f.write("}\n")

class FieldDrawer:
    def __init__(self, master, heading, width, height):
        self.master = master
        self.width = width
        self.height = height
        self.bot_half_width = bot_size / 2 
        self.file_writer = RouteWriter(route_name, width, height) 

        self.canvas = Canvas(master, width=width, height=height, bg="white")

        self.canvas.bind("<ButtonRelease-1>", self.on_click)
        self.canvas.bind("<ButtonRelease-3>", self.remove_last_point)
        self.canvas.bind("<Motion>",self.show_bot)
        self.canvas.bind("<Button-4>",self.rotate)
        self.canvas.bind("<Button-5>",self.rotate)


        self.start_heading = heading
        self.heading = heading
        #stack of tuples [((x_center,y_center), rotated), rectangle_id, arrow_id)]
        self.current_utility = ""
        self.utilities = []
        self.route_tuples = []
        self.bot_id = self.canvas.create_oval(0,0,0,0)
        self.arrow_id = self.canvas.create_oval(0,0,0,0)
        self.line_id = self.canvas.create_line(0,0,0,0)

    def draw_field(self):
        # Draw the playing field and other objects here
        # For simplicity, let's draw a rectangle representing the field
        self.canvas.grid(row=1,columnspan=8)

        self.clear_button = Button(self.master, text="Clear Points", command=self.clear_points)
        self.gen_button = Button(self.master, text="Generate Program", command=self.generate_program)

        self.clear_button.grid(row=2,column=0,columnspan=2)
        self.gen_button.grid(row=2,column=2,columnspan=2)

        self.background = PhotoImage(file="field_750.png")
        self.canvas.create_image(0,0,image=self.background,anchor="nw")


    def show_bot(self, event):
        self.canvas.delete(self.bot_id)
        self.canvas.delete(self.arrow_id)
        x = event.x
        y = event.y

        reg_rectangle_point1 = (x - self.bot_half_width, y - self.bot_half_width)
        reg_rectangle_point2 = (x + self.bot_half_width, y - self.bot_half_width)
        reg_rectangle_point3 = (x - self.bot_half_width, y + self.bot_half_width)
        reg_rectangle_point4 = (x + self.bot_half_width, y + self.bot_half_width)

        reg_triangle_point1 = reg_rectangle_point1 
        reg_triangle_point2 = (x + self.bot_half_width, y - self.bot_half_width)
        reg_triangle_point3 = (x, y - self.bot_half_width * 5/3)

        rot_rectangle_point1 = rotate(reg_rectangle_point1, (x,y), self.heading)
        rot_rectangle_point2 = rotate(reg_rectangle_point2, (x,y), self.heading)
        rot_rectangle_point3 = rotate(reg_rectangle_point3, (x,y), self.heading)
        rot_rectangle_point4 = rotate(reg_rectangle_point4, (x,y), self.heading)

        rot_triangle_point1 = rotate(reg_triangle_point1, (x,y), self.heading) 
        rot_triangle_point2 = rotate(reg_triangle_point2, (x,y), self.heading) 
        rot_triangle_point3 = rotate(reg_triangle_point3, (x,y), self.heading) 

        self.bot_id = self.canvas.create_polygon(rot_rectangle_point1, rot_rectangle_point3, rot_rectangle_point4, rot_rectangle_point2, fill="red", outline='black')
        self.arrow_id = self.canvas.create_polygon(rot_triangle_point1, rot_triangle_point2, rot_triangle_point3, fill='purple',outline = 'black')
        self.canvas.delete(self.line_id)
        if self.route_tuples != []:
            self.line_id = self.canvas.create_line(self.route_tuples[-1][0][0][0], self.route_tuples[-1][0][0][1], x,y)

    def on_click(self, event):
          
        if len(self.route_tuples) == 0:
            self.pack_utility_buttons()           
        elif self.current_utility:
            utility_duration = self.utility_duration_entry.get()
            utility_func_call = self.current_utility + "(" + utility_duration + ")"
            self.utilities.append(utility_func_call)
            self.current_utility = ""
        else:
            self.utilities.append(self.current_utility)

            
        bot_coords = self.canvas.coords(self.bot_id)
        arrow_coords = self.canvas.coords(self.arrow_id)
        box_id = self.canvas.create_polygon(bot_coords,fill='blue', outline='black')
        arrow_id = self.canvas.create_polygon(arrow_coords, fill='dark violet', outline='black')
        rotated = 1 if self.heading == 180 else 0
        self.route_tuples.append(( ((event.x,event.y) ,rotated), box_id,arrow_id))
        self.heading = 0
        

    def remove_last_point(self, event):
        if self.route_tuples != []:
            _, a, b = self.route_tuples.pop()
            self.canvas.delete(a)
            self.canvas.delete(b)

    def clear_points(self):
        for butt in self.butts:
            butt.destroy()
        self.heading = self.start_heading
        self.current_utility = ""
        self.utilities = []
        while (self.route_tuples != []) and (x := self.route_tuples.pop()):
            _, a,b= x
            self.canvas.delete(a)
            self.canvas.delete(b)

    def generate_program(self):
        center_points = [tuple[0] for tuple in self.route_tuples]
        for center_point in center_points:
            x=center_point[0][0]
            y=center_point[0][1]
        self.file_writer.write_program(self.start_heading, center_points, self.utilities.copy())

    def rotate(self, event):
        self.heading += 180
        self.heading %= 360

    def pack_utility_buttons(self):
        ininbutt = Button(self.master, text="Intake In", command=self.intake_in)
        inoutbutt = Button(self.master, text="Intake Out", command=self.intake_out)
        flyinbutt = Button(self.master, text="Flywheel In", command=self.flywheel_in)
        flyoutbutt = Button(self.master, text="Flywheel Out", command=self.flywheel_out)
        eleupbutt = Button(self.master, text="Elevation Up", command=self.elevate_up)
        eledownbutt = Button(self.master, text="Elevation Down", command=self.elevate_down)
        time_label = Label(self.master, text="Duration (ms)")
        time_entry = Entry(self.master)
        self.utility_duration_entry = time_entry
        self.butts = (time_label, time_entry, ininbutt, inoutbutt, flyinbutt, flyoutbutt, eledownbutt, eleupbutt) 
        for i, butt in enumerate(self.butts):
            butt.grid(row=0,column=i)

    def intake_in(self):
        self.current_utility = "intake_in" 

    def intake_out(self):
        self.current_utility = "intake_out" 

    def flywheel_out(self):
        self.current_utility = "flywheel_out" 

    def flywheel_in(self):
        self.current_utility = "flywheel_in" 
    
    def elevate_up(self):
        self.current_utility = "elevate_up"

    def elevate_down(self):
        self.current_utility = "elevate_down"

    
def main():
    root = Tk()

    popup = PopupWindow(root, 750)
    root.wait_window(popup.popup)

    root.mainloop()

if __name__ == "__main__":
    main()