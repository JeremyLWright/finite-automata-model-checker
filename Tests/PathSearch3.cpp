/**
 * @brief Unit Tests for finding accepting paths
 *
 * @par
 * Copyright Jeremy Wright (c) 2011
 * Creative Commons Attribution-ShareAlike 3.0 Unported License.
 */

#include <gtest/gtest.h>
#include "Automaton.h"

class PathSearch3 : public ::testing::Test {
    protected:
        //Create a simple automaton
        //          9--x-->8--x-->(7)-x-->6
        //          ^      ^      ^       ^
        //          |      |      |       |
        //          x      y      y       y
        //          |      |      |       |
        // ->1--a-->2--b-->3--c-->4--d--->5
        //          |      |      |       |
        //          z      k      k       k
        //          |      |      |       |
        //          10--z->11--z->12--z->13
        // Accepted String: abcy
        virtual void SetUp()
        {
            a = Automaton::construct();
            a->AddState(0);
            a->AddState(1);
            a->AddState(2);
            a->AddState(3);
            a->AddState(4);
            a->AddState(5);
            a->AddState(6);
            a->AddState(7);
            a->AddState(8);
            a->AddState(9);
            a->AddState(10);
            a->AddState(11);
            a->AddState(12);
            a->AddState(13);

            a->AddTransition(1, 2, "a");
            a->AddTransition(2, 3, "b");
            a->AddTransition(3, 4, "c");
            a->AddTransition(4, 5, "d");
            //Up Transistions
            a->AddTransition(2, 9, "x");
            a->AddTransition(3, 8, "y");
            a->AddTransition(4, 7, "y");
            a->AddTransition(5, 6, "y");
            //Down Transistions
            a->AddTransition(2, 10, "z");
            a->AddTransition(3, 11, "k");
            a->AddTransition(4, 12, "k");
            a->AddTransition(5, 13, "k");
            //Lower Transitions
            a->AddTransition(10, 11, "z");
            a->AddTransition(11, 12, "z");
            a->AddTransition(12, 13, "z");
            //Upper Transitions
            a->AddTransition(9, 8, "x");
            a->AddTransition(8, 7, "x");
            a->AddTransition(7, 6, "x");


            a->SetStartState(1);
            a->AddFinalState(7);
            complementA = a->opComplement();

        }

        virtual void TearDown()
        {
        }

        Automaton::Ptr a;
        Automaton::Ptr complementA;
};


TEST_F(PathSearch3, RunAcceptingString)
{
    //Run the String a
    list<string> input;
    input.push_back("a");
    input.push_back("b");
    input.push_back("c");
    input.push_back("y");
    EXPECT_TRUE(a->Run(input));
    EXPECT_FALSE(complementA->Run(input));
}

TEST_F(PathSearch3, FindSequence)
{
    list<string> AcceptingSequence;

    EXPECT_TRUE(a->FindSequence(AcceptingSequence));
    EXPECT_FALSE(AcceptingSequence.empty());
    vector<string> directAccess(AcceptingSequence.begin(), AcceptingSequence.end());
    EXPECT_EQ("a", directAccess[0]);
    if(directAccess[1] == "b")
    {
        if(directAccess[2] == "c")
        {
            EXPECT_EQ("a", directAccess[0]);
            EXPECT_EQ("b", directAccess[1]);
            EXPECT_EQ("c", directAccess[2]);
            EXPECT_EQ("y", directAccess[3]);
        }
        else if(directAccess[2] == "y")
        {
            EXPECT_EQ("a", directAccess[0]);
            EXPECT_EQ("b", directAccess[1]);
            EXPECT_EQ("y", directAccess[2]);
            EXPECT_EQ("x", directAccess[3]);
        }
        else
        {
            FAIL();
        }
    }
    else if(directAccess[1] == "x")
    {
        EXPECT_EQ("a", directAccess[0]);
        EXPECT_EQ("x", directAccess[1]);
        EXPECT_EQ("x", directAccess[2]);
        EXPECT_EQ("x", directAccess[3]);
    }
    else
    {
        FAIL();
    }
}
