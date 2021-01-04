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

#include "constants.hpp"
#include "limits.hpp"
#include "signed.hpp"

namespace tinymind {
    template<typename StateType, typename ActionType, typename ValueType, size_t NumberOfStates, size_t NumberOfActions>
    class NullRewardPolicy
    {
    public:
        ValueType getRewardForStateAndAction(const StateType state, const ActionType action) const
        {
            return 0;
        }

        void setRewardForStateAndAction(const StateType state, const ActionType action, const ValueType& reward)
        {
        }
    };

    template<typename StateType, typename ActionType, typename ValueType, size_t NumberOfStates, size_t NumberOfActions>
    class QTableRewardPolicy
    {
    public:
        QTableRewardPolicy()
        {
            size_t bufferIndex = 0;

            for(StateType state = 0;state < NumberOfStates;++state)
            {
                for(ActionType action = 0;action < NumberOfActions;++action)
                {
                    new(&this->mRewardTableBuffer[bufferIndex]) ValueType(0);
                    bufferIndex += sizeof(ValueType);
                }
            }
        }

        ValueType getRewardForStateAndAction(const StateType state, const ActionType action) const
        {
            size_t stateIndex;
            size_t actionIndex;
            size_t bufferIndex;

            if((state < NumberOfStates) && (action < NumberOfActions))
            {
                stateIndex = (state * NumberOfActions * sizeof(ValueType));
                actionIndex = (action * sizeof(ValueType));
                bufferIndex = (stateIndex + actionIndex);
                const ValueType* pValue = reinterpret_cast<const ValueType*>(&this->mRewardTableBuffer[bufferIndex]);
                return *pValue;
            }
            else
            {
                return 0;
            }
        }

        void setRewardForStateAndAction(const StateType state, const ActionType action, const ValueType& reward)
        {
            ValueType* pValue;
            size_t stateIndex;
            size_t actionIndex;
            size_t bufferIndex;

            if((state < NumberOfStates) && (action < NumberOfActions))
            {
                stateIndex = (state * NumberOfActions * sizeof(ValueType));
                actionIndex = (action * sizeof(ValueType));
                bufferIndex = (stateIndex + actionIndex);
                pValue = reinterpret_cast<ValueType*>(&this->mRewardTableBuffer[bufferIndex]);
                *pValue = reward;
            }
        }
    private:
        unsigned char mRewardTableBuffer[sizeof(ValueType) * NumberOfStates * NumberOfActions];
    };

    template<typename ValueType>
    class NullLearningPolicy
    {
    public:
        ValueType getDiscountFactor() const
        {
            return 0;
        }

        ValueType getLearningRate() const
        {
            return 0;
        }

        void setDiscountFactor(const ValueType& discountFactor)
        {
        }

        void setLearningRate(const ValueType& learningRate)
        {
        }
    };

    template<typename ValueType>
    class DefaultLearningPolicy
    {
    public:
        DefaultLearningPolicy() : mLearningRate(0), mDiscountFactor(0) {}

        ValueType getDiscountFactor() const
        {
            return this->mDiscountFactor;
        }

        ValueType getLearningRate() const
        {
            return this->mLearningRate;
        }

        void setDiscountFactor(const ValueType& discountFactor)
        {
            if(discountFactor < tinymind::Constants<ValueType>::one())
            {
                this->mDiscountFactor = discountFactor;
            }
        }

        void setLearningRate(const ValueType& learningRate)
        {
            if(learningRate < tinymind::Constants<ValueType>::one())
            {
                this->mLearningRate = learningRate;
            }
        }
    private:
        ValueType mLearningRate;
        ValueType mDiscountFactor;
    };

    template<   typename StateType,
                typename ActionType,
                typename ValueType,
                size_t NumberOfStates,
                size_t NumberOfActions,
                template<typename> class QLearningRandomNumberPolicy,
                class RewardPolicy = QTableRewardPolicy<StateType, ActionType, ValueType, NumberOfStates, NumberOfActions>,
                template<typename> class QLearningPolicy = DefaultLearningPolicy
            >
    class QLearningEnvironment
    {
    public:
        typedef StateType EnvironmentStateType;
        typedef ActionType EnvironmentActionType;
        typedef ValueType EnvironmentValueType;
        typedef QLearningRandomNumberPolicy<ActionType> RandomNumberPolicy;
        typedef QLearningPolicy<ValueType> LearningPolicy;

        static const size_t EnvironmentNumberOfStates = NumberOfStates;
        static const size_t EnvironmentNumberOfActions = NumberOfActions;
        static const StateType EnvironmentInvalidState = tinymind::limits<StateType>::max;
        static const ActionType EnvironmentInvalidAction = tinymind::limits<ActionType>::max;

