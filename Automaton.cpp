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

int Automaton::GetStartState() const
{
    map<int, State::Ptr>::const_iterator s = states.find(startState);
    if((s->second)->IsStart())
    {
        return (s->second)->Name(); 
    }
    else
    {
        ///TODO throw no start state defined
    }
}

list<int> Automaton::GetFinalStates() const
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

bool Automaton::Run(Sequence input) const
{
    int currentState = GetStartState();
    for(list<string>::const_iterator t = input.begin();
            t != input.end();
            ++t)
    {
        map<int, State::Ptr>::const_iterator s = states.find(currentState);
        if(s == states.end())
            throw std::exception();
        
        State::Ptr tempState = s->second;
        if(tempState->transitions.find(*t) == tempState->transitions.end())
        {
            return false; //There is no transition for the given input, reject
        }
        else
        {
            currentState = s->second->transitions[*t];
        }
    }
    map<int, State::Ptr>::const_iterator s = states.find(currentState);
    if(s->second->IsFinal())
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

Automaton::Ptr Automaton::opIntersect(Automaton::Ptr rhs) const 
{
    Automaton::Ptr result = Automaton::construct();
    return result;
}

Automaton::Ptr Automaton::opComplement() const
{
    return ~(*this);
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


bool  Automaton::State::IsFinal() const
{
    return isFinal;
}

void  Automaton::State::IsFinal(bool b)
{
    isFinal = b;
}

bool  Automaton::State::IsStart() const
{
    return isStart;
}

void  Automaton::State::IsStart(bool b)
{
    isStart = b;
}

int  Automaton::State::Name() const
{
    return name;
}

void  Automaton::State::Name(int n)
{
    name = n;
}

