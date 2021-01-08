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

#define BOOST_TEST_MODULE test module name
#include <boost/test/included/unit_test.hpp>

#include <cstdint>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <random>
#include <vector>
#include <iostream>

#include "qformat.hpp"
#include "qlearn.hpp"
#include "fixedPointTransferFunctions.hpp"
#include "activationFunctions.hpp"
#include "nnproperties.hpp"

static std::random_device rd;

#define NUMBER_OF_STATES 6
#define NUMBER_OF_ACTIONS 6

typedef uint8_t state_t;
typedef uint8_t action_t;

template<typename ValueType>
struct UniformRealRandomNumberGenerator
{
    typedef tinymind::ValueConverter<double, ValueType> WeightConverterPolicy;

    static ValueType generateRandomWeight()
    {
        const double temp = distribution(generator);
        const ValueType weight = WeightConverterPolicy::convertToDestinationType(temp);

        return weight;
    }

private:
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> distribution;
};

template<typename ValueType>
std::default_random_engine UniformRealRandomNumberGenerator<ValueType>::generator;

template<typename ValueType>
std::uniform_real_distribution<double> UniformRealRandomNumberGenerator<ValueType>::distribution(-1.0, 1.0);

template<typename T>
struct MazeEnvironmentRandomNumberGeneratorPolicy
{
    MazeEnvironmentRandomNumberGeneratorPolicy() : mRandomEngine(rd())
    {
    }

    size_t getRandomActionDecisionPoint() const
    {
        return this->mRandomActionDecisionPoint;
    }

