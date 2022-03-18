from AgentActions import *

numofEpisodes = 1000
states_space = []
actions_space = []
QTable = 0

learningRate = 0
discountRate = 0
explorationRate = 0
EDRate = 0           #exploration decay rate




#\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

     

def defineSpaces():

    #define states space
    columnID = 0

    #when game is not over AND sensor is off
    for columnID in range(10):
        states_space.append((True, True, False, columnID))
    
    #when game is not over AND sensor is on
    for columnID in range(10):
        states_space.append((True, True, True, columnID))

    goal_state = (True, False, False, -1);   states_space.append(goal_state);
    loss_state = (False, True, False, -1);   states_space.append(loss_state);


    #define actions space
    actions_space = ["C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "C10"]

    
def selectAction(explore):
    if explore is True:
        pass #return index
    else:
        pass #return index



def findState():
    #call isColumnActivated
    pass


def QLearning():
    for i in range(numofEpisodes):
        while pd.gameover == False:
            pass

#/////////////////////////////////////////////////////////////////////////
 
defineSpaces()



