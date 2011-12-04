#include "Automaton.h"
#include <stack>
#include <algorithm>
#include <sstream>

namespace {
    Automaton::StateName_t IntToStateName(int Name)
    {
        stringstream ss;
        ss<< Name;
        return ss.str();
    }
}

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

void Automaton::AddState(StateName_t Name)
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

void Automaton::AddState(int Name)
{
    AddState(IntToStateName(Name));
}

void Automaton::AddTransition(int FromNode, int ToNode, Label_t label)
{
    AddTransition(IntToStateName(FromNode), IntToStateName(ToNode), label);
}

void Automaton::AddTransition(StateName_t FromNode, StateName_t ToNode, string label)
{
    states[FromNode]->transitions[label] = ToNode;
    Alphabet.insert(label);
}

void Automaton::SetStartState(StateName_t Name)
{
    startState = Name;
    states[Name]->IsStart(true);
    ///TODO throw an exceptions if there is already a start state.
}

void Automaton::SetStartState(int Name)
{
    SetStartState(IntToStateName(Name));
}

void Automaton::AddFinalState(StateName_t Name)
{
    states[Name]->IsFinal(true);
}

void Automaton::AddFinalState(int Name)
{
    AddFinalState(IntToStateName(Name));
}


Automaton::StateName_t Automaton::GetStartState() const
{
    map<StateName_t, State::Ptr>::const_iterator s = states.find(startState);
    if((s->second)->IsStart())
    {
        return (s->second)->Name(); 
    }
    throw std::exception();
}

