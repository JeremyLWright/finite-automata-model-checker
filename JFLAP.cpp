/**
 * @brief Implementation for translating between JFLAP and AUtomatons
 *
 * @par
 * Copyright Jeremy Wright (c) 2011
 * Creative Commons Attribution-ShareAlike 3.0 Unported License.
 */

#include "JFLAP.h"
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
namespace bf = boost::filesystem;
using namespace std;
using namespace rapidxml;


JFLAP::JFLAP(string filename)
{
    size_t const documentSize = bf::file_size(filename);    
    cout << "File is: " << bf::file_size(filename) << " bytes." << endl;

    xml_document_buffer = new char[documentSize];
    ifstream inputFile(filename.c_str());
    inputFile.read(xml_document_buffer, documentSize);
    
    rapidxml::xml_document<> doc;
    doc.parse<0>(xml_document_buffer);

    //Verify the correct JFLAP input file
    if(strncmp(doc.first_node()->first_node()->value(), "fa", sizeof("fa")))
    {
        cerr << "JFLAP file must be a Finite Automaton." << endl;
        //TODO throw exception
    }
    xml_node<>* automatonStart = doc.first_node()->first_node()->next_sibling();

    //Verify the file is in the expected format.
    if(strncmp(automatonStart->name(), "automaton", sizeof("automaton")))
    {
    
    cerr << "File is not in expected format. Expected to see 'automaton', got " << automatonStart->name() << endl;
    //TODO throw exception
    }

    a = Automaton::construct();
    //Parse the xml document
    for(xml_node<>* node = automatonStart->first_node();
            node;
            node = node->next_sibling())
    {
        if(strncmp(node->name(), "state", sizeof("state")) == 0)
        {
            xml_attribute<>* attr = node->first_attribute();
            int StateId;
            from_string<int>(StateId, attr->value(), std::dec);
            a->AddState(StateId);
            //Look for initial or final
            for(xml_node<>* stateData = node->first_node();
                    stateData;
                    stateData = stateData->next_sibling())
            {
                if(strncmp(stateData->name(), "initial", sizeof("initial")) == 0)
                {
                    cout << "Marking Node " << StateId << " as initial." << endl;
                    a->SetStartState(StateId);
                }
                else if(strncmp(stateData->name(), "final", sizeof("final")) == 0)
                {
                    cout << "Marking Node " << StateId << " as final." << endl;
                    a->AddFinalState(StateId);
                }
            }

        }
        else if(strncmp(node->name(), "transition", sizeof("transition")) == 0)
        {
            int fromId = 0;
            int toId = 0;

            from_string<int>(fromId, node->first_node("from")->value(), std::dec);
            from_string<int>(toId, node->first_node("to")->value(), std::dec);
            string read(node->first_node("read")->value());
            cout << "Add Transition: " << fromId << " " << toId << " " << read << endl;
            a->AddTransition(fromId, toId, read);
        }
    }

}

JFLAP::~JFLAP()
{
    delete xml_document_buffer;
}

JFLAP::Ptr JFLAP::construct(string filename)
{
    JFLAP::Ptr c(new JFLAP(filename));
    c->self = c;
    return c;
}

Automaton::Ptr JFLAP::GetAutomaton() const
{
    return a;
}

void OutputAutomaton(Automaton::Ptr automaton, string filename)
{

}
