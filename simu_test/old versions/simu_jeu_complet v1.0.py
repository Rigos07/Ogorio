#Projet IR 2019 : Ogar - jeu de type agar.io
#Simulation du jeu complet pour test d'ia
#Version 1.0
#Lucas GHENIA

from tkinter import *
from random import *


#========== Constants ==========
MAX_WIDTH = 900
MAX_HEIGHT = 600
POINT_SIZE = 2
STOP = True
DOGS_LIST = []
SHEEPS_LIST = []



#========== Generating functions ==========

def generate_point(color):
    posx = randint(0,MAX_WIDTH)
    posy = randint(0,MAX_HEIGHT)
    radius = POINT_SIZE
    color = color
    dot = Board.create_oval(posx-radius, posy-radius, posx+radius, posy+radius, outline=color, fill=color)
    dot_object = [dot,posx,posy,color]
    return dot_object

def generate_dog_blue():
    dog = generate_point('blue')
    DOGS_LIST.append(dog)
    
def generate_dog_yellow():
    dog = generate_point('yellow')
    DOGS_LIST.append(dog)

def generate_sheep():
    sheep = generate_point('white')
    SHEEPS_LIST.append(sheep)


#=========== DOGS AIS ==========

#SCOUT DOG AI
def dog_scout_movement(posx,posy):
    if( len(SHEEPS_LIST) != 0):
        target = SHEEPS_LIST[0]
        x_speed = 5
        y_speed = 5
        if( posx - target[1] > 0):
            x_speed = -10
        if (posx - target[1] == 0):
            x_speed = 0

        if( posy - target[2] > 0):
            y_speed = -10
        if (posy - target[2] == 0):
            y_speed = 0

        posx += x_speed
        posy += y_speed
    return (posx,posy)



#BRINGER DOG AI
def dog_bringer_movement(posx,posy):
    if( len(SHEEPS_LIST) != 0):
        target = SHEEPS_LIST[0]
        if( (abs(posx - target[1]) > 30) or (abs(posy - target[2]) > 30)):
            x_speed = 2
            y_speed = 2
            if( posx - target[1] > 0):
                x_speed = -2
            if (posx - target[1] == 0):
                x_speed = 0

            if( posy - target[2] > 0):
                y_speed = -2
            if (posy - target[2] == 0):
                y_speed = 0   
            posx += x_speed
            posy += y_speed
    return (posx,posy)



#SHEEP AI
def sheep_movement(posx,posy):
    if(posx == 60):
        print('meh')
    return (posx,posy)


#========== Animations and buttons ==========

#Frames refreshing
def animate():
    global STOP
    if(STOP == False):
        for i in range(0,len(DOGS_LIST) ):
            
            if(DOGS_LIST[i][3] == 'blue'):
                ( DOGS_LIST[i][1] , DOGS_LIST[i][2] ) = dog_scout_movement(DOGS_LIST[i][1],DOGS_LIST[i][2])
                
            if(DOGS_LIST[i][3] == 'yellow'):
                ( DOGS_LIST[i][1] , DOGS_LIST[i][2] ) = dog_bringer_movement(DOGS_LIST[i][1],DOGS_LIST[i][2])
                
            X = DOGS_LIST[i][1]
            Y = DOGS_LIST[i][2]
            Board.coords(DOGS_LIST[i][0],X-POINT_SIZE,Y-POINT_SIZE,X+POINT_SIZE,Y+POINT_SIZE)

        for j in range(0,len(SHEEPS_LIST) ):
            vitesse = 5
            ( SHEEPS_LIST[j][1] , SHEEPS_LIST[j][2] ) = sheep_movement(SHEEPS_LIST[j][1],SHEEPS_LIST[j][2]) #ichwallah c moche mais c'est pour récup le tuple
            X = SHEEPS_LIST[j][1]
            Y = SHEEPS_LIST[j][2]
            Board.coords(SHEEPS_LIST[j][0],X-POINT_SIZE,Y-POINT_SIZE,X+POINT_SIZE,Y+POINT_SIZE)
            
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

#SHEEP GENERATOR
sheep_button = Button(Window,text='MEEEH',command = generate_sheep)
sheep_button.pack(side = LEFT , padx = 10)

#GENERATE MAP
"""
for i in range(0,10):
    generate_sheep()
"""
#test
"""
generate_point('blue',POINT_SIZE)

posx = randint(0,MAX_WIDTH)
posy = randint(0,MAX_HEIGHT)
radius = POINT_SIZE
color = 'yellow'
dog = Board.create_oval(posx-radius, posy-radius, posx+radius, posy+radius, outline=color, fill=color)
DOG_LIST.append(dog)
DOG_LIST.append(posx)
DOG_LIST.append(posy)

animate()"""

Window.mainloop()