    T randInt(const T min, const T max)
    {
        std::uniform_int_distribution<size_t> distribution(min, max);
        const T value = static_cast<T>(distribution(this->mRandomEngine));

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
    std::default_random_engine mRandomEngine;
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
    typedef tinymind::QLearningEnvironment<state_t, action_t, ValueType, NumberOfStates, NumberOfActions, MazeEnvironmentRandomNumberGeneratorPolicy> ParentType;
    static const size_t EnvironmentNumberOfStates = ParentType::EnvironmentNumberOfStates;
    static const size_t EnvironmentNumberOfActions = ParentType::EnvironmentNumberOfActions;
    static const size_t EnvironmentInvalidState = ParentType::EnvironmentInvalidState;
    static const size_t EnvironmentInvalidAction = ParentType::EnvironmentInvalidAction;
    static const ValueType EnvironmentNoRewardValue;
    static const ValueType EnvironmentInvalidActionValue;
    static const ValueType ONE;
    
    MazeEnvironment(const ValueType& learningRate, const ValueType& discountFactor, const size_t randomActionDecisionPoint) : 
        ParentType(learningRate, discountFactor, randomActionDecisionPoint), mGoalState(EnvironmentInvalidState)
    {
    }

    StateType getGoalState() const
    {
        return this->mGoalState;
    }

    size_t getNextStateForStateActionPair(const state_t state, const action_t action) const
    {
        return action;
    }

    ValueType getRewardForStateAndAction(const StateType state, const ActionType action) const
    {
        return this->mRewardPolicy.getRewardForStateAndAction(state, action);
    }

    size_t getValidActionsForState(const state_t state, action_t* pActions) const
    {
        size_t numberOfValidActions = 0;

        for(action_t action = 0;action < NumberOfActions;++action)
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
const ValueType MazeEnvironment<StateType, ActionType, ValueType, NumberOfStates, NumberOfActions, RewardPolicyType, QLearningPolicy>::EnvironmentNoRewardValue = ValueType(0);

template<typename StateType, typename ActionType, typename ValueType, size_t NumberOfStates, size_t NumberOfActions, typename RewardPolicyType,template<typename> class QLearningPolicy>
const ValueType MazeEnvironment<StateType, ActionType, ValueType, NumberOfStates, NumberOfActions, RewardPolicyType, QLearningPolicy>::EnvironmentInvalidActionValue = ValueType(-1, 0);

template<typename StateType, typename ActionType, typename ValueType, size_t NumberOfStates, size_t NumberOfActions, typename RewardPolicyType,template<typename> class QLearningPolicy>
const ValueType MazeEnvironment<StateType, ActionType, ValueType, NumberOfStates, NumberOfActions, RewardPolicyType, QLearningPolicy>::ONE = ValueType(1,0);

template<   typename StateType,
            typename ActionType,
            typename ValueType,
            size_t NumberOfStates,
            size_t NumberOfActions,
            typename RewardPolicyType,
            template<typename> class QLearningPolicy = tinymind::DefaultLearningPolicy
        >
struct DQNMazeEnvironment : public tinymind::QLearningEnvironment<state_t, action_t, ValueType, NumberOfStates, NumberOfActions, MazeEnvironmentRandomNumberGeneratorPolicy>
{
    typedef tinymind::QLearningEnvironment<state_t, action_t, ValueType, NumberOfStates, NumberOfActions, MazeEnvironmentRandomNumberGeneratorPolicy> ParentType;
    static const size_t EnvironmentNumberOfStates = ParentType::EnvironmentNumberOfStates;
    static const size_t EnvironmentNumberOfActions = ParentType::EnvironmentNumberOfActions;
    static const size_t EnvironmentInvalidState = ParentType::EnvironmentInvalidState;
    static const size_t EnvironmentInvalidAction = ParentType::EnvironmentInvalidAction;
    static const ValueType EnvironmentNoRewardValue;
    static const ValueType EnvironmentInvalidActionValue;
    static const ValueType ONE;
    
    DQNMazeEnvironment(const ValueType& learningRate, const ValueType& discountFactor, const size_t randomActionDecisionPoint) : 
        ParentType(learningRate, discountFactor, randomActionDecisionPoint), mGoalState(EnvironmentInvalidState)
    {
    }

    StateType getGoalState() const
    {
        return this->mGoalState;
    }

    static void getInputValues(const StateType state, ValueType *pInputs)
    {
        static const ValueType MAX_STATE = ValueType(NUMBER_OF_STATES,0);
        ValueType input = (ValueType(state, 0) / MAX_STATE);

        *pInputs = input;
    }

    size_t getNextStateForStateActionPair(const state_t state, const action_t action) const
    {
        return action;
    }

    ValueType getRewardForStateAndAction(const StateType state, const ActionType action) const
    {
        return this->mRewardPolicy.getRewardForStateAndAction(state, action);
    }

    size_t getValidActionsForState(const state_t state, action_t* pActions) const
    {
        size_t numberOfValidActions = 0;

        for(action_t action = 0;action < NumberOfActions;++action)
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
const ValueType DQNMazeEnvironment<StateType, ActionType, ValueType, NumberOfStates, NumberOfActions, RewardPolicyType, QLearningPolicy>::EnvironmentNoRewardValue = ValueType(0);

template<typename StateType, typename ActionType, typename ValueType, size_t NumberOfStates, size_t NumberOfActions, typename RewardPolicyType,template<typename> class QLearningPolicy>
const ValueType DQNMazeEnvironment<StateType, ActionType, ValueType, NumberOfStates, NumberOfActions, RewardPolicyType, QLearningPolicy>::EnvironmentInvalidActionValue = ValueType(-1, 0);

template<typename StateType, typename ActionType, typename ValueType, size_t NumberOfStates, size_t NumberOfActions, typename RewardPolicyType,template<typename> class QLearningPolicy>
const ValueType DQNMazeEnvironment<StateType, ActionType, ValueType, NumberOfStates, NumberOfActions, RewardPolicyType, QLearningPolicy>::ONE = ValueType(1,0);

typedef tinymind::QValue<16,16,true> QValueType;
typedef tinymind::QTableRewardPolicy<state_t, action_t, QValueType,NUMBER_OF_STATES, NUMBER_OF_ACTIONS> QTableRewardPolicyType;
typedef MazeEnvironment<state_t, action_t, QValueType, NUMBER_OF_STATES, NUMBER_OF_ACTIONS, QTableRewardPolicyType> MazeEnvironmentType;
typedef tinymind::QLearner<MazeEnvironmentType> QLearnerType;

typedef tinymind::NullRewardPolicy<state_t, action_t, QValueType, NUMBER_OF_STATES, NUMBER_OF_ACTIONS> NullRewardPolicyType;
typedef MazeEnvironment<state_t, action_t, QValueType, NUMBER_OF_STATES, NUMBER_OF_ACTIONS, NullRewardPolicyType, tinymind::NullLearningPolicy> UntrainedMazeEnvironmentType;
typedef tinymind::QLearner<UntrainedMazeEnvironmentType> UntrainedQLearnerType;

#define NUMBER_OF_INPUT_LAYER_NEURONS 1
#define NUMBER_OF_HIDDEN_LAYERS 1
#define NUMBER_OF_HIDDEN_LAYER_NEURONS (NUMBER_OF_ACTIONS + 2)
#define NUMBER_OF_OUTPUT_LAYER_NEURONS NUMBER_OF_ACTIONS
#define NUMBER_OF_ITERATIONS_FOR_TARGET_NN_UPDATE 10

typedef DQNMazeEnvironment<state_t, action_t, QValueType, NUMBER_OF_STATES, NUMBER_OF_ACTIONS, QTableRewardPolicyType> DQNMazeEnvironmentType;
typedef tinymind::FixedPointTransferFunctions<  QValueType,
                                                UniformRealRandomNumberGenerator<QValueType>,
                                                tinymind::TanhActivationPolicy<QValueType>,
                                                tinymind::TanhActivationPolicy<QValueType>,
                                                NUMBER_OF_OUTPUT_LAYER_NEURONS> TransferFunctionsType;
typedef tinymind::MultilayerPerceptron< QValueType,
                                        NUMBER_OF_INPUT_LAYER_NEURONS,
                                        NUMBER_OF_HIDDEN_LAYERS,
                                        NUMBER_OF_HIDDEN_LAYER_NEURONS,
                                        NUMBER_OF_OUTPUT_LAYER_NEURONS,
                                        TransferFunctionsType> NeuralNetworkType;
typedef tinymind::QValueNeuralNetworkPolicy<DQNMazeEnvironmentType,
                                            NeuralNetworkType,
                                            NUMBER_OF_ITERATIONS_FOR_TARGET_NN_UPDATE> QValuePolicyType;
typedef tinymind::QLearner<DQNMazeEnvironmentType, QValuePolicyType> DQNQLearnerType;

static const QValueType learningRate = (QValueType(1,0) / QValueType(5,0));
static const QValueType discountFactor = (QValueType(8,0) / QValueType(10,0));
static QLearnerType qLearner(learningRate, discountFactor, 100);
static DQNQLearnerType dqnQLearner(learningRate, discountFactor, 100);
static const QValueType reward(100,0);
static UntrainedQLearnerType untrainedQLearner(QValueType(0,0), QValueType(0,0), 0);

template<typename QLearnerType>
static void initializeRewardMatrix(QLearnerType& qLearner)
{
    state_t state;

    state = 0;
    for(action_t action = 0;action < 4;++action)
    {
        qLearner.getEnvironment().setRewardForStateAndAction(state, action, MazeEnvironmentType::EnvironmentInvalidActionValue);
    }
    qLearner.getEnvironment().setRewardForStateAndAction(state, 4, MazeEnvironmentType::EnvironmentNoRewardValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 5, MazeEnvironmentType::EnvironmentInvalidActionValue);

    state = 1;
    for(action_t action = 0;action < 3;++action)
    {
        qLearner.getEnvironment().setRewardForStateAndAction(state, action, MazeEnvironmentType::EnvironmentInvalidActionValue);
    }
    qLearner.getEnvironment().setRewardForStateAndAction(state, 3, MazeEnvironmentType::EnvironmentNoRewardValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 4, MazeEnvironmentType::EnvironmentInvalidActionValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 5, reward);

    state = 2;
    for(action_t action = 0;action < NUMBER_OF_ACTIONS;++action)
    {
        qLearner.getEnvironment().setRewardForStateAndAction(state, action, MazeEnvironmentType::EnvironmentInvalidActionValue);
    }
    qLearner.getEnvironment().setRewardForStateAndAction(state, 3, MazeEnvironmentType::EnvironmentNoRewardValue);

    state = 3;
    qLearner.getEnvironment().setRewardForStateAndAction(state, 0, MazeEnvironmentType::EnvironmentInvalidActionValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 1, MazeEnvironmentType::EnvironmentNoRewardValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 2, MazeEnvironmentType::EnvironmentNoRewardValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 3, MazeEnvironmentType::EnvironmentInvalidActionValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 4, MazeEnvironmentType::EnvironmentNoRewardValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 5, MazeEnvironmentType::EnvironmentInvalidActionValue);

    state = 4;
    qLearner.getEnvironment().setRewardForStateAndAction(state, 0, MazeEnvironmentType::EnvironmentNoRewardValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 1, MazeEnvironmentType::EnvironmentInvalidActionValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 2, MazeEnvironmentType::EnvironmentInvalidActionValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 3, MazeEnvironmentType::EnvironmentNoRewardValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 4, MazeEnvironmentType::EnvironmentInvalidActionValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 5, reward);

    state = 5;
    qLearner.getEnvironment().setRewardForStateAndAction(state, 0, MazeEnvironmentType::EnvironmentInvalidActionValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 1, MazeEnvironmentType::EnvironmentNoRewardValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 2, MazeEnvironmentType::EnvironmentInvalidActionValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 3, MazeEnvironmentType::EnvironmentInvalidActionValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 4, MazeEnvironmentType::EnvironmentNoRewardValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 5, reward);
}

BOOST_AUTO_TEST_SUITE(test_suite_qlearn)

BOOST_AUTO_TEST_CASE(test_qlearn_init)
{
    for(state_t state = 0;state < NUMBER_OF_STATES;++state)
    {
        for(action_t action = 0;action < NUMBER_OF_ACTIONS;++action)
        {
            BOOST_TEST(static_cast<QValueType>(0) == qLearner.getQValue(state, action));
            BOOST_TEST(static_cast<QValueType>(0) == qLearner.getEnvironment().getRewardForStateAndAction(state, action));
        }
    }
}

BOOST_AUTO_TEST_CASE(test_qlearn_setreward)
{
    initializeRewardMatrix(qLearner);

    BOOST_TEST(MazeEnvironmentType::EnvironmentNoRewardValue == qLearner.getEnvironment().getRewardForStateAndAction(0, 4));
    BOOST_TEST(MazeEnvironmentType::EnvironmentNoRewardValue == qLearner.getEnvironment().getRewardForStateAndAction(1, 3));
    BOOST_TEST(MazeEnvironmentType::EnvironmentNoRewardValue == qLearner.getEnvironment().getRewardForStateAndAction(2, 3));
    BOOST_TEST(MazeEnvironmentType::EnvironmentNoRewardValue == qLearner.getEnvironment().getRewardForStateAndAction(3, 1));
    BOOST_TEST(MazeEnvironmentType::EnvironmentNoRewardValue == qLearner.getEnvironment().getRewardForStateAndAction(3, 2));
    BOOST_TEST(MazeEnvironmentType::EnvironmentNoRewardValue == qLearner.getEnvironment().getRewardForStateAndAction(3, 4));
    BOOST_TEST(MazeEnvironmentType::EnvironmentNoRewardValue == qLearner.getEnvironment().getRewardForStateAndAction(4, 0));
    BOOST_TEST(MazeEnvironmentType::EnvironmentNoRewardValue == qLearner.getEnvironment().getRewardForStateAndAction(4, 3));
    BOOST_TEST(MazeEnvironmentType::EnvironmentNoRewardValue == qLearner.getEnvironment().getRewardForStateAndAction(5, 1));
    BOOST_TEST(MazeEnvironmentType::EnvironmentNoRewardValue == qLearner.getEnvironment().getRewardForStateAndAction(5, 4));

    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(0, 0));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(0, 1));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(0, 2));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(0, 5));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(1, 0));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(1, 1));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(1, 2));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(1, 4));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(2, 0));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(2, 1));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(2, 2));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(2, 4));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(2, 5));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(3, 0));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(3, 3));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(3, 5));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(4, 1));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(4, 2));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(4, 4));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(5, 0));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(5, 2));
    BOOST_TEST(MazeEnvironmentType::EnvironmentInvalidActionValue == qLearner.getEnvironment().getRewardForStateAndAction(5, 3));

    BOOST_TEST(reward == qLearner.getEnvironment().getRewardForStateAndAction(1, 5));
    BOOST_TEST(reward == qLearner.getEnvironment().getRewardForStateAndAction(4, 5));
    BOOST_TEST(reward == qLearner.getEnvironment().getRewardForStateAndAction(5, 5));
}

