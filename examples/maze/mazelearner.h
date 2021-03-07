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

#pragma once

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <new>

#include "qformat.hpp"
#include "qlearn.hpp"
#include "constants.hpp"

// 6 rooms and 6 actions
#define NUMBER_OF_STATES 6
#define NUMBER_OF_ACTIONS 6

typedef uint8_t state_t;
typedef uint8_t action_t;

template<typename T>
struct MazeEnvironmentRandomNumberGeneratorPolicy
{
    MazeEnvironmentRandomNumberGeneratorPolicy()
    {
        srand(static_cast<unsigned int>(time(0))); // seed random number generator
    }

    size_t getRandomActionDecisionPoint() const
    {
        return this->mRandomActionDecisionPoint;
    }

    T randInt(const T min, const T max)
    {
        // Generate a random number between min..max
        const T value = (rand() % 
                                    (max +
                                    1 -
                                    min)) +
                                    min;
        return value;
    }

    void setRandomActionDecisionPoint(const size_t randomActionDecisionPoint)
    {
        if(randomActionDecisionPoint <= 100)
        {
            this->mRandomActionDecisionPoint = randomActionDecisionPoint;
        }
    }
private:
    size_t mRandomActionDecisionPoint;
};

template<   typename StateType,
            typename ActionType,
            typename ValueType,
            size_t NumberOfStates,
            size_t NumberOfActions,
            typename RewardPolicyType,
            template<typename> class QLearningPolicy = tinymind::DefaultLearningPolicy
        >
struct MazeEnvironment : public tinymind::QLearningEnvironment<state_t, action_t, ValueType, NumberOfStates, NumberOfActions, MazeEnvironmentRandomNumberGeneratorPolicy>
{
    typedef tinymind::QLearningEnvironment<StateType, ActionType, ValueType, NumberOfStates, NumberOfActions, MazeEnvironmentRandomNumberGeneratorPolicy> ParentType;
    static constexpr size_t EnvironmentNumberOfStates = ParentType::EnvironmentNumberOfStates;
    static constexpr size_t EnvironmentNumberOfActions = ParentType::EnvironmentNumberOfActions;
    static constexpr size_t EnvironmentInvalidState = ParentType::EnvironmentInvalidState;
    static constexpr size_t EnvironmentInvalidAction = ParentType::EnvironmentInvalidAction;
    static const ValueType EnvironmentRewardValue;
    static const ValueType EnvironmentNoRewardValue;
    static const ValueType EnvironmentInvalidActionValue;

    MazeEnvironment(const ValueType& learningRate, const ValueType& discountFactor, const size_t randomActionDecisionPoint) : 
        ParentType(learningRate, discountFactor, randomActionDecisionPoint), mGoalState(EnvironmentInvalidState)
    {
    }

    StateType getGoalState() const
    {
        return this->mGoalState;
    }

    size_t getNextStateForStateActionPair(const StateType state, const ActionType action) const
    {
        return action;
    }

    ValueType getRewardForStateAndAction(const StateType state, const ActionType action) const
    {
        return this->mRewardPolicy.getRewardForStateAndAction(state, action);
    }

    size_t getValidActionsForState(const StateType state, ActionType* pActions) const
    {
        size_t numberOfValidActions = 0;

        for(ActionType action = 0;action < NumberOfActions;++action)
        {
            if(EnvironmentInvalidActionValue != this->getRewardForStateAndAction(state, action))
            {
                *pActions = action;
                ++pActions;
                ++numberOfValidActions;
            }
        }

        return numberOfValidActions;
    }

    bool isActionValidForState(const StateType state, const ActionType action) const
    {
        bool valid;
        switch(state)
        {
            case 0:
                if (4 == action)
                {
                    valid = true;
                }
                else
                {
                    valid = false;
                }
                break;
            case 1:
                if ((3 == action) || (5 == action))
                {
                    valid = true;
                }
                else
                {
                    valid = false;
                }
                break;
            case 2:
                if (3 == action)
                {
                    valid = true;
                }
                else
                {
                    valid = false;
                }
                break;
            case 3:
                if ((1 == action) || (4 == action) || (2 == action))
                {
                    valid = true;
                }
                else
                {
                    valid = false;
                }
                break;
            case 4:
                if ((0 == action) || (5 == action) || (3 == action))
                {
                    valid = true;
                }
                else
                {
                    valid = false;
                }
                break;
            case 5:
                if ((1 == action) || (4 == action) || (5 == action))
                {
                    valid = true;
                }
                else
                {
                    valid = false;
                }
                break;
            default:
                valid = false;
                break;
        }

        return valid;
    }

    bool isGoalState(const StateType state) const
    {
        return (state == this->mGoalState);
    }

    void setGoalState(const StateType goalState)
    {
        if (goalState < EnvironmentNumberOfStates)
        {
            this->mGoalState = goalState;
        }
    }

    void setRewardForStateAndAction(const StateType state, const ActionType action, const ValueType& reward)
    {
        this->mRewardPolicy.setRewardForStateAndAction(state, action, reward);
    }

    void takeAction(const typename ParentType::experience_t& experience)
    {
        this->mExperience = experience;
    }
    
    typename ParentType::experience_t mExperience;
    StateType mGoalState;
private:
    RewardPolicyType mRewardPolicy;
};

template<typename StateType, typename ActionType, typename ValueType, size_t NumberOfStates, size_t NumberOfActions, typename RewardPolicyType, template<typename> class QLearningPolicy>
const ValueType MazeEnvironment<StateType, ActionType, ValueType, NumberOfStates, NumberOfActions, RewardPolicyType, QLearningPolicy>::EnvironmentRewardValue = ValueType(1,0);

template<typename StateType, typename ActionType, typename ValueType, size_t NumberOfStates, size_t NumberOfActions, typename RewardPolicyType, template<typename> class QLearningPolicy>
const ValueType MazeEnvironment<StateType, ActionType, ValueType, NumberOfStates, NumberOfActions, RewardPolicyType, QLearningPolicy>::EnvironmentNoRewardValue = ValueType(0,0);

template<typename StateType, typename ActionType, typename ValueType, size_t NumberOfStates, size_t NumberOfActions, typename RewardPolicyType,template<typename> class QLearningPolicy>
const ValueType MazeEnvironment<StateType, ActionType, ValueType, NumberOfStates, NumberOfActions, RewardPolicyType, QLearningPolicy>::EnvironmentInvalidActionValue = ValueType(-1, 0);

// unsigned Q-format type to use as reward
typedef tinymind::QValue<16,16,false> QValueType;

// Define the action->reward policy
typedef tinymind::QTableRewardPolicy<state_t, action_t, QValueType, NUMBER_OF_STATES, NUMBER_OF_ACTIONS> QTableRewardPolicyType;

// define the maze environment type
typedef MazeEnvironment<state_t, action_t, QValueType, NUMBER_OF_STATES, NUMBER_OF_ACTIONS, QTableRewardPolicyType> MazeEnvironmentType;

// define the Q-learner type
typedef tinymind::QLearner<MazeEnvironmentType> QLearnerType;
