/**
 * @brief Unit Tests for simulating an automaton
 *
 * @par
 * Copyright Jeremy Wright (c) 2011
 * Creative Commons Attribution-ShareAlike 3.0 Unported License.
 */

#include <gtest/gtest.h>
#include "Automaton.h"

class SystemLevel : public ::testing::Test {
    friend class Automaton;
    protected:
        virtual void SetUp()
        {
            spec = Automaton::construct();
            spec->AddState(0);
            spec->AddState(1);

            spec->AddTransition(0, 1, "a");
            spec->AddTransition(1, 1, "a");
            spec->AddTransition(1, 1, "b");

            spec->SetStartState(0);
            spec->AddFinalState(1);

            model = Automaton::construct();
            model->AddState(0);
            model->AddState(1);
            
            model->AddTransition(0,1,"b");
            model->AddTransition(0,0,"a");
            model->AddTransition(0,0,"b");
            model->AddTransition(1,1,"b");

            model->SetStartState(0);
            model->AddFinalState(1);


        }

        virtual void TearDown()
        {
        }

        Automaton::Ptr spec;
        Automaton::Ptr model;
};

TEST_F(SystemLevel, ComplementSpec)
{
    Automaton::Ptr cspec = spec->opComplement();
//    EXPECT_TRUE(cspec->states[0]->IsFinal())


    list<string> input;
    EXPECT_TRUE(cspec->Run(input));
    input.push_back("b");
    EXPECT_TRUE(cspec->Run(input));
    list<string> input2;
    input2.push_back("a");
    EXPECT_FALSE(cspec->Run(input2));
}

TEST_F(SystemLevel, ProductTest)
{
    Automaton::Ptr m = model->opIntersect(spec->opComplement());
    list<string> expected;
    m->FindSequence(expected);
    EXPECT_EQ(expected.size(), 1);
    EXPECT_EQ(*(expected.begin()), "b");
}
