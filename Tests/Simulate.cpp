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
        //Create a simple automaton
        // ->0----a---->(1)--b--
        //              ^    |
        //               -----
        // Accepted Strings: ab*
        virtual void SetUp()
        {
            a = Automaton::construct();
            a->AddState(0);
            a->AddState(1);

            a->AddTransition(0, 1, "a");
            a->AddTransition(1, 1, "b");

            a->SetStartState(0);
            a->AddFinalState(1);
            complementA = a->opComplement();

        }

        virtual void TearDown()
        {
        }

        Automaton::Ptr a;
        Automaton::Ptr complementA;
};

TEST_F(Simulate, Simple)
{
    EXPECT_EQ(1,1);
}

TEST_F(Simulate, TestStartFinal)
{
    EXPECT_EQ(0, a->GetStartState());
    list<int> states = a->GetFinalStates();
    list<int>::const_iterator i = states.begin();
    EXPECT_EQ(1, *i);

}

TEST_F(Simulate, RunAcceptingString)
{
    //Run the String a
    list<string> input;
    input.push_back("a");
    EXPECT_TRUE(a->Run(input));
    EXPECT_FALSE(complementA->Run(input));
}

TEST_F(Simulate, RunAcceptingString1)
{
    //Run the String a
    list<string> input;
    input.push_back("a");
    input.push_back("b");
    EXPECT_TRUE(a->Run(input));
    EXPECT_FALSE(complementA->Run(input));
}

TEST_F(Simulate, RunAcceptingString2)
{
    //Run the String a
    list<string> input;
    input.push_back("a");
    input.push_back("b");
    input.push_back("b");
    EXPECT_TRUE(a->Run(input));
    EXPECT_FALSE(complementA->Run(input));
}

TEST_F(Simulate, RunNotAcceptingString)
{
    //Run the String a
    list<string> input;
    input.push_back("a");
    input.push_back("a");
    EXPECT_FALSE(a->Run(input));
    EXPECT_FALSE(complementA->Run(input)); //This is also false, because it is a string not in the language.
}

TEST_F(Simulate, EmptyString)
{
    //Run the String a
    list<string> input;
    EXPECT_FALSE(a->Run(input));
    EXPECT_TRUE(complementA->Run(input)); 

}

TEST_F(Simulate, Union)
{
    EXPECT_TRUE(false);
}

TEST_F(Simulate, Intersect)
{
    EXPECT_TRUE(false);
}

