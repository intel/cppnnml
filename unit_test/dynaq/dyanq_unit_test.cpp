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
DynaQ-Learning unit test. Learn the best path out of a simple maze.

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
#include "fixedPointTransferFunctions.hpp"
#include "neuralnet.hpp"
#include "qlearn.hpp"
#include "dynaq.hpp"

static std::random_device rd;

struct MazeEnvironmentRandomNumberGeneratorPolicy
{
    MazeEnvironmentRandomNumberGeneratorPolicy() : mRandomEngine(rd())
    {
    }

    size_t randInt(const size_t min, const size_t max)
    {
        std::uniform_int_distribution<size_t> distribution(min, max);
        const size_t value = distribution(this->mRandomEngine);

        return value;
    }
private:
    std::default_random_engine mRandomEngine;
};

template<typename ValueType>
struct RandomWeightGenerator
{
    static ValueType generateRandomWeight()
    {
        static std::default_random_engine generator(rd());
        static std::uniform_real_distribution<double> distribution(-1.0, 1.0);
        const double temp = distribution(generator);
        const typename ValueType::FullWidthValueType value = static_cast<typename ValueType::FullWidthValueType>(temp * static_cast<double>(1 << ValueType::NumberOfFractionalBits));
        const ValueType weight(value);

        return weight;
    }

private:
    static std::default_random_engine generator;
    static std::uniform_real_distribution<double> distribution;
};

template<typename ValueType, size_t NumberOfStates, size_t NumberOfActions>
struct MazeEnvironment : public tinymind::QLearningEnvironment<ValueType, NumberOfStates, NumberOfActions, MazeEnvironmentRandomNumberGeneratorPolicy>
{
    typedef tinymind::QLearningEnvironment<ValueType, NumberOfStates, NumberOfActions, MazeEnvironmentRandomNumberGeneratorPolicy> ParentType;
    static const size_t EnvironmentNumberOfStates = tinymind::QLearningEnvironment<ValueType, NumberOfStates, NumberOfActions, MazeEnvironmentRandomNumberGeneratorPolicy>::EnvironmentNumberOfStates;
    static const size_t EnvironmentNumberOfActions = tinymind::QLearningEnvironment<ValueType, NumberOfStates, NumberOfActions, MazeEnvironmentRandomNumberGeneratorPolicy>::EnvironmentNumberOfActions;
    static const size_t EnvironmentInvalidState = tinymind::QLearningEnvironment<ValueType, NumberOfStates, NumberOfActions, MazeEnvironmentRandomNumberGeneratorPolicy>::EnvironmentInvalidState;
    static const size_t EnvironmentInvalidAction = tinymind::QLearningEnvironment<ValueType, NumberOfStates, NumberOfActions, MazeEnvironmentRandomNumberGeneratorPolicy>::EnvironmentInvalidAction;
    static const ValueType EnvironmentNoRewardValue;
    static const ValueType EnvironmentInvalidActionValue;
    
    MazeEnvironment(const ValueType& learningRate, const ValueType& discountFactor, const uint32_t randomActionDecisionPoint) : 
        tinymind::QLearningEnvironment<ValueType, NumberOfStates, NumberOfActions, MazeEnvironmentRandomNumberGeneratorPolicy>(learningRate, discountFactor, randomActionDecisionPoint)
    {
    }

    size_t getNextStateForStateActionPair(const size_t state, const size_t action) const
    {
        return action;
    }