list<Automaton::StateName_t> Automaton::GetFinalStates() const
{
    list<StateName_t> finals;
    for(map<StateName_t, State::Ptr>::const_iterator f = states.begin();
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
    for(map<StateName_t, State::Ptr>::const_iterator f = b->states.begin();
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
    StateName_t currentState = GetStartState();
    for(list<string>::const_iterator t = input.begin();
            t != input.end();
            ++t)
    {
        map<StateName_t, State::Ptr>::const_iterator s = states.find(currentState);
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
    map<StateName_t, State::Ptr>::const_iterator s = states.find(currentState);
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
    for(map<StateName_t, State::Ptr>::const_iterator i = p.states.begin();
            i != p.states.end();
            ++i)
    {
        states[i->first] = State::construct(i->second->Name());
        states[i->first]->IsStart(i->second->IsStart());
        states[i->first]->IsFinal(i->second->IsFinal());
        states[i->first]->transitions = i->second->transitions;
    }
}

list<Automaton::StateName_t> Automaton::GetAdjecentStates(StateName_t state) const
{
    list<StateName_t> adjecentStates;
    map<StateName_t, State::Ptr>::const_iterator s = states.find(state);

    for(map<string, StateName_t>::const_iterator i = s->second->transitions.begin();
            i != s->second->transitions.end();
            ++i)
    {
        adjecentStates.push_back(i->second);
    }
    return adjecentStates;
}

string Automaton::FindTransitionToState(StateName_t a, StateName_t b) const
{
    map<StateName_t, State::Ptr>::const_iterator s = states.find(a);
    for(map<string, StateName_t>::const_iterator r = s->second->transitions.begin();
            r != s->second->transitions.end();
            ++r)
    {
        if(r->second == b)
            return r->first;
    }
    return "";
}
// Reference: http://en.literateprograms.org/Depth-first_search_(C_Plus_Plus)
bool Automaton::FindPath(StateName_t start, list<StateName_t>& result) const
{
    if(find(result.begin(), result.end(), start) != result.end())
        return false;

    result.push_back(start);

    if(states.find(start)->second->IsFinal())
        return true;

    list<StateName_t> adj = GetAdjecentStates(start);
    for(list<StateName_t>::const_iterator itr = adj.begin();
            itr != adj.end();
            ++itr)
    {
        if(FindPath(*itr, result))
            return true;
    }

    result.pop_back();
    return false;
}

bool Automaton::FindSequence(Sequence& acceptedSequence) const 
{
    list<StateName_t> acceptedPath;
    if(FindPath(startState, acceptedPath))
    {
        list<StateName_t>::const_iterator nextState = acceptedPath.begin();
        ++nextState; //Advance to the next state so its alwas ahead of currentState
        for(list<StateName_t>::const_iterator currentState = acceptedPath.begin();
                nextState != acceptedPath.end();
                ++currentState, ++nextState)
        {
            acceptedSequence.push_back(FindTransitionToState(*currentState, *nextState));
        }
        return true;
    }
    
    return false;
}

Automaton::Ptr Automaton::opIntersect(Automaton::Ptr const rhs) const 
{
    Automaton::Ptr result = Automaton::construct();

    Automaton::Ptr lhs = self.lock();
    //Generate the list of States
    list<StateName_t> leftStates;
    list<StateName_t> rightStates;
    for(map<StateName_t, State::Ptr>::const_iterator lItr = lhs->states.begin();
            lItr != lhs->states.end();
            ++lItr)
    {
        leftStates.push_back(lItr->first);
    }
    
    for(map<StateName_t, State::Ptr>::const_iterator rItr = rhs->states.begin();
            rItr != rhs->states.end();
            ++rItr)
    {
        rightStates.push_back(rItr->first);
    }

    map<pair<StateName_t, StateName_t>, State::Ptr> StateName_tersectStates;
    for(list<StateName_t>::const_iterator rItr = rightStates.begin();
            rItr != rightStates.end();
            ++rItr)
    {

        for(list<StateName_t>::const_iterator lItr = leftStates.begin();
                lItr != leftStates.end();
                ++lItr)
        {
            pair<StateName_t, StateName_t> combinedState(*lItr, *rItr);
            StateName_t CombinedName = CreateCombinedStatesName(*lItr, *rItr);

            result->AddState(CombinedName);
            StateName_tersectStates[combinedState] = result->states.at(CombinedName); 
            StateName_tersectStates.at(combinedState)->IsFinal( rhs->states.at(*rItr)->IsFinal() && lhs->states.at(*lItr)->IsFinal() ); //Accept iff both states are accepting
            StateName_tersectStates.at(combinedState)->IsStart( rhs->states.at(*rItr)->IsStart() && lhs->states.at(*lItr)->IsStart() ); //Accept iff both states are Starting
        }

    }
    
    //Simulate both DFAs in parallel...
    for(set<string>::const_iterator alphabetItr = Alphabet.begin();
           alphabetItr != Alphabet.end();
          ++alphabetItr)
    {
        for(map<pair<StateName_t, StateName_t>, State::Ptr>::const_iterator combStatesItr = StateName_tersectStates.begin();
                combStatesItr != StateName_tersectStates.end();
                ++combStatesItr)
        {
            StateName_t lhsDestination = lhs->states.at(combStatesItr->first.first)->Move(*alphabetItr);
            StateName_t rhsDestination = rhs->states.at(combStatesItr->first.second)->Move(*alphabetItr);
            
            StateName_t CombinedName = CreateCombinedStatesName(combStatesItr->first.first, combStatesItr->first.second);
            StateName_t DestCombinedName = CreateCombinedStatesName(lhsDestination, rhsDestination);
          
            result->AddTransition(CombinedName, DestCombinedName, *alphabetItr);
            
        }
    }

    return result;

}

Automaton::StateName_t Automaton::CreateCombinedStatesName(StateName_t a, StateName_t b) const
{
    stringstream ss;
    ss << a << "/" << b;
    return ss.str();
}

Automaton::Ptr Automaton::opComplement() const
{
    return ~(*this);
}

///////////////// State Class ////////////////////////////

Automaton::State::Ptr  Automaton::State::construct(int Name)
{
    Automaton::State::Ptr c(new Automaton::State(IntToStateName(Name)));
    c->self = c;
    return c;
}

Automaton::State::Ptr  Automaton::State::construct(string Name)
{
    Automaton::State::Ptr c(new Automaton::State(Name));
    c->self = c;
    return c;
}

Automaton::State::State(StateName_t Name):
    isStart(false),
    isFinal(false),
    name(Name)
{
}

Automaton::State::~State()
{
}

//From current State, what is the follow state.
Automaton::StateName_t Automaton::State::Move(StateName_t label) const
{
    return transitions.at(label);
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

Automaton::StateName_t  Automaton::State::Name() const
{
    return name;
}

void  Automaton::State::Name(StateName_t n)
{
    name = n;
}

