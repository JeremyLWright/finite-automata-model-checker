/**
 * @brief Unit Tests for simulating an automaton
 *
 * @par
 * Copyright Jeremy Wright (c) 2011
 * Creative Commons Attribution-ShareAlike 3.0 Unported License.
 */

#include <gtest/gtest.h>
#include "Automaton.h"

class SystemLevelTests : public ::testing::Test {
    protected:
        virtual void SetUp()
        {
            spec = Automaton::construct();
            spec->AddState("0");
            spec->AddState("1");
            spec->AddState("2");

            spec->AddTransition("0", "1", "a");
            spec->AddTransition("0", "2", "b");
            spec->AddTransition("1", "1", "a");
            spec->AddTransition("1", "1", "b");
            spec->AddTransition("2", "2", "a");
            spec->AddTransition("2", "2", "b");

            spec->SetStartState("0");
            spec->AddFinalState("1");

            model = Automaton::construct();
            model->AddState("i");
            model->AddState("ii");
            
            model->AddTransition("i","i","a");
            model->AddTransition("i","ii","b");
            model->AddTransition("ii","i","a");
            model->AddTransition("ii","ii","b");

            model->SetStartState("i");
            model->AddFinalState("ii");
        }

        virtual void TearDown()
        {
        }

        Automaton::Ptr spec;
        Automaton::Ptr model;
};

TEST_F(SystemLevelTests, ComplementSpec)
{
    Automaton::Ptr cspec = spec->opComplement();
    list<string> input;
    EXPECT_TRUE(cspec->Run(input));
    input.push_back("b");
    EXPECT_TRUE(cspec->Run(input));
    list<string> input2;
    input2.push_back("a");
    EXPECT_FALSE(cspec->Run(input2));
}

TEST_F(SystemLevelTests, ProductTest)
{
    Automaton::Ptr m = model->opIntersect(spec->opComplement());
    m->PrettyPrint();
    list<string> expected;
    m->FindSequence(expected);
    for(list<string>::const_iterator itr = expected.begin();
            itr != expected.end();
            ++itr)   
        cout << *itr << ", ";
    cout << endl;
   // EXPECT_EQ(expected.size(), 1);
   // EXPECT_EQ(*(expected.begin()), "b");
}