    size_t getValidActionsForState(const size_t state, size_t* pActions) const
    {
        size_t numberOfValidActions = 0;

        for(size_t action = 0;action < NumberOfActions;++action)
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

    void takeAction(const typename ParentType::experience_t& experience)
    {
        this->mExperience = experience;
    }
    
    typename ParentType::experience_t mExperience;
};

template<typename ValueType, size_t NumberOfStates, size_t NumberOfActions>
const ValueType MazeEnvironment<ValueType, NumberOfStates, NumberOfActions>::EnvironmentNoRewardValue = ValueType(0);

template<typename ValueType, size_t NumberOfStates, size_t NumberOfActions>
const ValueType MazeEnvironment<ValueType, NumberOfStates, NumberOfActions>::EnvironmentInvalidActionValue = ValueType(-1, 0);

#define NUMBER_OF_STATES 6
#define NUMBER_OF_ACTIONS 6

typedef tinymind::QValue<16,16,true> QValueType;
typedef MazeEnvironment<QValueType, NUMBER_OF_STATES, NUMBER_OF_ACTIONS> MazeEnvironmentType;
typedef tinymind::QLearner<MazeEnvironmentType> QLearnerType;
typedef tinymind::FixedPointTransferFunctions<QValueType, RandomWeightGenerator<QValueType> > TransferFunctionsType;
typedef tinymind::ElmanNetwork<TransferFunctionsType::TransferFunctionsValueType, 2, 3, 1, TransferFunctionsType> FixedPointElmanNetworkType;
typedef tinymind::DynaQLearner<QLearnerType, FixedPointElmanNetworkType> DynaQLearnerType;

static const QValueType learningRate = (QValueType(1,0) / QValueType(5,0));
static const QValueType discountFactor = (QValueType(1,0) / QValueType(2,0));
static QLearnerType qLearner(learningRate, discountFactor, 100);
static DynaQLearnerType dynaqLearner(learningRate, discountFactor, 100);
static const QValueType reward(100,0);

static void initializeRewardMatrix(void)
{
    size_t state;

    state = 0;
    for(size_t action = 0;action < 4;++action)
    {
        qLearner.getEnvironment().setRewardForStateAndAction(state, action, MazeEnvironmentType::EnvironmentInvalidActionValue);
    }
    qLearner.getEnvironment().setRewardForStateAndAction(state, 4, MazeEnvironmentType::EnvironmentNoRewardValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 5, MazeEnvironmentType::EnvironmentInvalidActionValue);

    state = 1;
    for(size_t action = 0;action < 3;++action)
    {
        qLearner.getEnvironment().setRewardForStateAndAction(state, action, MazeEnvironmentType::EnvironmentInvalidActionValue);
    }
    qLearner.getEnvironment().setRewardForStateAndAction(state, 3, MazeEnvironmentType::EnvironmentNoRewardValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 4, MazeEnvironmentType::EnvironmentInvalidActionValue);
    qLearner.getEnvironment().setRewardForStateAndAction(state, 5, reward);

    state = 2;
    for(size_t action = 0;action < NUMBER_OF_ACTIONS;++action)
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

BOOST_AUTO_TEST_SUITE(test_suite_dynaqlearn)

BOOST_AUTO_TEST_CASE(test_dynaqlearn_init)
{
    BOOST_TEST(QValueType(0) == qLearner.getAccumulatedReward());

    for(size_t state = 0;state < NUMBER_OF_STATES;++state)
    {
        for(size_t action = 0;action < NUMBER_OF_ACTIONS;++action)
        {
            BOOST_TEST(static_cast<QValueType>(0) == qLearner.getQTableValue(state, action));
            BOOST_TEST(static_cast<QValueType>(0) == qLearner.getEnvironment().getRewardForStateAndAction(state, action));
        }
    }
}

BOOST_AUTO_TEST_CASE(test_dynaqlearn_setreward)
{
    initializeRewardMatrix();

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

BOOST_AUTO_TEST_CASE(test_dynaqlearn_text_next_state)
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

BOOST_AUTO_TEST_CASE(test_dynaqlearn_change_learning_rate)
{
    static const QValueType newLearningRate = (QValueType(1,0) / QValueType(100,0));
    BOOST_TEST(learningRate != newLearningRate);
    BOOST_TEST(learningRate == qLearner.getEnvironment().getLearningRate());
    qLearner.getEnvironment().setLearningRate(newLearningRate);
    BOOST_TEST(newLearningRate == qLearner.getEnvironment().getLearningRate());
    qLearner.getEnvironment().setLearningRate(learningRate);
    BOOST_TEST(learningRate == qLearner.getEnvironment().getLearningRate());
}

BOOST_AUTO_TEST_CASE(test_dynaqlearn_change_discount_factor)
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

BOOST_AUTO_TEST_CASE(test_dynaqlearn_choose_random_action_test)
{
    const uint32_t oldDecisionPoint = qLearner.getEnvironment().getRandomActionDecisionPoint();
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

BOOST_AUTO_TEST_CASE(test_dynaqlearn_goal_state)
{
    BOOST_TEST(-1 == qLearner.getGoalState());

    qLearner.setGoalState(0);
    BOOST_TEST(0 == qLearner.getGoalState());

    qLearner.setGoalState(-1);
    BOOST_TEST(0 == qLearner.getGoalState());

    qLearner.setGoalState(QLearnerType::NumberOfStates);
    BOOST_TEST(0 == qLearner.getGoalState());

    qLearner.setGoalState(NUMBER_OF_STATES - 1);
    BOOST_TEST((NUMBER_OF_STATES - 1) == qLearner.getGoalState());
}

BOOST_AUTO_TEST_CASE(test_qlearn_argmax_best_action)
{
    typedef tinymind::QTablePolicy<MazeEnvironmentType> QValuePolicyType;
    QValuePolicyType qValuePolicy;
    QValueType qValue(0, 0);
    size_t state = 1;
    size_t actions[NUMBER_OF_ACTIONS] = { 0, 1, 2, 3, 4, 5 };
    size_t bestAction;

    for (size_t action = 0; action < NUMBER_OF_ACTIONS; ++action)
    {
        qValuePolicy.setQValue(state, action, qValue);
        ++qValue;
    }

    bestAction = tinymind::ArgMaxPolicy<MazeEnvironmentType, QValuePolicyType>::selectActionForState(state, &actions[0], NUMBER_OF_ACTIONS, qValuePolicy);
    BOOST_TEST(5 == bestAction);

    qValue = 0;
    for (size_t action = NUMBER_OF_ACTIONS; action > 0; --action)
    {
        qValuePolicy.setQValue(state, action - 1, qValue);
        ++qValue;
    }

    bestAction = tinymind::ArgMaxPolicy<MazeEnvironmentType, QValuePolicyType>::selectActionForState(state, &actions[0], NUMBER_OF_ACTIONS, qValuePolicy);
    BOOST_TEST(0 == bestAction);

    qValuePolicy.setQValue(state, 1, QValueType(100, 0));
    bestAction = tinymind::ArgMaxPolicy<MazeEnvironmentType, QValuePolicyType>::selectActionForState(state, &actions[0], NUMBER_OF_ACTIONS - 2, qValuePolicy);
    BOOST_TEST(1 == bestAction);
}

BOOST_AUTO_TEST_CASE(test_dynaqlearn_take_action_test)
{
    typename MazeEnvironmentType::ParentType::experience_t experience;
    size_t state = 1;
    size_t action = 5;

    experience.state = state;
    experience.action = action;
    qLearner.getEnvironment().takeAction(experience);
    experience.reward = qLearner.getEnvironment().getRewardForStateAndAction(experience.state, experience.action);
    experience.newState =  experience.action;
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
    experience.newState =  experience.action;
    qLearner.updateFromExperience(experience);

    BOOST_TEST(0 == experience.state);
    BOOST_TEST(4 == experience.action);
    BOOST_TEST(MazeEnvironmentType::EnvironmentNoRewardValue == experience.reward);
    BOOST_TEST(4 == experience.newState);
}

BOOST_AUTO_TEST_CASE(test_dynaqlearn_iterate_test)
{
    char const* const rewardFilePath = "qlearn_accum_reward.txt";
    char const* const qTableFilePath = "qtable.txt";
    typename MazeEnvironmentType::ParentType::experience_t experience;
    std::default_random_engine engine(rd());
    std::uniform_int_distribution<size_t> stateDistribution(0, NUMBER_OF_STATES - 1);
    size_t state;
    size_t decisionPoint = 100;
    std::ofstream rewardFile(rewardFilePath);
    std::ofstream qTableFile(qTableFilePath);

    for (auto i = 0; i < 500; ++i)
    {
        qLearner.getEnvironment().setRandomActionDecisionPoint(decisionPoint);

        qLearner.startNewEpisode();
        state = stateDistribution(engine);
        qLearner.setState(state);
        qLearner.setGoalState(5);
        qLearner.iterate();

        experience = qLearner.getEnvironment().mExperience;
        experience.reward = qLearner.getEnvironment().getRewardForStateAndAction(experience.state, experience.action);
        experience.newState =  experience.action;
        qLearner.updateFromExperience(experience);

        while (qLearner.getState() != 5)
        {
            qLearner.iterate();
            experience = qLearner.getEnvironment().mExperience;
            experience.reward = qLearner.getEnvironment().getRewardForStateAndAction(experience.state, experience.action);
            experience.newState =  experience.action;
            qLearner.updateFromExperience(experience);
        }

        rewardFile << qLearner.getAccumulatedReward().getValue() << std::endl;

        if (decisionPoint > 0)
        {
            decisionPoint -= 1;
        }
    }

    // print out q table
    for(auto state = 0;state < NUMBER_OF_STATES;++state)
    {
        qTableFile << state << ",";
        for(auto action = 0;action < NUMBER_OF_ACTIONS;++action)
        {
            qTableFile << qLearner.getQTableValue(state, action).getValue();
            if (action != (NUMBER_OF_ACTIONS - 1))
            {
                qTableFile << ",";
            }
        }

        qTableFile << std::endl;
    }

    qTableFile.close();
}
BOOST_AUTO_TEST_SUITE_END()
