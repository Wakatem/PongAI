from AgentHelper import *
import numpy as np
import random 

numofEpisodes = 1000
states_space = []
actions_space = []

QTable = np.zeros((32,10))

learningRate = 1
discountRate = 0
exploration_decay_rate = 0.01   
min_exploration_rate = 0.01
max_exploration_rate = 1


#\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

     
def defineSpaces():

    #define states space

    #when game is not over AND sensor is off
    for columnID in range(10):
        states_space.append((True, True, False, columnID, True))
        states_space.append((True, True, False, columnID, False))
    
    #when game is not over AND sensor is on
    for columnID in range(10):
        states_space.append((True, True, True, columnID, True))

    goal_state = (True, False, False, -1, True);   states_space.append(goal_state);
    loss_state = (False, True, True, -1, True);   states_space.append(loss_state);


    #define actions space
    actions_space = ["C1", "C2", "C3", "C4", "C5", "C6", "C7", "C8", "C9", "C10"]


def determineState():
    livesLeft = (pd.lives != 0)
    scoreLeft = (pd.score != pd.scoreTarget)
    sensorActivated = isSensorActivated()
    ball_toUp = pd.ball_toUp
    vc = findVirtualColumn()
    
    if livesLeft:
        if scoreLeft:
            for state in states_space:
                if livesLeft is state[0]  and  scoreLeft is state[1]  and  sensorActivated is state[2]  and  vc is state[3] and ball_toUp is state[4]:
                    return states_space.index(state)
        else:
            for state in states_space: 
                if state[0]==True  and  state[1]==False:
                    return states_space.index(state) #goal state
    else:   
        for state in states_space: 
                if state[0]==False  and  state[1]==True:
                    return states_space.index(state) #lost state


def findState(livesLeft, scoreLeft, sensorActivated, new_state_column, ball_toUp):
      for state in states_space:
          if livesLeft is state[0]  and  scoreLeft is state[1]  and  sensorActivated is state[2]  and  new_state_column is state[3] and ball_toUp is state[4]:
              return states_space.index(state) 




def selectAction(explore, state):
    if explore is True:
        return (True, random.randint(0,9)) #return explored action index
    else:
        highest = 0;
        index = 0;
        for j in QTable:
            if QTable[state, index] > highest:
                highest = QTable[state, highest]
                index = j

        return (False,  index) #return exploited action index

 
def performAction(state, action):
    reward = 0
    new_state = 0
    gameover = 0

    if action[0] is False:
        print(action)
    vColumn = virtual_columns[action[1]] # column to go to

    for ac in actual_columns:
            if action[0] == True:
                move(random.randint(vColumn[0], vColumn[1]))
            else:
                if vColumn[0] <= ac[0] and ac[1] <= vColumn[1]:
                    if isColumnActivated(ac):
                        move(ac[0])
                        break
    

    #observe the change in inputs
    livesLeft = (pd.lives != 0)
    scoreLeft = (pd.score != pd.scoreTarget)
    sensorActivated = isSensorActivated()
    newColumn = findVirtualColumn()
    ball_toUp = pd.ball_toUp


    
    #define reward
    if sensorActivated is True:
        reward = -1
    
    else:
        reward = 0 if ball_toUp is True else 5


    new_state = findState(livesLeft, scoreLeft, sensorActivated, newColumn, ball_toUp)
    gameover = pd.gameover

    return (new_state, reward, gameover)



def QLearning():
    er = 1
    totalEpisodesRewards = 0

    for episode in range(numofEpisodes):
        print('ep : ', episode)
        resetGame()
        state = determineState()
        epRewards = 0
        gameover = False


        while gameover is False:
            explorationRateThreshold = random.uniform(0,1)
            action = 0
            if explorationRateThreshold > er:
                action = selectAction(False, state)     #exploit
                print('exploit')
                print(action)
                print(states_space[action[1]])
            else:
                action = selectAction(True, state)      #explore

            #execute action and observe environment
            new_state, reward, gameover = performAction(state, action)

            #update qtable
            QTable[state, action] = QTable[state, action] * (1 - learningRate) + learningRate * (reward + discountRate * np.max(QTable[new_state, :]))

            #update current state and rewards per ep
            state = new_state
            epRewards += reward



        #print(er)
        er = min_exploration_rate + (max_exploration_rate - min_exploration_rate) * np.exp(-exploration_decay_rate*episode)
        #totalEpisodesRewards += epRewards
        #print(QTable)

            


#/////////////////////////////////////////////////////////////////////////
 
defineSpaces()
QLearning()







