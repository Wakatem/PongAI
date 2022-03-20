from AgentBehaviours import *
importQTable = False


if importQTable is True:
    QTable = readQTable()
    pass
else:
    QTable = np.zeros((42,10))

numofEpisodes = 10
learningRate = 1
discountRate = 0
exploration_decay_rate = 0.0001   
min_exploration_rate = 0.01
max_exploration_rate = 1
er = 1.0


#Q Learning Algorithm

totalEpisodesRewards = 0

for episode in range(numofEpisodes):
    updateEpisodes(episode+1, er)
    resetGame()
    state = determineState()
    epRewards = 0
    gameover = False


    while gameover is False:
        explorationRateThreshold = random.uniform(0,1)
        action = 0
        if explorationRateThreshold > er:
            updateActionStrategy('Exploit')
            action = selectAction(QTable, False, state)     #exploit

        else:
            updateActionStrategy('Explore')
            action = selectAction(QTable, True, state)      #explore

        #execute action and observe environment
        new_state, reward, gameover = performAction(action)
        updateReward(actions_space[action[1]], reward)

        #update qtable
        QTable[state, action] = QTable[state, action] * (1 - learningRate) + learningRate * (reward + discountRate * np.max(QTable[new_state, :]))

        #update current state and rewards per ep
        state = new_state
        epRewards += reward



    er = min_exploration_rate + (max_exploration_rate - min_exploration_rate) * np.exp(-exploration_decay_rate*episode)
    totalEpisodesRewards += epRewards
    
    

#after N episodes, save the qtable
writeQTable(QTable)
    
    










