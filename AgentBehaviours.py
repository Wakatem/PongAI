from AgentHelper import *
import numpy as np
import random 

states_space = []
actions_space = []

def defineSpaces():

    #define states space

    #when game is not over AND sensor is off
    for columnID in range(10):
        states_space.append((True, True, False, columnID, True))        #even indices
        states_space.append((True, True, False, columnID, False))       #odd indices
    
    #when game is not over AND sensor is on
    for columnID in range(10):
        states_space.append((True, True, True, columnID, True))         #even indices
        states_space.append((True, True, True, columnID, False))        #odd indices

    goal_state = (True, False, False, -1, True);   states_space.append(goal_state)
    loss_state = (False, True, True, -1, True);   states_space.append(loss_state)


    #define actions space
    for i in range(10):
        actions_space.append('C' + str(i+1))


def determineState(inputs=()):

    stateNotFound = False

    #function used to determine state
    if len(inputs) == 0:
        livesLeft = (pd.lives != 0)
        scoreLeft = (pd.score < pd.scoreTarget)
        sensorActivated = isSensorActivated()
        vc = findVirtualColumn()
        ball_toUp = pd.ball_toUp

    #function used to find state
    else:
        livesLeft = inputs[1]
        scoreLeft = inputs[2]
        sensorActivated = inputs[3]
        vc = inputs[4]
        ball_toUp = inputs[5]
    
    if livesLeft:
        if scoreLeft:
            for state in states_space:
                if livesLeft is state[0]  and  scoreLeft is state[1]  and  sensorActivated is state[2]  and  vc is state[3] and ball_toUp is state[4]:
                    stateNotFound = True
                    return states_space.index(state)
        else:
            for state in states_space: 
                if state[0]==True  and  state[1]==False:
                    return states_space.index(state) #goal state
    else:   
        for state in states_space: 
                if state[0]==False  and  state[1]==True:
                    return states_space.index(state) #lost state

    if stateNotFound is True:
        print('nothing found, was going to c', action)
        print(livesLeft, scoreLeft, sensorActivated, new_state_column, ball_toUp)

      




def selectAction(QTable, explore, state):
    if explore is True:
        return (True, random.randint(0,9)) #return explored action index
    else:
        highest = 0.0
        ii = 0
        arr = QTable[state,:].tolist()
        for j in arr:
            if j > highest:
                highest = float(j)
                ii = arr.index(float(j))
            print('state is', state)
            #print(states_space[ii])
            #print(arr)
            #print(ii)
            #print();print();
        
        return (False,  ii) #return exploited action index

 
def performAction(action):
    reward = 0
    new_state = 0
    gameover = 0

    vColumn = virtual_columns[action[1]] # column to go to
    #print('go to ', vColumn)

    if action[0] == True:
        move(random.randint(vColumn[0], vColumn[1]))
    else:
        ballPos = pd.ballPos_horizontal
        for ac in actual_columns:
                if vColumn[0] <= ac[0] and ac[1] <= vColumn[1]:
                    if isColumnActivated(ac, ballPos):
                        move(ac[0])
                        break
    

    #observe the change in inputs
    livesLeft = (pd.lives != 0)
    scoreLeft = (pd.score < pd.scoreTarget)
    sensorActivated = isSensorActivated()
    newColumn = findVirtualColumn()
    ball_toUp = pd.ball_toUp


    
    #define reward
    if sensorActivated is True:
        reward = -1
    
    else:
        reward = 0 if ball_toUp is True else 5


    new_state = determineState((action, livesLeft, scoreLeft, sensorActivated, newColumn, ball_toUp))
    #print('action was: go to', action[1]+1)
    #print('new state is: ', states_space[new_state])
    #print();print();
    gameover = pd.gameover

    return (new_state, reward, gameover)


#////////////////////////////////////////
defineSpaces()