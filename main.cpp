/**
 * @brief Basic Automaton Simulator for CSE355 Extra Credit Project
 *
 * @par
 * Copyright Jeremy Wright (c) 2011
 * Creative Commons Attribution-ShareAlike 3.0 Unported License.
 */

#include <iostream>
#include <stdint.h>
#include <cassert>
#include <sstream>
#include "Automaton.h"
#include "JFLAP.h"
using namespace std;


int main(int argc, const char *argv[])
{
    if(argc != 3)
    {
        cout << "Usage: " << endl;
        cout << argv[0] << " Specification.jff Model.jff" << endl;
        return -1;
    }
     
    JFLAP* jflap = new JFLAP(argv[1]);
    Automaton::Ptr specification = jflap->GetAutomaton();
    delete jflap;

    JFLAP* implInput = new JFLAP(argv[2]);
    Automaton::Ptr implementation = implInput->GetAutomaton();
    delete implInput;

    Automaton::Ptr lm = implementation->opIntersect(specification->opComplement());

    list<string> input;
    if(lm->FindSequence(input))
    {
        cout << "Found at least 1 path" << endl;
        for(list<string>::const_iterator i = input.begin();
                i != input.end();
                ++i)
        {
            cout << *i << ", ";
        }
        cout << endl;
    }
    else
    {
        cout << "Your model matches the specification, L(M)=EmptySet" << endl;
    }


    return 0;
}