BOOST_AUTO_TEST_CASE(test_qlearn_text_next_state)
{
    BOOST_TEST(4 == qLearner.getEnvironment().getNextStateForStateActionPair(0, 4));
    BOOST_TEST(3 == qLearner.getEnvironment().getNextStateForStateActionPair(1, 3));
    BOOST_TEST(5 == qLearner.getEnvironment().getNextStateForStateActionPair(1, 5));
    BOOST_TEST(3 == qLearner.getEnvironment().getNextStateForStateActionPair(2, 3));
    BOOST_TEST(1 == qLearner.getEnvironment().getNextStateForStateActionPair(3, 1));
    BOOST_TEST(2 == qLearner.getEnvironment().getNextStateForStateActionPair(3, 2));
    BOOST_TEST(4 == qLearner.getEnvironment().getNextStateForStateActionPair(3, 4));
    BOOST_TEST(0 == qLearner.getEnvironment().getNextStateForStateActionPair(4, 0));
    BOOST_TEST(3 == qLearner.getEnvironment().getNextStateForStateActionPair(4, 3));
    BOOST_TEST(5 == qLearner.getEnvironment().getNextStateForStateActionPair(4, 5));
    BOOST_TEST(1 == qLearner.getEnvironment().getNextStateForStateActionPair(5, 1));
    BOOST_TEST(4 == qLearner.getEnvironment().getNextStateForStateActionPair(5, 4));
    BOOST_TEST(5 == qLearner.getEnvironment().getNextStateForStateActionPair(5, 5));
}

