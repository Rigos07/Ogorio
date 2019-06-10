#Projet IR 2019 : Ogar - jeu de type agar.io
#Simulation du jeu complet pour test d'ia
#Version 2.0
#Contient :
#Chiens sous formes de classes
#Generation du terrain
#IA des brebis
#IA des chiens rameneurs
#
#Lucas GHENIA


from tkinter import *
from random import *
from math import *

#========== Constants ==========
MAX_WIDTH = 900
MAX_HEIGHT = 600
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
            self.sight = 0
            
        if(color == "blue"):
            self.speed = 2.5
            self.radius = 0
            self.sight = 50
            
        if(color == "yellow"):
            self.speed = 2
            self.radius = 20
            self.sight = 30
            
        if(color == "green"):
            self.speed = 1.5
            self.radius = 30
            self.sight = 40
            
        if(color == "purple"):
            self.speed = 1
            self.radius = 20
            self.sight = 45 #wallah j'ai pas fait la vue rectangle


    def print(self):
        print("Dot : \ncolor = ",self.color,"\nspeed = ", self.speed,"\naction radius = ", self.radius,"\nsight surface = ", self.sight,"x",self.sight,"\nposition = ", self.x,",",self.y,"\nEND")
            
    

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
    dog = generate_point('blue')
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

#SCOUT DOG AI
def dog_scout_movement(dog):
    sheeps_in_sight = 0
    for i in range(0, len(SHEEPS_LIST) ):
        target = SHEEPS_LIST[i]
        dx = target.x - dog.x
        dy = target.y - dog.y
        if( abs(dx) < dog.sight and abs(dy) < dog.sight ):
            sheeps_in_sight += 1
            vx = (dog.speed * dx) / sqrt(dx**2 + dy**2)
            vy = (dog.speed * dy) / sqrt(dx**2 + dy**2)
            if(sqrt(dx**2 + dy**2) < 20):
                vx = - vx
                vy = - vy
                
            dog.x += vx
            dog.y += vy
            break
    if(sheeps_in_sight == 0):
        default_speed_x = dog.speed / sqrt(2)
        default_speed_y = dog.speed / sqrt(2)
        
        if(dog.x >= MAX_WIDTH or dog.x == 0):
            default_speed_x = - default_speed_x
            
        if(dog.y >= MAX_HEIGHT or dog.y == 0):
            default_speed_y = - default_speed_y
            
        dog.x += default_speed_x
        dog.y += default_speed_y



#BRINGER DOG AI
def dog_bringer_movement(dog):
    sheeps_in_sight = 0
    for i in range(0, len(SHEEPS_LIST) ):
        target = SHEEPS_LIST[i]
        dx = target.x - dog.x
        dy = target.y - dog.y
        if( abs(dx) < dog.sight and abs(dy) < dog.sight ):
            sheeps_in_sight += 1
            dest_dx = target.x - sheepfold_center[0]
            dest_dy = target.y - sheepfold_center[1]
            dest_distance = sqrt(dest_dx**2 + dest_dy**2)

            delta_x = (dog.radius * dest_dx) / dest_distance
            delta_y = (dog.radius * dest_dy) / dest_distance
                
            r = 10
            distance = sqrt( (dog.x - target.x - delta_x)**2 + (dog.y - target.y - delta_y)**2 )
            if(distance != 0):
                (vx , vy) = position_to_speed(dog.x,dog.y,target.x + delta_x ,target.y + delta_y ,dog.speed)
            else:
                vx = 0
                vy = 0 
            dog.x += vx
            dog.y += vy
            break
        
    #Roaming behavior
    if(sheeps_in_sight == 0):
        default_speed_x = dog.speed / sqrt(2)
        default_speed_y = dog.speed / sqrt(2)
        
        if(dog.x >= MAX_WIDTH or dog.x == 0):
            default_speed_x = - default_speed_x
            
        if(dog.y >= MAX_HEIGHT or dog.y == 0):
            default_speed_y = - default_speed_y
            
        dog.x += default_speed_x
        dog.y += default_speed_y


