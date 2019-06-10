from tkinter import *
from random import *

"""
Constants
"""
MAX_WIDTH = 900
MAX_HEIGHT = 600
POINT_SIZE = 2
STOP = True
STARS_LIST = []
color = 'yellow'

def generate_star():
    posx = randint(0,MAX_WIDTH/2)
    posy = randint(0,MAX_HEIGHT/2)
    radius = POINT_SIZE
    dot = Board.create_oval(posx-radius, posy-radius, posx+radius, posy+radius, outline=color, fill=color)
    dot_object = [dot,posx,posy,color]
    STARS_LIST.append(dot_object)
    

def animate():
    global STOP
    if(STOP == False):
        TO_DEL = []
        generate_star()
        for i in range(0,len(STARS_LIST) ):
            STARS_LIST[i][1] += 10
            STARS_LIST[i][2] += 10
            X = STARS_LIST[i][1]
            Y = STARS_LIST[i][2]
            if(X > MAX_WIDTH or Y > MAX_HEIGHT):
                TO_DEL.append(i)
            else:
                Board.coords(STARS_LIST[i][0],X-POINT_SIZE,Y-POINT_SIZE,X+POINT_SIZE,Y+POINT_SIZE)
        for j in range(0,len(TO_DEL) ):
            Board.delete(STARS_LIST[TO_DEL[j]][0])
            STARS_LIST.pop(TO_DEL[j])
            
        Window.after(50,animate)

def start_anim():
    global STOP
    if( STOP == True ):
        STOP = False
        animate()
        

def stop_anim():
    global STOP
    STOP = True

Window = Tk()
Board = Canvas(Window, width=MAX_WIDTH,height=MAX_HEIGHT , bg = 'black')
Board.pack()

#START BUTTON
start_button = Button(Window,text='START',command = start_anim)
start_button.pack(side = LEFT , padx = 10)

#STOP BUTTON
stop_button = Button(Window,text='STOP',command = stop_anim)
stop_button.pack(side = LEFT , padx = 10)


Window.mainloop()
