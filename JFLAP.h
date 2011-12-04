/**
 * @brief Behavior to translate between JFLAP files and Automatons
 *
 * @par
 * Copyright Jeremy Wright (c) 2011
 * Creative Commons Attribution-ShareAlike 3.0 Unported License.
 */

#ifndef _JFLAP
#define _JFLAP
#include "Automaton.h"

class JFLAP
{
public:
    Automaton::Ptr GetAutomaton() const;
    JFLAP(string filename);
    virtual ~JFLAP();
private:
    Automaton::Ptr a;
    
};

#endif /* end of include guard: _JFLAP */

