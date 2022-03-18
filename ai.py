from AgentActions import *

numofEpisodes = 1000
states_space = []
actions_space = []
QTable = 0

learningRate = 0
discountRate = 0
explorationRate = 1
exploration_decay_rate = 0.001          


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

    
def selectAction(explore):
    if explore is True:
        pass #return index
    else:
        pass #return index



def findState():
    i = 0
    vc = findVirtualColumn()
    for state in range(len(states_space)):
        if (pd.lives != 0) == state[0]:
            if (pd.score != pd.scoreTarget) == state[1]:
                if isSensorActivated == state[2]:
                    if vc == state[3]:
                        return states_space.index(state)





 
def QLearning():
    for i in range(numofEpisodes):

        #each episode - restart environment automatically
        current_rewards = 0
        while pd.gameover == False:
            findState()
            explorationRateThreshold = random.uniform(0,1)
            action = 0
            if explorationRateThreshold > explorationRate:
                action = selectAction(False)
            else:
                action = selectAction(True)

            performAction(action)

            


#/////////////////////////////////////////////////////////////////////////
 
defineSpaces()



