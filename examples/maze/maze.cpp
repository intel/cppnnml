/**
* Copyright (c) 2020 Intel Corporation
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*/

/*
Q-Learning unit test. Learn the best path out of a simple maze.

5 == Outside the maze
________________________________________________
|                       |                       |
|                       |                       |
|           0           |          1             / 5
|                       |                       |
|____________/  ________|__/  __________________|_______________________
|                       |                       |                       |
|                       |                        /                      |
|           4           |          3            |           2           |
|                        /                      |                       |
|__/  __________________|_______________________|_______________________|
    5

The paths out of the maze:

0->4->5
0->4->3->1->5
1->5
1->3->4->5
2->3->1->5
2->3->4->5
3->1->5
3->4->5
4->5
4->3->1->5
*/

#include <iostream>
#include <fstream>
#include <string>

#include "mazelearner.h"

extern QLearnerType qLearner;
extern size_t randomActionDecisionPoint;

int main(const int argc, char *argv[])
{
    using namespace std;
    state_t state;
    action_t action;
    typename MazeEnvironmentType::ParentType::experience_t experience;
    string logEntry;
    ofstream logFile;

    logFile.open("maze.txt");
    if(!logFile.is_open())
    {
        cerr << "Failed to open output file." << endl;
        return -1;
    }

    // Init reward table
    for(state = 0;state < NUMBER_OF_STATES;++state)
    {
        for(action = 0;action < NUMBER_OF_ACTIONS;++action)
        {
            if (qLearner.getEnvironment().isActionValidForState(state, action))
            {
                if (5 == action)
                {
                    qLearner.getEnvironment().setRewardForStateAndAction(state, action, MazeEnvironmentType::EnvironmentRewardValue);
                }
                else
                {
                    qLearner.getEnvironment().setRewardForStateAndAction(state, action, MazeEnvironmentType::EnvironmentNoRewardValue);
                }
            }
            else
            {
                qLearner.getEnvironment().setRewardForStateAndAction(state, action, MazeEnvironmentType::EnvironmentInvalidActionValue);
            }
        }
    }

    qLearner.getEnvironment().setRandomActionDecisionPoint(randomActionDecisionPoint);
    qLearner.getEnvironment().setGoalState(5);

    // randomly search the maze for the reward, keep updating the Q table
    for (auto i = 0; i < 500; ++i)
    {
        // after 400 random iterations, scale down the randomness on every iteration
        if (i >= 400)
        {
            if (randomActionDecisionPoint > 0)
            {
                --randomActionDecisionPoint;
                qLearner.getEnvironment().setRandomActionDecisionPoint(randomActionDecisionPoint);
            }
        }

        qLearner.startNewEpisode();

        state = rand() % NUMBER_OF_STATES;
        cout << "*** starting in state " << (int)state << " ***" << endl;
        logEntry.clear();
        logEntry += to_string(state);
        logEntry += ",";
        action = qLearner.takeAction(state);
        cout << "take action " << (int)action << endl;
        logEntry += to_string(action);
        logEntry += ",";
        experience.state = state;
        experience.action = action;
        experience.reward = qLearner.getEnvironment().getRewardForStateAndAction(experience.state, experience.action);
        experience.newState =  static_cast<state_t>(experience.action);
        qLearner.updateFromExperience(experience);

        // look until we find the cheese
        while (qLearner.getState() != qLearner.getEnvironment().getGoalState())
        {
            action = qLearner.takeAction(qLearner.getState());
            cout << "take action " << (int)action << endl;
            logEntry += to_string(action);
            logEntry += ",";
            experience.state = qLearner.getState();
            experience.action = action;
            experience.reward = qLearner.getEnvironment().getRewardForStateAndAction(experience.state, experience.action);
            experience.newState =  static_cast<state_t>(experience.action);
            qLearner.updateFromExperience(experience);
        }

        logFile << logEntry << endl;
    }

    // trainging is done, now run some test iterations
    for (auto i = 0; i < 100; ++i)
    {
        qLearner.startNewEpisode();

        state = rand() % NUMBER_OF_STATES;
        cout << "*** starting in state " << (int)state << " ***" << endl;
        logEntry.clear();
        logEntry += to_string(state);
        logEntry += ",";
        action = qLearner.takeAction(state);
        cout << "take action " << (int)action << endl;
        logEntry += to_string(action);
        logEntry += ",";
        experience.state = state;
        experience.action = action;
        experience.reward = qLearner.getEnvironment().getRewardForStateAndAction(experience.state, experience.action);
        experience.newState =  static_cast<state_t>(experience.action);
        qLearner.updateFromExperience(experience);

        // look until we find the cheese
        while (qLearner.getState() != qLearner.getEnvironment().getGoalState())
        {
            action = qLearner.takeAction(qLearner.getState());
            cout << "take action " << (int)action << endl;
            logEntry += to_string(action);
            logEntry += ",";
            experience.state = qLearner.getState();
            experience.action = action;
            experience.reward = qLearner.getEnvironment().getRewardForStateAndAction(experience.state, experience.action);
            experience.newState =  static_cast<state_t>(experience.action);
            qLearner.updateFromExperience(experience);
        }

        logFile << logEntry << endl;
    }

    return 0;
}