        struct experience_t
        {
            experience_t() : state(EnvironmentInvalidState), action(EnvironmentInvalidAction), reward(0), newState(EnvironmentInvalidState) {};

            experience_t(const experience_t& other)
            {
                this->state = other.state;
                this->action = other.action;
                this->reward = other.reward;
                this->newState = other.newState;
            }

            experience_t& operator=(const experience_t& other)
            {
                this->state = other.state;
                this->action = other.action;
                this->reward = other.reward;
                this->newState = other.newState;

                return *this;
            }

            StateType state;
            ActionType action;
            ValueType reward;
            StateType newState;
        };

        QLearningEnvironment(const ValueType& learningRate, const ValueType& discountFactor, const size_t randomActionDecisionPoint)
        {
            size_t bufferIndex = 0;

            for(ActionType action = 0;action < NumberOfActions;++action)
            {
                new(&this->mActionsBuffer[bufferIndex]) ActionType(0);
                bufferIndex += sizeof(ActionType);
            }

            this->setLearningRate(learningRate);
            this->setDiscountFactor(discountFactor);
            this->setRandomActionDecisionPoint(randomActionDecisionPoint);
        }

        ActionType chooseRandomActionFromValidActions(ActionType const* const validActions, const size_t numberOfValidActions)
        {
            ActionType actionIndex;
            ActionType action;

            switch(numberOfValidActions)
            {
                case 0:
                    action = EnvironmentInvalidAction;
                    break;
                case 1:
                    action = validActions[0];
                    break;
                default:
                    actionIndex = this->mRandomNumberPolicy.randInt(0, static_cast<ActionType>(numberOfValidActions - 1));
                    action = validActions[actionIndex];
                    break;
            }

            return action;
        }

        ValueType getDiscountFactor() const
        {
            return this->mLearningPolicy.getDiscountFactor();
        }

        ValueType getInitialQTableValue() const
        {
            return static_cast<ValueType>(0);
        }

        ValueType getLearningRate() const
        {
            return this->mLearningPolicy.getLearningRate();
        }

        ActionType getRandomActionForState(const StateType state) const
        {
            ActionType* pActions = reinterpret_cast<ActionType*>(&this->mActionsBuffer[0]);
            const size_t numberOfValidActions = this->getValidActionsForState(state, pActions);
            ActionType action;

            switch(numberOfValidActions)
            {
                case 0:
                    action = EnvironmentInvalidAction;
                    break;
                case 1:
                    action = *pActions;
                    break;
                default:
                    action = this->chooseRandomActionFromValidActions(pActions, numberOfValidActions);
                    break;
            }

            return action;
        }

        size_t getRandomActionDecisionPoint() const
        {
            return this->mRandomNumberPolicy.getRandomActionDecisionPoint();
        }

        ValueType getRewardForStateAndAction(const StateType state, const ActionType action) const
        {
            return this->mRewardPolicy.getRewardForStateAndAction(state, action);
        }

        void setDiscountFactor(const ValueType& discountFactor)
        {
            this->mLearningPolicy.setDiscountFactor(discountFactor);
        }

        void setLearningRate(const ValueType& learningRate)
        {
            this->mLearningPolicy.setLearningRate(learningRate);
        }

        void setRandomActionDecisionPoint(const size_t randomActionDecisionPoint)
        {
            this->mRandomNumberPolicy.setRandomActionDecisionPoint(randomActionDecisionPoint);
        }

        void setRewardForStateAndAction(const StateType state, const ActionType action, const ValueType& reward)
        {
            this->mRewardPolicy.setRewardForStateAndAction(state, action, reward);
        }

        bool shouldChooseRandomAction()
        {
            const size_t randomActionDecisionPoint = this->mRandomNumberPolicy.getRandomActionDecisionPoint();
            size_t value;
            bool chooseRandomAction;
            
            if (0 == randomActionDecisionPoint)
            {
                return false;
            }

            if (100 == randomActionDecisionPoint)
            {
                return true;
            }

            value = this->mRandomNumberPolicy.randInt(0, 100);
            chooseRandomAction = (value <= randomActionDecisionPoint);
            
            return chooseRandomAction;
        }
    protected:
        RandomNumberPolicy mRandomNumberPolicy;
        RewardPolicy mRewardPolicy;
        LearningPolicy mLearningPolicy;
    private:
        unsigned char mActionsBuffer[sizeof(ActionType) * NumberOfActions];
    };

    template<typename EnvironmentType, typename QLearnerQValuePolicy>
    class ArgMaxPolicy
    {
    public:
        typedef typename EnvironmentType::EnvironmentStateType StateType;
        typedef typename EnvironmentType::EnvironmentActionType ActionType;
        typedef typename EnvironmentType::EnvironmentValueType ValueType;

