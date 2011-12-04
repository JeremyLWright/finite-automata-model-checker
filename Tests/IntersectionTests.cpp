/**
 * @brief Unit Tests for testing the Intersection
 *
 * @par
 * Copyright Jeremy Wright (c) 2011
 * Creative Commons Attribution-ShareAlike 3.0 Unported License.
 */

#include <gtest/gtest.h>
#include "Automaton.h"
#include <iostream>
using namespace std;

class IntersectTests : public ::testing::Test {
    protected:
        //Create a simple automaton
        // ->(A)<---b---->B--a--
        //  ^ |           ^    |
        //  -a-            -----
        // Accepted Strings: a*+(bb)*
        virtual void SetUp()
        {
            a = Automaton::construct();
            a->AddState("A");
            a->AddState("B");

            a->AddTransition("A", "B", "b");
            a->AddTransition("A", "A", "a");
            a->AddTransition("B", "B", "a");
            a->AddTransition("B", "A", "b");
            

            a->SetStartState("A");
            a->AddFinalState("A");

            //Create a simple automaton
            //    -<--a--<----
            //    |          |        ----<---
            //    |          ^       |        |
            // ->(x)---b--->(y)--b-->z--a,b-->^
            // ^  |               
            //  -a-          
            // Accepted Strings: 
            b = Automaton::construct();
            b->AddState("x");
            b->AddState("y");
            b->AddState("z");
            
            b->AddTransition("x","x","a");
            b->AddTransition("x","y","b");
            b->AddTransition("y","x","a");
            b->AddTransition("y","z","b");
            b->AddTransition("z","z","a");
            b->AddTransition("z","z","b");

            b->SetStartState("x");
            b->AddFinalState("x");
            b->AddFinalState("y");


        }

        virtual void TearDown()
        {
        }

        Automaton::Ptr a;
        Automaton::Ptr b;
};

TEST_F(IntersectTests, Intersect)
{
    Automaton::Ptr intersect = a->opIntersect(b);
    intersect->PrettyPrint();
    
}