# 2 POINTS VERSION
def dog_bringer_movement2(dog):
    sheeps_in_sight = 0
    for i in range(0, len(SHEEPS_LIST) ):
        target = SHEEPS_LIST[i]
        dx = target.x - dog.x
        dy = target.y - dog.y
        if( abs(dx) < dog.sight and abs(dy) < dog.sight ): #Sheep targeting behavior
            sheeps_in_sight += 1
            dest_dx = target.x - sheepfold_center[0]
            dest_dy = target.y - sheepfold_center[1]
            dest_distance = sqrt(dest_dx**2 + dest_dy**2)
            
            delta_x = (dog.radius * dest_dx) / dest_distance #<= FINAL OBJECTIVE POINT
            delta_y = (dog.radius * dest_dy) / dest_distance
            
            delta_x1 = -(dog.radius * dest_dy) / dest_distance #<= FIRST INTERMEDIATE POINT
            delta_y1 = (dog.radius * dest_dx) / dest_distance

            delta_x2 = (dog.radius * dest_dy) / dest_distance #<= SECOND INTERMEDIATE POINT
            delta_y2 = -(dog.radius * dest_dx) / dest_distance

            if(target.x + delta_x1 < 0):
                delta_x1 = - target.x
            if(target.x + delta_x2 < 0):
                delta_x2 = - target.x
                              
            r = 10
            distance = sqrt( (dog.x - target.x - delta_x)**2 + (dog.y - target.y - delta_y)**2 )
            distance_1 = sqrt( (dog.x - target.x - delta_x1)**2 + (dog.y - target.y - delta_y1)**2 )
            distance_2 = sqrt( (dog.x - target.x - delta_x2)**2 + (dog.y - target.y - delta_y2)**2 )

            #Affichage des 3 points
            #Board.create_oval(target.x + delta_x - r, target.y + delta_y - r, target.x + delta_x + r, target.y + delta_y + r, outline='red', fill='red') #Point objectif
            #Board.create_oval(target.x + delta_x1 - r, target.y + delta_y1 - r, target.x + delta_x1 + r, target.y + delta_y1 + r, outline='light green', fill='light green') #point 1
            #Board.create_oval(target.x + delta_x2 - r, target.y + delta_y2 - r, target.x + delta_x2 + r, target.y + delta_y2 + r, outline='light blue', fill='light blue') #point 2

            distance_dog_to_sheepfold = sqrt( (dog.x - sheepfold_center[0])**2 +(dog.y - sheepfold_center[1])**2 )
            distance_sheep_to_sheepfold = sqrt( (target.x - sheepfold_center[0])**2 + (target.y - sheepfold_center[1])**2)
        
            is_behind_sheep = (distance_dog_to_sheepfold > distance_sheep_to_sheepfold)

            if( is_behind_sheep and distance != 0 ):
                (vx , vy) = position_to_speed(dog.x,dog.y,target.x + delta_x ,target.y + delta_y ,dog.speed) #directly go to objective point
            else:
                if(distance == 0): #if point has been reached
                    vx = 0
                    vy = 0

                #Going to the closest intermediate point
                if(distance_1 >= distance_2):
                    (vx , vy) = position_to_speed(dog.x,dog.y,target.x + delta_x2 ,target.y + delta_y2 ,dog.speed) 
            
                if(distance_1 < distance_2):
                    (vx , vy) = position_to_speed(dog.x,dog.y,target.x + delta_x1 ,target.y + delta_y1 ,dog.speed)

            dog.x += vx
            dog.y += vy
            break
    #Roaming behavior
    if(sheeps_in_sight == 0):
        default_speed_x = dog.speed / sqrt(2)
        default_speed_y = dog.speed / sqrt(2)
        
        if(dog.x >= MAX_WIDTH or dog.x == 0):
            default_speed_x = - default_speed_x
            
        if(dog.y >= MAX_HEIGHT or dog.y == 0):
            default_speed_y = - default_speed_y
            
        dog.x += default_speed_x
        dog.y += default_speed_y


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
    global STOP
    if(STOP == False):
        for i in range(0,len(DOGS_LIST) ):
            
            if(DOGS_LIST[i].color == 'blue'):
                dog_scout_movement(DOGS_LIST[i])
                
            if(DOGS_LIST[i].color == 'yellow' or DOGS_LIST[i].color == 'green'):
                dog_bringer_movement(DOGS_LIST[i])

            if(DOGS_LIST[i].color == 'purple'):
                dog_bringer_movement2(DOGS_LIST[i])
            
                
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