BOOST_AUTO_TEST_CASE(test_qlearn_change_learning_rate)
{
    static const QValueType newLearningRate = (QValueType(1,0) / QValueType(100,0));
    BOOST_TEST(learningRate != newLearningRate);
    BOOST_TEST(learningRate == qLearner.getEnvironment().getLearningRate());
    qLearner.getEnvironment().setLearningRate(newLearningRate);
    BOOST_TEST(newLearningRate == qLearner.getEnvironment().getLearningRate());
    qLearner.getEnvironment().setLearningRate(learningRate);
    BOOST_TEST(learningRate == qLearner.getEnvironment().getLearningRate());
}

BOOST_AUTO_TEST_CASE(test_qlearn_change_discount_factor)
{
    static const QValueType delta = (QValueType(1,0) / QValueType(100,0));
    static const QValueType discountFactor = qLearner.getEnvironment().getDiscountFactor();
    static const QValueType newDiscountFactor = (discountFactor + delta);

    BOOST_TEST(discountFactor != newDiscountFactor);
    qLearner.getEnvironment().setDiscountFactor(newDiscountFactor);
    BOOST_TEST(newDiscountFactor == qLearner.getEnvironment().getDiscountFactor());
    qLearner.getEnvironment().setDiscountFactor(discountFactor);
    BOOST_TEST(discountFactor == qLearner.getEnvironment().getDiscountFactor());
}

