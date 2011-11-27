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
namespace bf = boost::filesystem;
using namespace std;
using namespace rapidxml;

template <typename T>
bool from_string(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
{
        std::istringstream iss(s, std::istringstream::in);
        return !(iss >> f >> t).fail();
}

int main(int argc, const char *argv[])
{
    assert(argc >= 2);
    size_t const documentSize = bf::file_size(argv[1]);
    cout << "File is: " << bf::file_size(argv[1]) << " bytes." << endl;

    char* xml_document_buffer = new char[documentSize];
    ifstream inputFile(argv[1]);
    inputFile.read(xml_document_buffer, documentSize);
    
    rapidxml::xml_document<> doc;
    doc.parse<0>(xml_document_buffer);

    //Verify the correct JFLAP input file
    if(strncmp(doc.first_node()->first_node()->value(), "fa", sizeof("fa")))
    {
        cerr << "JFLAP file must be a Finite Automaton." << endl;
        return -1;
    }
    xml_node<>* automatonStart = doc.first_node()->first_node()->next_sibling();

    //Verify the file is in the expected format.
    if(strncmp(automatonStart->name(), "automaton", sizeof("automaton")))
    {
        cerr << "File is not in expected format. Expected to see 'automaton', got " << automatonStart->name() << endl;
        return -1;
    }

    Automaton::Ptr a = Automaton::construct();
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

    cout << "Document Complete." << endl;


    list<string> input;
    input.push_back("a");
    input.push_back("b");
    if( a->Run(input))
    {
        cout << "Accepted." << endl;
    }

    delete xml_document_buffer;
    return 0;
}
