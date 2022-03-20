from PongGame import PongDetails as pd
from PongGame import *
import numpy as np
import sys, os

actual_columns =  []        #columns found based on screen width
virtual_columns = []        #10 virtual columns
previous_column_index = -1

def countColumns():
    starting_x = 0
    ending_x = 0
    i=1

    while True: 
        
        if int(ending_x+pd.batWidth) <= pd.windowWidth:
            ending_x += int(pd.batWidth)

            #add current x as ending position and save the pair
            actual_columns.append((starting_x,ending_x))
                
            #next column starting x is the current column ending x
            starting_x = ending_x
        
        else:
            #calculate the remaining odd ending_x (if there is)
            diff = pd.windowWidth - ending_x
            ending_x += int(diff)

            actual_columns.append((starting_x,ending_x))
            break;
        i+=1



def mapColumns():
    steps = int(len(actual_columns) / 10)  #total window columns to step into per virtual column
    starting_x = 0                  #starting x value of X virtual column
    ending_x = 0                    #ending x value of X virtual column
    
    i = 0       #columns index

    for vColumn in range(10):

        if vColumn == 9:
            #map remaining columns to the last virtual column
            starting_x = actual_columns[i][0]
            while i != len(actual_columns)-1: i+=1
            ending_x = actual_columns[i][1]

            #add virtual column tuple to list
            virtual_columns.append((starting_x, ending_x))

        else:
            for step in range(steps): 
              #get starting x from first column only
              if step == 0:
                starting_x = actual_columns[i][0]
              i+=1 

            ending_x = actual_columns[i-1][1]

            #add virtual column tuple to list
            virtual_columns.append((starting_x, ending_x))
        
        #print(virtual_columns[vColumn])
        

def isColumnActivated(actualColumn, ballPos):
    if actualColumn[0] <= ballPos and ballPos <= actualColumn[1]:
        return True
    else:
        return False




def findVirtualColumn():
    #update ball's vertical position status on every frame
    global previous_column_index
    try:
        if pd.ballPos_vertical > pd.batY:
            updateIfBallIsBackUp(True)
        for vc in virtual_columns:
            if vc[0] <= pd.ballPos_horizontal and pd.ballPos_horizontal <= vc[1]:
                previous_column_index = virtual_columns.index(vc)
                return virtual_columns.index(vc)

        if pd.ball_toLeft is True:
            return previous_column_index - 1
        else:
            return previous_column_index + 1

        print('unkown position, ball at: ', pd.ballPos_horizontal)
    except:
        exc_type, exc_obj, exc_tb = sys.exc_info()
        fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
        print(exc_type, fname, exc_tb.tb_lineno)




def isSensorActivated():
    if pd.ballIsBackUp == True:
        if pd.ballPos_vertical > pd.batY:
            updateIfBallIsBackUp(False) #the ball now remains within the sensor until X timeframes
            return True
    return False


     
def writeQTable(QTable):
    np.savetxt('qtable.txt', QTable, delimiter=',')

def readQTable():
    return np.loadtxt('qtable.txt', dtype=float, delimiter=',')


# ================================================
countColumns()
mapColumns()