BOOST_AUTO_TEST_CASE(test_qlearn_choose_random_action)
{
    const size_t oldDecisionPoint = qLearner.getEnvironment().getRandomActionDecisionPoint();
    std::vector<bool> choices;
    std::size_t t;

    qLearner.getEnvironment().setRandomActionDecisionPoint(50);
    for(auto i = 0;i < 1000;++i)
    {
        choices.push_back(qLearner.getEnvironment().shouldChooseRandomAction());
    }

    t = std::count_if(choices.begin(), choices.end(), [](const bool value){return value;});

    BOOST_TEST(((t >= 400) && (t <= 600)));

    qLearner.getEnvironment().setRandomActionDecisionPoint(90);
    choices.clear();
    for(auto i = 0;i < 1000;++i)
    {
        choices.push_back(qLearner.getEnvironment().shouldChooseRandomAction());
    }

    t = std::count_if(choices.begin(), choices.end(), [](const bool value){return value;});

    BOOST_TEST(((t >= 800) && (t <= 1000)));

    qLearner.getEnvironment().setRandomActionDecisionPoint(0);
    choices.clear();
    for(auto i = 0;i < 1000;++i)
    {
        choices.push_back(qLearner.getEnvironment().shouldChooseRandomAction());
    }

    t = std::count_if(choices.begin(), choices.end(), [](const bool value){return value;});

    BOOST_TEST(0 == t);

    qLearner.getEnvironment().setRandomActionDecisionPoint(100);
    choices.clear();
    for(auto i = 0;i < 1000;++i)
    {
        choices.push_back(qLearner.getEnvironment().shouldChooseRandomAction());
    }

    t = std::count_if(choices.begin(), choices.end(), [](const bool value){return value;});

    BOOST_TEST(1000 == t);

    qLearner.getEnvironment().setRandomActionDecisionPoint(oldDecisionPoint);
}

