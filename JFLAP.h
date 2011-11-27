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
    typedef std::tr1::shared_ptr<JFLAP> Ptr;
    typedef std::tr1::weak_ptr<JFLAP> WeakPtr;
    static JFLAP::Ptr construct(string filename);
    Automaton::Ptr GetAutomaton() const;
    static void OutputAutomaton(Automaton::Ptr automaton, string filename);
    virtual ~JFLAP();
private:
    Automaton::Ptr a;
    char* xml_document_buffer;
    JFLAP(string filename);
    JFLAP::WeakPtr self;
    
};

#endif /* end of include guard: _JFLAP */

