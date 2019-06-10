#Projet IR 2019 : Ogar - jeu de type agar.io
#Simulation du circuit d'exploration des bleus
#Les chiens vont vers le point du circuit le plus proche
#(projeté orhogonal)
#Lucas GHENIA


from tkinter import *
from random import *
from math import *

#========== Constants ==========
MAX_WIDTH = 1500
MAX_HEIGHT = 700
POINT_SIZE = 2
MARGIN = 5
STOP = True
DOGS_LIST = []
SHEEPS_LIST = []

#========== Dogs and sheeps classes ==========

class Dot:
    def init(self,color,posx,posy,texture):
        self.color=color
        self.x = posx
        self.y = posy
        self.texture = texture
        if(color == "white"):
            self.speed = 1
            self.radius = 0
            self.sightx = 0
            self.sighty = 0
            self.checked_points = []
            self.sheeps = []
            
        if(color == "blue"):
            self.speed = 2.5
            self.radius = 0
            self.sightx = 100
            self.sighty = 100
            self.checked_points = []
            self.sheeps = []
            
        if(color == "yellow"):
            self.speed = 2
            self.radius = 20
            self.sightx = 30
            self.sighty = 30
            self.checked_points = []
            self.sheeps = []
            
        if(color == "green"):
            self.speed = 1.5
            self.radius = 30
            self.sightx = 40
            self.sighty = 40
            self.checked_points = []
            self.sheeps = []
            
        if(color == "purple"):
            self.speed = 1
            self.radius = 20
            self.sightx = 26
            self.sighty = 80
            self.checked_points = []
            self.sheeps = []


    def print(self):
        print("Dot : \ncolor = ",self.color,"\nspeed = ", self.speed,"\naction radius = ", self.radius,"\nsight surface = ", self.sightx,"x",self.sighty,"\nposition = ", self.x,",",self.y,"\nEND")
            
    

#========== Generating functions ==========

def generate_point(color):
    posx = randint(0,MAX_WIDTH)
    posy = randint(0,MAX_HEIGHT)
    radius = POINT_SIZE
    color = color
    dot = Board.create_oval(posx-radius, posy-radius, posx+radius, posy+radius, outline=color, fill=color)
    dot_object = Dot()
    dot_object.init(color,posx,posy,dot)
    return dot_object

def generate_dog_blue():
    global blue_circuit
    dog = generate_point('blue')
    for i in range(0, blue_circuit.size):
        dog.checked_points.append(False)
    DOGS_LIST.append(dog)
    
def generate_dog_yellow():
    dog = generate_point('yellow')
    DOGS_LIST.append(dog)
    
def generate_dog_green():
    dog = generate_point('green')
    DOGS_LIST.append(dog)

def generate_dog_purple():
    dog = generate_point('purple')
    DOGS_LIST.append(dog)

def generate_sheep():
    sheep = generate_point('white')
    SHEEPS_LIST.append(sheep)
    


#=========== DOGS AIS ==========

class Circuit:
    def init(self):
        self.points = []
        self.size = 0
    def add_point(self,posx,posy):
        self.points.append( (posx,posy) )
        self.size += 1
    def get_point(self,i):
        return (self.points[i][0] , self.points[i][1] )


#NUMBER OF POINTS : 4 + 4*MAX_WIDTH//400

blue_circuit = Circuit()
blue_circuit.init()

blue_zone_xmin = MAX_WIDTH%400
blue_zone_ymin= 0
blue_zone_xmax = MAX_WIDTH
blue_zone_ymax = MAX_HEIGHT

blue_circuit.add_point(blue_zone_xmin + 100,blue_zone_ymin + 100)
blue_circuit.add_point(blue_zone_xmax - 100,blue_zone_ymin + 100)
blue_circuit.add_point(blue_zone_xmax - 100,blue_zone_ymax -100)