BOOST_AUTO_TEST_CASE(test_qlearn_goal_state)
{
    BOOST_TEST(static_cast<state_t>(-1) == qLearner.getEnvironment().getGoalState());

    qLearner.getEnvironment().setGoalState(0);
    BOOST_TEST(0 == qLearner.getEnvironment().getGoalState());

    qLearner.getEnvironment().setGoalState(-1);
    BOOST_TEST(0 == qLearner.getEnvironment().getGoalState());

    qLearner.getEnvironment().setGoalState(QLearnerType::NumberOfStates);
    BOOST_TEST(0 == qLearner.getEnvironment().getGoalState());

    qLearner.getEnvironment().setGoalState(NUMBER_OF_STATES - 1);
    BOOST_TEST((NUMBER_OF_STATES - 1) == qLearner.getEnvironment().getGoalState());
}

BOOST_AUTO_TEST_CASE(test_qlearn_argmax_best_action)
{
    typedef tinymind::QValueTablePolicy<MazeEnvironmentType> QValuePolicyType;
    QValuePolicyType qValuePolicy;
    QValueType qValue(0, 0);
    state_t state = 1;
    action_t actions[NUMBER_OF_ACTIONS] = { 0, 1, 2, 3, 4, 5 };
    action_t bestAction;

    for (action_t action = 0; action < NUMBER_OF_ACTIONS; ++action)
    {
        qValuePolicy.setQValue(state, action, qValue);
        ++qValue;
    }

    bestAction = tinymind::ArgMaxPolicy<MazeEnvironmentType, QValuePolicyType>::selectBestActionForState(state, &actions[0], NUMBER_OF_ACTIONS, qValuePolicy);
    BOOST_TEST(5 == bestAction);

    qValue = 0;
    for (action_t action = NUMBER_OF_ACTIONS; action > 0; --action)
    {
        qValuePolicy.setQValue(state, action - 1, qValue);
        ++qValue;
    }

    bestAction = tinymind::ArgMaxPolicy<MazeEnvironmentType, QValuePolicyType>::selectBestActionForState(state, &actions[0], NUMBER_OF_ACTIONS, qValuePolicy);
    BOOST_TEST(0 == bestAction);

    qValuePolicy.setQValue(state, 1, QValueType(100, 0));
    bestAction = tinymind::ArgMaxPolicy<MazeEnvironmentType, QValuePolicyType>::selectBestActionForState(state, &actions[0], NUMBER_OF_ACTIONS - 2, qValuePolicy);
    BOOST_TEST(1 == bestAction);
}

