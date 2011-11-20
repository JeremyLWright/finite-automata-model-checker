#include "Automaton.h"
Automaton::Ptr Automaton::construct()
{
    Automaton::Ptr c(new Automaton());
    c->self = c;
    return c;
}

Automaton::Automaton()
{
}

Automaton::~Automaton()
{
}

void Automaton::AddState(int Name)
{
    if(states.find(Name) == states.end())
    {
        states[Name] = State::construct(Name);
    }
    else
    {
        throw Name;
    }

}

void Automaton::AddTransition(int FromNode, int ToNode, string label)
{
    states[FromNode]->transitions[label] = ToNode;
}

void Automaton::SetStartState(int Name)
{
    startState = Name;
    states[Name]->Property(State::START);
    ///TODO throw an exceptions if there is already a start state.
}

void Automaton::AddFinalState(int Name)
{
    states[Name]->Property(State::FINAL);
}

int Automaton::GetStartState()
{
    State::Ptr s = states[startState];
    if(s->Property() == State::START)
    {
        return s->Name(); 
    }
    else
    {
        ///TODO throw no start state defined
    }
}

list<int> Automaton::GetFinalStates()
{
    list<int> finals;
    for(map<int, State::Ptr>::const_iterator f = states.begin();
            f != states.end();
            ++f)
    {
        if(f->second->Property() == State::FINAL)
            finals.push_back(f->second->Name());
    }
    if(finals.empty())
    {
        ///TODO throw empty list

    } 
    return finals;
}



Automaton::State::Ptr  Automaton::State::construct(int Name)
{
    Automaton::State::Ptr c(new Automaton::State(Name));
    c->self = c;
    return c;
}

Automaton::State::State(int Name):
    property(NORMAL),
    name(Name)
{
}

Automaton::State::~State()
{
}

Automaton::State::Property_t  Automaton::State::Property()
{
    return property;
}

void  Automaton::State::Property(Property_t prop)
{
    property = prop;
}

int  Automaton::State::Name()
{
    return name;
}

void  Automaton::State::Name(int n)
{
    name = n;
}

bool Automaton::Run(list<string> input)
{
    int currentState = GetStartState();
    for(list<string>::const_iterator t = input.begin();
            t != input.end();
            ++t)
    {
     
        State::Ptr tempState = states[currentState];
        if(tempState->transitions.find(*t) == tempState->transitions.end())
        {
            return false; //There is no transition for the given input, reject
        }
        else
        {
            currentState = states[currentState]->transitions[*t];
        }
    }
    if(states[currentState]->Property() == State::FINAL)
    {
        return true;
    }
    else
    {
        return false;
    }
}