for i in range(0 , (MAX_WIDTH//400)-1):
    blue_circuit.add_point(blue_zone_xmax - 100*(4*i+3) , blue_zone_ymax - 100)
    blue_circuit.add_point(blue_zone_xmax - 100*(4*i+3) , blue_zone_ymin + 100*3)
    blue_circuit.add_point(blue_zone_xmax - 100*(4*i+5) , blue_zone_ymin + 100*3)
    blue_circuit.add_point(blue_zone_xmax - 100*(4*i+5) , blue_zone_ymax - 100)
    


blue_circuit.add_point(blue_zone_xmin + 100,blue_zone_ymax -100)

for i in range(0, blue_circuit.size):
    print(blue_circuit.get_point(i))

#SCOUT DOG AI
def dog_scout_movement(dog, circuit):
    is_scouting = False
    i = 0
    while(i < circuit.size and is_scouting == False):
        if( dog.checked_points[i] ):
            is_scouting = True
            last_indice = i
        i += 1
    
    if( not(is_scouting) ):#Spawn : find the closest point
        min_distance = MAX_WIDTH
        min_indice = circuit.size+1
        for i in range(0 , circuit.size):
            pointx = circuit.get_point(i)[0]
            pointy = circuit.get_point(i)[1]
            distance = sqrt( (dog.x - pointx)**2 + (dog.y - pointy)**2 )
            if( distance < min_distance):
                min_distance = distance
                min_indice = i
        ( targetx , targety ) = circuit.get_point(min_indice)
        """for i in range( 0,ceil(min_distance) ):
            if (is_on_circuit(dog.x+i,dog.y,circuit)):
                print("prout1 x = ",dog.x+i)
                ( targetx , targety ) = ( dog.x+i , dog.y )
            if (is_on_circuit(dog.x,dog.y+i,circuit)):
                print("prout2 y = ",dog.y+i)
                ( targetx , targety ) = ( dog.x , dog.y+i )
            if (is_on_circuit(dog.x-i,dog.y,circuit)):
                print("prout3 x = ",dog.x-i)
                ( targetx , targety ) = ( dog.x-i , dog.y )
            if (is_on_circuit(dog.x,dog.y-i,circuit)):
                print("prout4 y = ",dog.y-i)
                ( targetx , targety ) = ( dog.x , dog.y-i )"""
        
        if(min_distance < MARGIN):
            vx = 0
            vy = 0
            dog.checked_points[min_indice] = True
        else:
            ( vx , vy ) = position_to_speed(dog.x,dog.y,targetx,targety,dog.speed)

        dog.x += vx
        dog.y += vy

        
    else:#scouting algorithm
        if(last_indice == circuit.size-1 ):
            next_point_indice = 0
        else:
            next_point_indice = last_indice + 1

        ( targetx , targety ) = circuit.get_point(next_point_indice)
        distance = sqrt( (dog.x - targetx)**2 + (dog.y - targety)**2 )
        if(distance < MARGIN):
            vx = 0
            vy = 0
            dog.checked_points[next_point_indice] = True
            dog.checked_points[last_indice] = False
        else:
            ( targetx , targety ) = circuit.get_point(next_point_indice)
            ( vx , vy ) = position_to_speed(dog.x,dog.y,targetx,targety,dog.speed)
            
        dog.x += vx
        dog.y += vy
        
    """for i in range(0, len(SHEEPS_LIST) ):
        target = SHEEPS_LIST[i]
        dx = target.x - dog.x
        dy = target.y - dog.y
        if( abs(dx) <= dog.sightx and abs(dy) <= dog.sighty ):
            dog.sheeps.append( (target.x,target.y) )
            print("Moutons trouvés : ",dog.sheeps)"""


def is_on_circuit(x,y,circuit):
    to_return = False
    i = 1
    while( i < circuit.size and not(to_return) ):
        if( circuit.get_point(i)[0] == circuit.get_point(i-1)[0] ):
            if(x == circuit.get_point(i)[0]):
                print("trouvé x",x)
                to_return = True
        if( circuit.get_point(i)[1] == circuit.get_point(i-1)[1] ):
            if(y == circuit.get_point(i)[1]):
                print("trouvéy",y)
                to_return = True
        i += 1
    if( circuit.get_point(0)[0] == circuit.get_point(circuit.size-1)[0] ):
        if(x == circuit.get_point(0)[0]):
            to_return = True
    if( circuit.get_point(0)[1] == circuit.get_point(circuit.size-1)[1] ):
        if(y == circuit.get_point(0)[1]):
            to_return = True
    return to_return


#BRINGER DOG AI
#def dog_bringer_movement(dog):
    
#SHEEP AI
def sheep_movement(sheep):
    if(sheep.x == 60):
        print('meh',sheep.x)
    sheepfold_distance = sqrt((sheep.x - sheepfold_center[0])**2 + (sheep.y - sheepfold_center[1])**2)
    if(sheepfold_distance < sheepfold_radius):
        vx = (sheep.speed * (sheep.x - sheepfold_center[0]) ) / sheepfold_distance
        vy = (sheep.speed * (sheep.y - sheepfold_center[1]) ) / sheepfold_distance
    else:
        speed_sum = 0
        vx_sum = 0
        vy_sum = 0
        for i in range(0,len(DOGS_LIST) ):
            dog = DOGS_LIST[i]
            dx = sheep.x - dog.x
            dy = sheep.y - dog.y
            dog_distance = sqrt(dx**2 + dy**2)
                
            if( dog_distance <= dog.radius + MARGIN ):
                speed_sum += dog.speed 
                vx_sum += (sheep.speed * dx) / dog_distance
                vy_sum += (sheep.speed * dy) / dog_distance

        if(speed_sum != 0):
            sheep.x += vx_sum/abs(speed_sum)
            sheep.y += vy_sum/abs(speed_sum)
            if(sheep.x > MAX_WIDTH):
                sheep.x = MAX_WIDTH
            if(sheep.y > MAX_HEIGHT):
                sheep.y = MAX_HEIGHT
            if(sheep.x < 0):
                sheep.x = 0
            if(sheep.y < 0):
                sheep.y = 0
    
                
        


#========== Animations and buttons ==========

#Movement function

def position_to_speed(posx,posy,targetx,targety,speed):
    dx = targetx - posx
    dy = targety - posy
    distance = sqrt(dx**2 + dy**2)
    vx = (speed * dx) / distance
    vy = (speed * dy) / distance
    return (vx,vy)


#Frames refreshing
def animate():
    global STOP, blue_circuit
    if(STOP == False):
        for i in range(0,len(DOGS_LIST) ):
            
            if(DOGS_LIST[i].color == 'blue'):
                dog_scout_movement(DOGS_LIST[i],blue_circuit)
                
            #if(DOGS_LIST[i].color == 'yellow' or DOGS_LIST[i].color == 'green'):
            #    dog_bringer_movement(DOGS_LIST[i])

            #if(DOGS_LIST[i].color == 'purple'):
            #    dog_bringer_movement2(DOGS_LIST[i])
            
                
            X = DOGS_LIST[i].x
            Y = DOGS_LIST[i].y
                
            Board.coords(DOGS_LIST[i].texture,X-POINT_SIZE,Y-POINT_SIZE,X+POINT_SIZE,Y+POINT_SIZE)

        for j in range(0,len(SHEEPS_LIST) ):
            sheep_movement(SHEEPS_LIST[j])
            
            X = SHEEPS_LIST[j].x
            Y = SHEEPS_LIST[j].y
            
            Board.coords(SHEEPS_LIST[j].texture,X-POINT_SIZE,Y-POINT_SIZE,X+POINT_SIZE,Y+POINT_SIZE)
            
        Window.after(50,animate)

#Buttons functions
def start_anim():
    global STOP
    if( STOP == True ):
        STOP = False
        animate()
        
 
def stop_anim():
    global STOP
    STOP = True

    
#========== Main ==========

#Window Initialisation
Window = Tk()
Window.title("Simulation Ogor.io")
Board = Canvas(Window, width=MAX_WIDTH,height=MAX_HEIGHT , bg = 'black')
Board.pack()

#SHEEPFOLD
sheepfold_radius = MAX_WIDTH/10
sheepfold_center = (0 , MAX_HEIGHT/2)
sheepfold = Board.create_oval(sheepfold_center[0] - sheepfold_radius,sheepfold_center[1] - sheepfold_radius,sheepfold_center[0] + sheepfold_radius,sheepfold_center[1] + sheepfold_radius, outline='gray', fill='gray') #a modifier avec ce qu'il y a au dessus

#BLUE CIRCUIT
Board.create_line(blue_zone_xmin, blue_zone_ymin, blue_zone_xmin,blue_zone_ymax,fill='light blue',width=3)

for i in range(1,blue_circuit.size):
    Board.create_line(blue_circuit.get_point(i-1)[0], blue_circuit.get_point(i-1)[1], blue_circuit.get_point(i)[0],blue_circuit.get_point(i)[1],fill='light blue',width=3)

Board.create_line(blue_circuit.get_point(blue_circuit.size-1)[0], blue_circuit.get_point(blue_circuit.size-1)[1], blue_circuit.get_point(0)[0],blue_circuit.get_point(0)[1],fill='light blue',width=3)

#START BUTTON
start_button = Button(Window,text='START',command = start_anim)
start_button.pack(side = LEFT , padx = 10)

#STOP BUTTON
stop_button = Button(Window,text='STOP',command = stop_anim)
stop_button.pack(side = LEFT , padx = 10)

#DOGE GENERATOR
dog_button = Button(Window,text='WOOF BLEU',command = generate_dog_blue)
dog_button.pack(side = LEFT , padx = 10)

dog_button = Button(Window,text='WOOF JAUNE',command = generate_dog_yellow)
dog_button.pack(side = LEFT , padx = 10)

dog_button = Button(Window,text='WOOF VERT',command = generate_dog_green)
dog_button.pack(side = LEFT , padx = 10)

dog_button = Button(Window,text='WOOF VIOLET',command = generate_dog_purple)
dog_button.pack(side = LEFT , padx = 10)

#SHEEP GENERATOR
sheep_button = Button(Window,text='MEEEH',command = generate_sheep)
sheep_button.pack(side = LEFT , padx = 10)

#GENERATE MAP
for i in range(0,9):
    generate_sheep()

#PERSONALISED GENERATION
"""dot = Board.create_oval(900-POINT_SIZE, 300-POINT_SIZE, 900+POINT_SIZE, 300+POINT_SIZE, outline='yellow', fill='yellow')
private_dog = Dot()
private_dog.init('yellow',900,300,dot)
DOGS_LIST.append(private_dog)"""


Window.mainloop()