BOOST_AUTO_TEST_CASE(test_qlearn_take_action)
{
    typename MazeEnvironmentType::ParentType::experience_t experience;
    state_t state = 1;
    action_t action = 5;

    initializeRewardMatrix(qLearner);

    experience.state = state;
    experience.action = action;
    qLearner.getEnvironment().takeAction(experience);
    experience.reward = qLearner.getEnvironment().getRewardForStateAndAction(experience.state, experience.action);
    experience.newState =  static_cast<state_t>(experience.action);
    qLearner.updateFromExperience(experience);

    BOOST_TEST(1 == experience.state);
    BOOST_TEST(5 == experience.action);
    BOOST_TEST(reward == experience.reward);
    BOOST_TEST(5 == experience.newState);

    state = 0;
    action = 4;

    experience.state = state;
    experience.action = action;
    qLearner.getEnvironment().takeAction(experience);
    experience.reward = qLearner.getEnvironment().getRewardForStateAndAction(experience.state, experience.action);
    experience.newState =  static_cast<state_t>(experience.action);
    qLearner.updateFromExperience(experience);

    BOOST_TEST(0 == experience.state);
    BOOST_TEST(4 == experience.action);
    BOOST_TEST(MazeEnvironmentType::EnvironmentNoRewardValue == experience.reward);
    BOOST_TEST(4 == experience.newState);
}

BOOST_AUTO_TEST_CASE(test_qlearn_iterate)
{
    typedef typename QValueType::FullWidthValueType FullWidthValueType;
    static char const* const qTableFilePath = "qtable.txt";
    static char const* const qTableBinFilePath = "qtable.bin";
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<size_t> stateDistribution(0, NUMBER_OF_STATES - 1);
    size_t decisionPoint = 100;
    std::ofstream qTableFile(qTableFilePath);
    std::ofstream qTableBinFile(qTableBinFilePath, std::ios::out | std::ios::binary);
    typename MazeEnvironmentType::ParentType::experience_t experience;
    state_t state;
    action_t action;
    FullWidthValueType value;

    for (auto i = 0; i < 500; ++i)
    {
        qLearner.getEnvironment().setRandomActionDecisionPoint(decisionPoint);

        qLearner.startNewEpisode();
        state = static_cast<state_t>(stateDistribution(engine));
        qLearner.setState(state);
        qLearner.getEnvironment().setGoalState(5);
        action = qLearner.takeAction(state);
        experience.state = state;
        experience.action = action;
        experience.reward = qLearner.getEnvironment().getRewardForStateAndAction(experience.state, experience.action);
        experience.newState =  static_cast<state_t>(experience.action);
        qLearner.updateFromExperience(experience);

        while (qLearner.getState() != 5)
        {
            action = qLearner.takeAction(qLearner.getState());
            experience.state = qLearner.getState();
            experience.action = action;
            experience.reward = qLearner.getEnvironment().getRewardForStateAndAction(experience.state, experience.action);
            experience.newState =  static_cast<state_t>(experience.action);
            qLearner.updateFromExperience(experience);
        }

        if (decisionPoint > 0)
        {
            decisionPoint -= 1;
        }
    }

    // store q table in text form
    for(state_t state = 0;state < NUMBER_OF_STATES;++state)
    {
        qTableFile << state << ",";
        for(action_t action = 0;action < NUMBER_OF_ACTIONS;++action)
        {
            qTableFile << qLearner.getQValue(state, action).getValue();
            if (action != (NUMBER_OF_ACTIONS - 1))
            {
                qTableFile << ",";
            }
        }

        qTableFile << std::endl;
    }

    qTableFile.close();

    // store q table in binary form
    for(state_t state = 0;state < NUMBER_OF_STATES;++state)
    {
        for(action_t action = 0;action < NUMBER_OF_ACTIONS;++action)
        {
            value = qLearner.getQValue(state, action).getValue();
            qTableBinFile.write(reinterpret_cast<const char*>(&value), sizeof(FullWidthValueType));
        }
    }

    qTableBinFile.close();
}

