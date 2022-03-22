from AgentBehaviours import *

continueTraining = True
importQTable = True



if importQTable is True:
    QTable = readQTable()
    pass
else:
    QTable = np.zeros((42,10))


if continueTraining is True:
    numofEpisodes, currentEp, er = readTraining();
else:
    numofEpisodes = 10000
    currentEp = 500
    er = 1

learningRate = 0.95
discountRate = 0.05
exploration_decay_rate = 0.001   
min_exploration_rate = 0.01
max_exploration_rate = 1


#Q Learning Algorithm

totalEpisodesRewards = 0

try:
    while currentEp != numofEpisodes:
        updateEpisodes(currentEp+1, er)
        resetGame()
        state = determineState()    #state index
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
            new_state, reward, gameover = performAction(state, action)
            updateReward(actions_space[action[1]], reward)
    
            #update qtable
            QTable[state, action] = QTable[state, action] * (1 - learningRate) + learningRate * (reward + discountRate * np.max(QTable[new_state, :]))
    
            #update current state and rewards per ep
            state = new_state
            epRewards += reward
    
    
            if pd.closeGame is True:
                break
        
        er = min_exploration_rate + (max_exploration_rate - min_exploration_rate) * np.exp(-exploration_decay_rate*currentEp)
        totalEpisodesRewards += epRewards
        if pd.closeGame is True:
            writeQTable(QTable)
            writeTraining((numofEpisodes, currentEp, er))
            break
        else:
            if currentEp % 10 == 0:
                writeQTable(QTable)
        currentEp += 1

except Exception as e:
        exc_type, exc_obj, exc_tb = sys.exc_info()
        fname = os.path.split(exc_tb.tb_frame.f_code.co_filename)[1]
        print(exc_type, fname, exc_tb.tb_lineno)