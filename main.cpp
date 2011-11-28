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
    assert(argc >= 2);
     
    JFLAP::Ptr jflap = JFLAP::construct(argv[1]);
    Automaton::Ptr specification = jflap->GetAutomaton();
    
    JFLAP::Ptr implInput = JFLAP::construct(argv[2]);
    Automaton::Ptr implementation = implInput->GetAutomaton();

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