BOOST_AUTO_TEST_CASE(test_dqn_qlearn_iterate)
{
    typedef typename QValueType::FullWidthValueType FullWidthValueType;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<size_t> stateDistribution(0, NUMBER_OF_STATES - 1);
    size_t decisionPoint = 100;
    typename DQNMazeEnvironmentType::ParentType::experience_t experience;
    state_t state;
    action_t action;

    initializeRewardMatrix(dqnQLearner);

    dqnQLearner.getEnvironment().setGoalState(5);

    for (auto i = 0; i < 500; ++i)
    {
        dqnQLearner.getEnvironment().setRandomActionDecisionPoint(decisionPoint);

        dqnQLearner.startNewEpisode();
        state = static_cast<state_t>(stateDistribution(engine));
        dqnQLearner.setState(state);
        action = dqnQLearner.takeAction(state);
        experience.state = state;
        experience.action = action;
        experience.reward = dqnQLearner.getEnvironment().getRewardForStateAndAction(experience.state, experience.action);
        experience.newState =  static_cast<state_t>(experience.action);
        dqnQLearner.updateFromExperience(experience);

        while (dqnQLearner.getState() != dqnQLearner.getEnvironment().getGoalState())
        {
            action = dqnQLearner.takeAction(dqnQLearner.getState());
            experience.state = dqnQLearner.getState();
            experience.action = action;
            experience.reward = dqnQLearner.getEnvironment().getRewardForStateAndAction(experience.state, experience.action);
            experience.newState =  static_cast<state_t>(experience.action);
            dqnQLearner.updateFromExperience(experience);
        }

        if (decisionPoint > 0)
        {
            decisionPoint -= 1;
        }
    }
}

BOOST_AUTO_TEST_CASE(test_untrained_qlearner_reward)
{
    // Reward table should not exist
    for(state_t state = 0;state < NUMBER_OF_STATES;++state)
    {
        for(action_t action = 0;action < NUMBER_OF_ACTIONS;++action)
        {
            BOOST_TEST(static_cast<QValueType>(0) == untrainedQLearner.getEnvironment().getRewardForStateAndAction(state, action));
        }
    }
    // Attempt to set reward
    for(state_t state = 0;state < NUMBER_OF_STATES;++state)
    {
        for(action_t action = 0;action < NUMBER_OF_ACTIONS;++action)
        {
            untrainedQLearner.getEnvironment().setRewardForStateAndAction(state, action, reward);
        }
    }

    // Setting reward should have no effect
    for(state_t state = 0;state < NUMBER_OF_STATES;++state)
    {
        for(action_t action = 0;action < NUMBER_OF_ACTIONS;++action)
        {
            BOOST_TEST(static_cast<QValueType>(0) == untrainedQLearner.getEnvironment().getRewardForStateAndAction(state, action));
        }
    }
}

//Load the Q table from the trained Q learner and use it.
BOOST_AUTO_TEST_CASE(test_untrained_qlearner_iterate)
{
    static const state_t goalState = 5;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<size_t> stateDistribution(0, NUMBER_OF_STATES - 1);
    size_t iterations = 0;
    typename UntrainedMazeEnvironmentType::ParentType::experience_t experience;
    state_t state;
    action_t action;

    for(state = 0;state < NUMBER_OF_STATES;++state)
    {
        for(action_t action = 0;action < NUMBER_OF_ACTIONS;++action)
        {
            untrainedQLearner.setQValue(state, action, qLearner.getQValue(state, action));
        }
    }

    do
    {
        untrainedQLearner.startNewEpisode();
        state = static_cast<state_t>(stateDistribution(engine));
        untrainedQLearner.setState(state);
        untrainedQLearner.getEnvironment().setGoalState(goalState);
        do
        {
            action = untrainedQLearner.takeAction(untrainedQLearner.getState());
            experience.state = state;
            experience.action = action;
            experience.newState =  static_cast<state_t>(experience.action);
            untrainedQLearner.setState(experience.newState);
        } while (untrainedQLearner.getState() != goalState);

        ++iterations;
        BOOST_TEST(goalState == untrainedQLearner.getState());
    } while(iterations < 1000);
}

BOOST_AUTO_TEST_SUITE_END()

