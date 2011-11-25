/**
 * @brief Unit Tests for finding accepting paths
 *
 * @par
 * Copyright Jeremy Wright (c) 2011
 * Creative Commons Attribution-ShareAlike 3.0 Unported License.
 */

#include <gtest/gtest.h>
#include "Automaton.h"

class PathSearch : public ::testing::Test {
    protected:
        //Create a simple automaton
        //          9--x-->8--x-->7---x-->6
        //          ^      ^      ^       ^
        //          |      |      |       |
        //          x      y      y       y
        //          |      |      |       |
        // ->1--a-->2--b-->3--c-->4--d-->(5)
        //          |      |      |       |
        //          z      k      k       k
        //          |      |      |       |
        //          10--z->11--z->12--z->13
        // Accepted String: abcd
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
            a->AddFinalState(5);
            complementA = a->opComplement();

        }

        virtual void TearDown()
        {
        }

        Automaton::Ptr a;
        Automaton::Ptr complementA;
};


TEST_F(PathSearch, RunAcceptingString)
{
    //Run the String a
    list<string> input;
    input.push_back("a");
    input.push_back("b");
    input.push_back("c");
    input.push_back("d");
    EXPECT_TRUE(a->Run(input));
    EXPECT_FALSE(complementA->Run(input));
}

TEST_F(PathSearch, FindPath)
{
    list<int> AcceptingSequence;
    list<int> ExpectedSequence;
#if 0
    ExpectedSequence.push_back("a");
    ExpectedSequence.push_back("b");
    ExpectedSequence.push_back("c");
    ExpectedSequence.push_back("d");
#endif

    ExpectedSequence.push_back(1);
    ExpectedSequence.push_back(2);
    ExpectedSequence.push_back(3);
    ExpectedSequence.push_back(4);
    ExpectedSequence.push_back(5);

    a->FindPath(1, AcceptingSequence);
    EXPECT_FALSE(AcceptingSequence.empty());
    list<int>::const_iterator j = ExpectedSequence.begin();
    for(list<int>::const_iterator i = AcceptingSequence.begin();
            i != AcceptingSequence.end();
            ++i, ++j)
    {
        EXPECT_EQ(*j, *i);
    }
}

