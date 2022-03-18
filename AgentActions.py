from PongGame import PongDetails as pd

actual_columns =  []        #columns found based on screen width
virtual_columns = [] #10 virtual columns


def countColumns():
    starting_x = 0
    ending_x = 0
    i=1

    while True: 
        
        if ending_x+pd.batWidth <= pd.windowWidth:
            ending_x += pd.batWidth

            #add current x as ending position and save the pair
            actual_columns.append((starting_x,ending_x))
                
            #next column starting x is the current column ending x
            starting_x = ending_x
        
        else:
            #calculate the remaining odd ending_x (if there is)
            diff = pd.windowWidth - ending_x
            ending_x += diff

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
        
        print(virtual_columns[vColumn])
        

def isColumnActivated(column_index):
    i=0
    for pair in actual_columns:
        if pair[0] <= pd.ballPos_horizontal and pd.ballPos_horizontal <= pair[1]:
            return i
    i+=1
 
def isSensorActivated():
    if pd.ballPos_vertical > pd.batY:
        return True
    else:
        return False


def performAction(action_index):
    pass


# ================================================
countColumns()
mapColumns()