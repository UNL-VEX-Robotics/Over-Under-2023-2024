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

        self.label_bot_size = Label(self.popup, text="Bot width (your bot is a square do not argue)")
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
        clear_button = Button(self.master, text="Clear Points", command=field_drawer.clear_points)
        gen_button = Button(self.master, text="Generate Program", command=field_drawer.generate_program)

        clear_button.pack(side=LEFT, expand=1)
        gen_button.pack(side=RIGHT, expand=1)

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
        

    def write_program(self, starting_heading: float, points: list[tuple[float, float], int]):
        self.remove_route()
        self.write_header()
        self.write_main()
        points.reverse()
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
                if rotated:
                    dist *= -1
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
        #stack of tuples (rectangle_id, arrow_id)
        self.route_tuples = []
        self.bot_id = self.canvas.create_oval(0,0,0,0)
        self.arrow_id = self.canvas.create_oval(0,0,0,0)

    def draw_field(self):
        # Draw the playing field and other objects here
        # For simplicity, let's draw a rectangle representing the field
        self.canvas.pack()
        self.background = PhotoImage(file="field_small.png")
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

    def on_click(self, event):
        bot_coords = self.canvas.coords(self.bot_id)
        arrow_coords = self.canvas.coords(self.arrow_id)
        point_id = self.canvas.create_polygon(bot_coords,fill='blue', outline='black')
        arrow_id = self.canvas.create_polygon(arrow_coords, fill='dark violet', outline='black')
        rotated = 1 if self.heading == 180 else 0
        self.route_tuples.append((point_id,arrow_id,rotated))
        self.heading = 0
        

    def remove_last_point(self, event):
        if self.route_tuples != []:
            a, b, _ = self.route_tuples.pop()
            self.canvas.delete(a)
            self.canvas.delete(b)

    def clear_points(self):
        self.heading = self.start_heading
        while (self.route_tuples != []) and (x := self.route_tuples.pop()):
            a,b,c = x
            self.canvas.delete(a)
            self.canvas.delete(b)

    def generate_program(self):
        box_ids = [tuple[0] for tuple in self.route_tuples]
        rotateds = [tuple[-1] for tuple in self.route_tuples]
        point_coords = [self.canvas.coords(point_id) for point_id in box_ids]

        center_points = []
        i = 0
        for coords in point_coords:
            x = [coords[0],coords[2],coords[4],coords[6]]
            y = [coords[1],coords[3],coords[5],coords[7]]
            x_min = min(x)
            y_min = min(y)
            x_center = x_min + sqrt(2*self.bot_half_width**2)
            y_center = y_min + sqrt(2*self.bot_half_width**2)
            center_points.append(((x_center,y_center), rotateds[i]))
            i+=1
        self.file_writer.write_program(self.start_heading,center_points)

    def rotate(self, event):
        self.heading += 180
        self.heading %= 360

def main():
    root = Tk()

    popup = PopupWindow(root, 800)
    root.wait_window(popup.popup)

    root.mainloop()

if __name__ == "__main__":
    main()