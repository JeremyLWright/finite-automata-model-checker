/**
 * @brief Unit Tests for simulating an automaton
 *
 * @par
 * Copyright Jeremy Wright (c) 2011
 * Creative Commons Attribution-ShareAlike 3.0 Unported License.
 */

#include <gtest/gtest.h>
#include "Automaton.h"

class Simulate : public ::testing::Test {
    protected:
        virtual void SetUp()
        {
        }

        virtual void TearDown()
        {
        }

};

TEST_F(Simulate, Simple)
{
    EXPECT_EQ(1,1);
}

//Create a simple automaton
// ->0----a---->1--b--
//              ^    |
//               -----
// Accepted Strings: ab*
TEST_F(Simulate, RunOne)
{
    Automaton::Ptr a = Automaton::construct();

    a->AddState(0);
    a->AddState(1);

    a->AddTransition(0, 1, "a");
    a->AddTransition(1, 1, "b");

    a->SetStartState(0);
    a->AddFinalState(1);

    EXPECT_EQ(0, a->GetStartState());
    list<int> states = a->GetFinalStates();
    list<int>::const_iterator i = states.begin();
    EXPECT_EQ(1, *i);

}
