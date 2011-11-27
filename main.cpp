/**
 * @brief Basic Automaton Simulator for CSE355 Extra Credit Project
 *
 * @par
 * Copyright Jeremy Wright (c) 2011
 * Creative Commons Attribution-ShareAlike 3.0 Unported License.
 */

#include "rapidxml.hpp"
#include "rapidxml_print.hpp"
#include "rapidxml_utils.hpp"
#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <stdint.h>
#include <cassert>
#include <sstream>
#include "Automaton.h"
#include "JFLAP.h"
namespace bf = boost::filesystem;
using namespace std;
using namespace rapidxml;
int main(int argc, const char *argv[])
{
    assert(argc >= 2);
     
    cout << "Document Complete." << endl;

    JFLAP::Ptr jflap = JFLAP::construct(argv[1]);
    Automaton::Ptr a = jflap->GetAutomaton();

    list<string> input;
    input.push_back("a");
    input.push_back("b");
    if( a->Run(input))
    {
        cout << "Accepted." << endl;
    }

    return 0;
}
