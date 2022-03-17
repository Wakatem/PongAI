from PongGame import PongDetails as pd

stateSpaces = ()
actionSpaces = ()
QTable = 0

learningRate = 0
discountRate = 0
explorationRate = 0
EDRate = 0  #exploration decay rate

columns = []

def countColumns():
    
    starting_x = 0
    current_x = 0
    i=1

    while current_x <= pd.windowWidth:    
        if i%2==0:
            current_x += pd.batwidth
            columns.append((starting_x,current_x))
        else:
            starting_x = current_x
        i+=1

countColumns()

#for column in columns:
 #   print(column[])

def columnActivation(column_index):
    i=0
    for pair in columns:
        if pair[0] <= pd.ballPos and pd.ballPos <= pair[1]:
            return i
    i+=1
   


def defineSpaces():
    for i in range(len(columns)):
        columns += (True, True, False, i)

    for i in range(len(columns)):
        columns += (True, True, True, i)

    goal_state = (True, False, False, -1)
    lost_state = (False, True, False, -1)

def selectAction():
    pass

def findState():
    pass
    #call columnActivation


   
