        static ActionType selectBestActionForState(const StateType state, ActionType const* const actions, const size_t numberOfValidActions, const QLearnerQValuePolicy& qValuePolicy)
        {
            ActionType bestAction;
            ValueType bestQValue;
            ValueType qValue;
            ValueType qValue0;
            ValueType qValue1;
            ActionType action;
            ActionType actionIndex;

            switch(numberOfValidActions)
            {
                case 0:
                    bestAction = EnvironmentType::EnvironmentInvalidAction;
                    break;
                case 1:
                    bestAction = actions[0];
                    break;
                case 2:
                    qValue0 = qValuePolicy.getQValue(state, actions[0]);
                    qValue1 = qValuePolicy.getQValue(state, actions[1]);
                    if(qValue0 > qValue1)
                    {
                        bestAction = actions[0];
                    }
                    else
                    {
                        bestAction = actions[1];
                    }
                    break;
                default:
                    bestQValue = 0;
                    bestAction = EnvironmentType::EnvironmentInvalidAction;
                    for(actionIndex = 0;actionIndex < numberOfValidActions;++actionIndex)
                    {
                        action = actions[actionIndex];
                        qValue = qValuePolicy.getQValue(state, action);
                        if(qValue > bestQValue)
                        {
                            bestQValue = qValue;
                            bestAction = action;
                        }
                    }
                    break;
            }
            
            return bestAction;
        }
    };

    template<typename EnvironmentType>
    class QValueTablePolicy
    {
    public:
        typedef typename EnvironmentType::EnvironmentStateType StateType;
        typedef typename EnvironmentType::EnvironmentActionType ActionType;
        typedef typename EnvironmentType::EnvironmentValueType ValueType;

        static const size_t NumberOfStates = EnvironmentType::EnvironmentNumberOfStates;
        static const size_t NumberOfActions = EnvironmentType::EnvironmentNumberOfActions;

        QValueTablePolicy()
        {
            size_t bufferIndex = 0;

            for(StateType state = 0;state < NumberOfStates;++state)
            {
                for(ActionType action = 0;action < NumberOfActions;++action)
                {
                    new(&this->mQTableBuffer[bufferIndex]) ValueType(0);
                    bufferIndex += sizeof(ValueType);
                }
            }
        }

        ValueType getQValue(const StateType state, const ActionType action) const
        {
            size_t stateIndex;
            size_t actionIndex;
            size_t bufferIndex;

            if((state < NumberOfStates) && (action < NumberOfActions))
            {
                stateIndex = (state * NumberOfActions * sizeof(ValueType));
                actionIndex = (action * sizeof(ValueType));
                bufferIndex = (stateIndex + actionIndex);
                const ValueType* pValue = reinterpret_cast<const ValueType*>(&this->mQTableBuffer[bufferIndex]);
                return *pValue;
            }
            else
            {
                return 0;
            }
        }

        void setQValue(const StateType state, const ActionType action, const ValueType& value)
        {
            ValueType* pValue;
            size_t stateIndex;
            size_t actionIndex;
            size_t bufferIndex;

            if((state < NumberOfStates) && (action < NumberOfActions))
            {
                stateIndex = (state * NumberOfActions * sizeof(ValueType));
                actionIndex = (action * sizeof(ValueType));
                bufferIndex = (stateIndex + actionIndex);
                pValue = reinterpret_cast<ValueType*>(&this->mQTableBuffer[bufferIndex]);
                *pValue  = value;
            }
        }
    private:
        unsigned char mQTableBuffer[sizeof(ValueType) * NumberOfStates * NumberOfActions];
    };

    template<   typename EnvironmentType,
                template<typename> class QLearnerQValuePolicy = QValueTablePolicy,
                template<typename, typename> class QLearnerStateToActionPolicy = ArgMaxPolicy
            >
    class QLearner
    {
    public:
        typedef typename EnvironmentType::EnvironmentStateType StateType;
        typedef typename EnvironmentType::EnvironmentActionType ActionType;
        typedef typename EnvironmentType::EnvironmentValueType ValueType;
        typedef typename EnvironmentType::experience_t experience_t;
        typedef QLearnerQValuePolicy<EnvironmentType> QValuePolicy;
        typedef QLearnerStateToActionPolicy<EnvironmentType, QValuePolicy> StateToActionPolicy;

        static const size_t NumberOfStates = EnvironmentType::EnvironmentNumberOfStates;
        static const size_t NumberOfActions = EnvironmentType::EnvironmentNumberOfActions;
        static const StateType InvalidState = EnvironmentType::EnvironmentInvalidState;
        static const ActionType InvalidAction = EnvironmentType::EnvironmentInvalidAction;

