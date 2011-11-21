#include "Automaton.h"
Automaton::Ptr Automaton::construct()
{
    Automaton::Ptr c(new Automaton());
    c->self = c;
    return c;
}

Automaton::Ptr Automaton::construct(Automaton::WeakPtr const copy)
{
    Ptr j = copy.lock();
    Automaton::Ptr c(new Automaton(*(j.get())));
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
    states[Name]->IsStart(true);
    ///TODO throw an exceptions if there is already a start state.
}

void Automaton::AddFinalState(int Name)
{
    states[Name]->IsFinal(true);
}

int Automaton::GetStartState()
{
    State::Ptr s = states[startState];
    if(s->IsStart())
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
        if(f->second->IsFinal())
            finals.push_back(f->second->Name());
    }
    if(finals.empty())
    {
        ///TODO throw empty list

    } 
    return finals;
}


Automaton::Ptr Automaton::operator~() const
{
    Automaton::Ptr b = Automaton::construct(self);
    for(map<int, State::Ptr>::const_iterator f = b->states.begin();
            f != b->states.end();
            ++f)
    {
        if(f->second->IsFinal())
            f->second->IsFinal(false);
        else
            f->second->IsFinal(true);
    }

    return b;
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
    if(states[currentState]->IsFinal())
    {
        return true;
    }
    else
    {
        return false;
    }
}

Automaton::Automaton(Automaton const & p)
{
    startState = p.startState;
    for(map<int, State::Ptr>::const_iterator i = p.states.begin();
            i != p.states.end();
            ++i)
    {
        states[i->first] = State::construct(i->second->Name());
        states[i->first]->IsStart(i->second->IsStart());
        states[i->first]->IsFinal(i->second->IsFinal());
        states[i->first]->transitions = i->second->transitions;
    }
}

///////////////// State Class ////////////////////////////

Automaton::State::Ptr  Automaton::State::construct(int Name)
{
    Automaton::State::Ptr c(new Automaton::State(Name));
    c->self = c;
    return c;
}

Automaton::State::State(int Name):
    isStart(false),
    isFinal(false),
    name(Name)
{
}

Automaton::State::~State()
{
}


bool  Automaton::State::IsFinal()
{
    return isFinal;
}

void  Automaton::State::IsFinal(bool b)
{
    isFinal = b;
}

bool  Automaton::State::IsStart()
{
    return isStart;
}

void  Automaton::State::IsStart(bool b)
{
    isStart = b;
}

int  Automaton::State::Name()
{
    return name;
}

void  Automaton::State::Name(int n)
{
    name = n;
}

