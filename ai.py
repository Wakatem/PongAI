from AgentActions import *

numofEpisodes = 1000
states_space = ()
actions_space = ()
QTable = 0

learningRate = 0
discountRate = 0
explorationRate = 0
EDRate = 0           #exploration decay rate




#\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

     

def defineSpaces():

    #define states space
    for i in range(len(columns)):
        states_space += (True, True, False, i)

    for i in range(len(columns)):
        states_space += (True, True, True, i)

    goal_state = (True, False, False, -1);   states_space += goal_state;
    loss_state = (False, True, False, -1);   states_space += loss_state;


    #define actions space


    
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

while True:
    print(pd.ballPos_vertical)
   


