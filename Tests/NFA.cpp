/**
 * @brief Unit Tests for NFA conversion Behavior
 *
 * @par
 * Copyright Jeremy Wright (c) 2011
 * Creative Commons Attribution-ShareAlike 3.0 Unported License.
 */


#include <gtest/gtest.h>
#include "Automaton.h"

class NFATests : public ::testing::Test {
    friend class Automaton;
    protected:
        virtual void SetUp()
        {
            bookProblem = Automaton::construct();
            
            bookProblem->AddState(1);
            bookProblem->AddState(2);
            bookProblem->AddState(3);
    
            bookProblem->AddTransition(1,2, "b");
            bookProblem->AddTransition(1,3, bookProblem->EPSILON);
            bookProblem->AddTransition(2,3, "a");

            bookProblem->AddTransition(2,3, "b");
            bookProblem->AddTransition(3,1, "a");

            bookProblem->AddTransition(2,2, "a");

            bookProblem->SetStartState(1);
            bookProblem->AddFinalState(1);

        }

        virtual void TearDown()
        {
        }

        void NFATest_EpsilonSize(Automaton::Ptr g, size_t size)
        {
            EXPECT_EQ(size, g->states[1]->transitions.count(g->EPSILON));
        }

        Automaton::Ptr bookProblem;
};

TEST_F(NFATests, CorrectConstruction)
{
    EXPECT_TRUE(bookProblem->IsNFA());
}

TEST_F(NFATests, Converstion)
{
    Automaton::Ptr dfa = bookProblem->opSubsetConversion();
    EXPECT_FALSE(dfa->IsNFA());
}

TEST_F(NFATests, EpsilonClosure2)
{
    Automaton::Ptr incomplete = Automaton::construct();
    incomplete->AddState(1);
    incomplete->AddState(2);
    incomplete->AddState(3);
    incomplete->AddState(4);

    incomplete->AddTransition(1,2,incomplete->EPSILON);
    incomplete->AddTransition(1,3,incomplete->EPSILON);
    incomplete->AddTransition(3,4,"b");

    //NFATest_EpsilonSize(incomplete, 2);

    
    list<int> expectedClosure;
    expectedClosure.push_back(1);
    expectedClosure.push_back(2);
    expectedClosure.push_back(3);

    set<int> calcClosure;
    incomplete->EpsilonClosure(1, calcClosure);
    
    EXPECT_EQ(expectedClosure.size(), calcClosure.size());
    set<int>::const_iterator c = calcClosure.begin();
    for(list<int>::const_iterator e = expectedClosure.begin();
            e != expectedClosure.end();
            ++e, ++c)
    {
        EXPECT_EQ(*e, *c);
    }
}

TEST_F(NFATests, EpsilonClosure)
{
    list<int> expectedClosure;
    expectedClosure.push_back(1);
    expectedClosure.push_back(3);

    set<int> calcClosure;
    bookProblem->EpsilonClosure(1, calcClosure);
    
    set<int>::const_iterator c = calcClosure.begin();
    for(list<int>::const_iterator e = expectedClosure.begin();
            e != expectedClosure.end();
            ++e, ++c)
    {
        EXPECT_EQ(*e, *c);
    }
}

