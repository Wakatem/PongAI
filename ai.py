from AgentActions import *
import numpy as np
import random 

numofEpisodes = 1000
states_space = []
actions_space = []

QTable = np.zeros((22,10))

learningRate = 0
discountRate = 0
explorationRate = 1
exploration_decay_rate = 0.001   
min_exploration_rate = 0.01
max_exploration_rate = 1


#\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

     
def defineSpaces():

    #define states space

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

    
def selectAction(explore, state):
    if explore is True:
        return random.randint(0,9) #return index
    else:
        return np.argmax(QTable[state,:]) 

        

def findState():
    livesLeft = (pd.lives != 0)
    scoreLeft = (pd.score != pd.scoreTarget)
    sensorActivated = isSensorActivated()
    vc = findVirtualColumn()
    
    if livesLeft:
        if scoreLeft:
            for state in states_space:
                if livesLeft == state[0]  and  scoreLeft == state[1]  and  sensorActivated == state[2]  and  vc == state[3]:
                    return states_space.index(state)
        else:
            for state in states_space: 
                if state[0]==True  and  state[1]==False:
                    return states_space.index(state) #goal state
    else:   
        for state in states_space: 
                if state[0]==False  and  state[1]==True:
                    return states_space.index(state) #lost state

 
def QLearning():
    for episode in range(numofEpisodes):

        #each episode - restart environment automatically
        current_rewards = 0
        while pd.gameover == False:
            state = findState()
            explorationRateThreshold = random.uniform(0,1)
            action = 0
            if explorationRateThreshold > explorationRate:
                action = selectAction(False, state)
            else:
                action = selectAction(True, state)

            performAction(action)
            #update qtable

    explorationRate = min_exploration_rate + (max_exploration_rate - min_exploration_rate) * np.exp(-exploration_decay_rate*episode)

            


#/////////////////////////////////////////////////////////////////////////
 
defineSpaces()

#print(tabulate(QTable, tablefmt="grid"))