        QLearner(const ValueType& learningRate, const ValueType& discountFactor, const size_t randomActionDecisionPoint) : 
            mEnvironment(learningRate, discountFactor, randomActionDecisionPoint), mState(InvalidState)
        {
            size_t bufferIndex = 0;

            for(ActionType action = 0;action < NumberOfActions;++action)
            {
                new(&this->mActionsBuffer[bufferIndex]) ActionType(0);
                bufferIndex += sizeof(ActionType);
            }

            for(StateType state = 0;state < NumberOfStates;++state)
            {
                for(ActionType action = 0;action < NumberOfActions;++action)
                {
                    this->mQValuePolicy.setQValue(state, action, this->mEnvironment.getInitialQTableValue());
                }
            }
        }

        EnvironmentType& getEnvironment()
        {
            return this->mEnvironment;
        }

        ValueType getQValue(const StateType state, const ActionType action) const
        {
            return this->mQValuePolicy.getQValue(state, action);
        }

        StateType getState() const
        {
            return this->mState;
        }

        void setQValue(const StateType state, const ActionType action, const ValueType& value)
        {
            this->mQValuePolicy.setQValue(state, action, value);
        }

        void setState(const StateType state)
        {
            if(state < NumberOfStates)
            {
                this->mState = state;
            }
        }

        void startNewEpisode()
        {
            this->mState = InvalidState;
        }

        ActionType takeAction(const StateType state)
        {
            const ActionType action = this->chooseAction(state);
            experience_t experience;

            this->setState(state);

            experience.state = state;
            experience.action = action;

            this->mEnvironment.takeAction(experience);

            return action;
        }

        void updateFromExperience(const experience_t& experience)
        {
            const ValueType learningRate = this->mEnvironment.getLearningRate();
            ValueType oneMinusLearningRate;
            ValueType discountFactor;
            ValueType oldQValue;
            ValueType futureQValue;
            ValueType qValue;

            if(learningRate > 0)
            {
                oneMinusLearningRate = (tinymind::Constants<ValueType>::one() - learningRate);
                discountFactor = this->mEnvironment.getDiscountFactor();

                if(InvalidState == this->mState)
                {
                    oldQValue = 0;
                }
                else
                {
                    oldQValue = this->mQValuePolicy.getQValue(this->mState, experience.action);
                }

                futureQValue = calculateFutureQValue(experience.newState);

                qValue = ((oneMinusLearningRate * oldQValue) + (learningRate * (experience.reward + (discountFactor * futureQValue))));

                if(InvalidState != this->mState)
                {
                    this->mQValuePolicy.setQValue(this->mState, experience.action, qValue);
                }
            }

            this->mState = experience.newState;
        }
    private:
        // Private methods
        ValueType calculateFutureQValue(const StateType state)
        {
            const ActionType action = this->chooseAction(state);
            ValueType qValue;

            if (InvalidAction == action)
            {
                qValue = 0;
            }
            else
            {
                qValue = this->mQValuePolicy.getQValue(state, action);
            }

            return qValue;
        }

        ActionType chooseAction(const StateType state)
        {
            ActionType* pActions = reinterpret_cast<ActionType*>(&this->mActionsBuffer[0]);
            const size_t numberOfValidActions = this->mEnvironment.getValidActionsForState(state, pActions);
            ActionType action;

            if (this->mEnvironment.shouldChooseRandomAction())
            {
                action = this->mEnvironment.chooseRandomActionFromValidActions(pActions, numberOfValidActions);
            }
            else
            {
                action = StateToActionPolicy::selectBestActionForState(state, pActions, numberOfValidActions, this->mQValuePolicy);
                if(InvalidAction == action)
                {
                    action = this->mEnvironment.chooseRandomActionFromValidActions(pActions, numberOfValidActions);
                }
            }

            return action;
        }

    private:
        EnvironmentType mEnvironment;
        QValuePolicy mQValuePolicy;
        StateType mState;
        unsigned char mActionsBuffer[sizeof(ActionType) * NumberOfActions];

        static_assert(NumberOfStates > 1, "Invalid number of states.");
        static_assert(NumberOfActions > 1, "Invalid number of actions.");
        static_assert((tinymind::limits<StateType>::max > (NumberOfStates - 1)), "State type too small to hold number of states. Max value reserved for invalid state.");
        static_assert((tinymind::limits<ActionType>::max > (NumberOfActions - 1)), "Action type too small to hold number of actions. Max value reserved for invalid action.");
        static_assert(IsSigned<StateType>::result == false, "Must use an unsigned type to represent states.");
        static_assert(IsSigned<ActionType>::result == false, "Must use an unsigned type to represent actions.");
    };
}
