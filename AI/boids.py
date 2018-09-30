import random
from Tkinter import *   #visual package
from math import sqrt
import time

WIDTH = 1000
HEIGHT = 800
BOIDS = 10            #number of boids
SPEED_LIMIT = 400
BOID_SIZE = 5
t0 = time.clock()


################################################################################

def main():
    #Initialize and loops over
    #Tkinter needs mainloop() function to work correctly
    initialise()
    mainloop()

def initialise():
    build_boids()
    build_graph()

#Creates GUI
def build_graph():
    global graph
    root = Tk()
    root.overrideredirect(True)
    root.geometry('%dx%d+%d+%d' % (WIDTH, HEIGHT, (root.winfo_screenwidth() - WIDTH) / 2, (root.winfo_screenheight() - HEIGHT) / 2))
    root.bind_all('<Escape>', lambda event: event.widget.quit())
    graph = Canvas(root, width=WIDTH, height=HEIGHT, background='white')
    graph.after(40, update)
    graph.pack()

#Update frame every 40ms
def update():
    draw()
    curr_time = time.clock() - t0
    if curr_time < 10 or curr_time > 20:
        move()
    else:
        move_wind()
    graph.after(40, update)

#Draw all the boids in one frame
def draw():
    graph.delete(ALL) #deletes old boids
    for boid in boids:
        x1 = boid.position.x - BOID_SIZE
        y1 = boid.position.y - BOID_SIZE
        x2 = boid.position.x + BOID_SIZE
        y2 = boid.position.y + BOID_SIZE

        graph.create_oval((x1, y1, x2, y2), fill='blue')
    graph.update()

#Defines movement of the boids without wind
def move():
    for boid in boids:
        boid.update_velocity(boids)
    for boid in boids:
        boid.move()

#Defines movement of the boids with wind
def move_wind():
    for boid in boids:
        boid.update_velocity_wind(boids)
    for boid in boids:
        boid.move()

#Sets the limit of a boid
def limit_speed(boid):
    if boid.velocity.mag() > SPEED_LIMIT:
        boid.velocity /= boid.velocity.mag() / SPEED_LIMIT

def build_boids():
    global boids
    #initialise specified number of boids
    boids = tuple(Boid(WIDTH, HEIGHT) for boid in xrange(BOIDS))

#----------------------2D Vector class----------------------------------------#

# Our own class for 2D vector

class Vector_2D:

    def __init__(self, x, y):
        self.x = float(x)
        self.y = float(y)

    def __repr__(self):
        return 'Vector_2D(%s, %s)' % (self.x, self.y)

    def __add__(self, other):
        return Vector_2D(self.x + other.x, self.y + other.y)

    def __sub__(self, other):
        return Vector_2D(self.x - other.x, self.y - other.y)

    def __mul__(self, other):
        return Vector_2D(self.x * other, self.y * other)

    def __div__(self, other):
        return Vector_2D(self.x / other, self.y / other)

    def __iadd__(self, other):
        self.x += other.x
        self.y += other.y
        return self

    def __isub__(self, other):
        self.x -= other.x
        self.y -= other.y
        return self

    def __idiv__(self, other):
        if isinstance(other, Vector_2D):
            self.x /= other.x if other.x else 1
            self.y /= other.y if other.y else 1
        else:
            self.x /= other
            self.y /= other
        return self

    def mag(self):
        return sqrt((self.x ** 2) + (self.y ** 2))

#---------------------------End 2D Vector class--------------------------------#

# Class Boid implements the rules and the random starting position

class Boid:

    def __init__(self, width, height):
        self.velocity = Vector_2D(0, 0)
        self.position = Vector_2D(*self.random_start(width, height))

#Every boid starts at random location
    def random_start(self, width, height):
        y = random.randint(1, height)
        x = random.randint(1, width)

        return x,y


    def update_velocity(self, boids):
        v1 = self.rule1(boids)
        v2 = self.rule2(boids)
        v3 = self.rule3(boids)
        self.vectors = v1 + v2 + v3

    def update_velocity_wind(self, boids):
        v1 = self.rule1(boids)
        v2 = self.rule2(boids)
        v3 = self.rule3(boids)
        v4 = self.rule4(boids)
        self.vectors = v1 + v2 + v3 + v4

    def move(self):
        self.velocity += self.vectors
        limit_speed(self)
        self.position += self.velocity/125

    #Cohesion Rule
    def rule1(self, boids):
        vector = Vector_2D(0, 0)
        for boid in boids:
            if boid is not self:
                vector += boid.position
        vector /= len(boids) - 1
        return (vector - self.position) / 8

    #Separation Rule
    def rule2(self, boids):
        vector = Vector_2D(0, 0)
        for boid in boids:
            if boid is not self:
                #We divided by 100 because with 25 the boids were too slow
                if (self.position - boid.position).mag() < 100:
                    vector -= (boid.position - self.position)
        return vector

    #Alignemnt Rule
    def rule3(self, boids):
        vector = Vector_2D(0, 0)
        for boid in boids:
            if boid is not self:
                vector += boid.velocity
        vector /= len(boids) - 1
        return (vector - self.velocity) / 32

    #Wind rule
    def rule4(self, boids):
        vector = Vector_2D(10, 0)
        return vector

#-----------------------Starts program---------------------------------------#
main()
