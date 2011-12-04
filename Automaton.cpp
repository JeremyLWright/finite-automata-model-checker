#include "Automaton.h"
#include <stack>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include <iostream>

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

Automaton::Automaton():
    EPSILON("epsilon")
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

void Automaton::AddState(State::Ptr State)
{
    if(states.find(State->Name()) == states.end())
    {
        states[State->Name()] = State;
    }
    else
    {
        throw State;
    }
}

void Automaton::AddTransition(int FromNode, int ToNode, string label)
{
    State::Ptr workingState = states[FromNode];
    workingState->transitions.insert( pair<string, int>(label, ToNode));
    if(label != EPSILON)
        Alphabet.insert(label);
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
    throw std::exception();
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
    if(IsNFA())
    {
        throw logic_error("Cannot execute an NFA, please convert.");
    }
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
            currentState = s->second->transitions.find(*t)->second;
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

list<int> Automaton::GetAdjecentStates(int state) const
{
    list<int> adjecentStates;
    map<int, State::Ptr>::const_iterator s = states.find(state);

    for(multimap<string, int>::const_iterator i = s->second->transitions.begin();
            i != s->second->transitions.end();
            ++i)
    {
        adjecentStates.push_back(i->second);
    }
    return adjecentStates;
}

string Automaton::FindTransitionToState(int a, int b) const
{
    map<int, State::Ptr>::const_iterator s = states.find(a);
    for(multimap<string, int>::const_iterator r = s->second->transitions.begin();
            r != s->second->transitions.end();
            ++r)
    {
        if(r->second == b)
            return r->first;
    }
    return "";
}
// Reference: http://en.literateprograms.org/Depth-first_search_(C_Plus_Plus)
bool Automaton::FindPath(int start, list<int>& result) const
{
    if(find(result.begin(), result.end(), start) != result.end())
        return false;

    result.push_back(start);

    if(states.find(start)->second->IsFinal())
        return true;

    list<int> adj = GetAdjecentStates(start);
    for(list<int>::const_iterator itr = adj.begin();
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
    list<int> acceptedPath;
    if(FindPath(startState, acceptedPath))
    {
        list<int>::const_iterator nextState = acceptedPath.begin();
        ++nextState; //Advance to the next state so its alwas ahead of currentState
        for(list<int>::const_iterator currentState = acceptedPath.begin();
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
    //a n b == !(!a u !b)
    return (
    (self.lock()->opComplement())->
        opUnion(
    rhs->opComplement())
    )->opComplement();
}

Automaton::Ptr Automaton::opUnion(Automaton::Ptr const rhs) const
{
    Automaton::Ptr u = Automaton::construct();
    int const magicNumber = 9999;
    for(map<int, State::Ptr>::const_iterator i = states.begin();
            i != states.end();
            ++i)
    {
        u->AddState(i->second->Name());
        u->states[i->second->Name()]->IsFinal(i->second->IsFinal());
        u->states[i->second->Name()]->transitions = i->second->transitions;
    }
    
    for(map<int, State::Ptr>::const_iterator i = rhs->states.begin();
            i != rhs->states.end();
            ++i)
    {
        u->AddState(i->second->Name());
        u->states[i->second->Name()]->IsFinal(i->second->IsFinal());
        u->states[i->second->Name()]->transitions = i->second->transitions;
    }

    u->AddState(magicNumber);
    u->AddTransition(magicNumber, rhs->GetStartState(), "epislon");
    u->AddTransition(magicNumber, self.lock()->GetStartState(), "epislon");
    u->SetStartState(magicNumber);
    //Run NFA to DFA on u
    return u;
}

Automaton::Ptr Automaton::opComplement() const
{
    return ~(*this);
}

bool Automaton::IsNFA() const
{
    for(map<int, State::Ptr>::const_iterator stateItr = states.begin();
            stateItr != states.end();
            ++stateItr)
    {
        if(stateItr->second->transitions.count(EPSILON) != 0)
            return true;
    }
    return false;
}

bool Automaton::EpsilonClosure(int startState, set<int>& states) const
{
    set<State::Ptr> intermediate;
    bool v = EpsilonClosure(startState, intermediate);
    for(set<State::Ptr>::const_iterator itr = intermediate.begin();
            itr != intermediate.end();
            ++itr)
    {
        states.insert((*itr)->Name());
    }
    return v;
}

bool Automaton::EpsilonClosure(set<State::Ptr> startSet, set<State::Ptr>& closure) const
{
    bool value = true;
    for(set<State::Ptr>::const_iterator setItr = startSet.begin();
            setItr != startSet.end();
            ++setItr)
    {
        if(EpsilonClosure((*setItr)->Name(), closure) == false)
            value = false;
    }
    return value;
}

bool Automaton::EpsilonClosure(int state, set<State::Ptr>& closure) const
{
    if(find(closure.begin(), closure.end(), states.at(state)) != closure.end())
        return false;


    //Goal is no more epsilon transitions
    State::Ptr workingState = states.find(state)->second;
    closure.insert(workingState);
    set<int> epsilons = workingState->GetTransitions(EPSILON);
    if(epsilons.size() == 0)
        return true;

    set<State::Ptr> sepsilons;
    for(set<int>::const_iterator i = epsilons.begin();
            i != epsilons.end();
            ++i)
    {
        sepsilons.insert(states.at(*i));
    }
    
    for(set<State::Ptr>::const_iterator itr = sepsilons.begin();
            itr != sepsilons.end();
            ++itr)
    {
        if(EpsilonClosure((*itr)->Name(), closure) == false)
        {
            closure.erase(workingState);
        }
    }

    return true;   
}
void Automaton::PrettyPrint(vector<State::Ptr> const & table) const
{
    cout << "DFA Converstion Table:" << endl;
    for(vector<State::Ptr>::const_iterator stateItr = table.begin();
            stateItr != table.end();
            ++stateItr)
    {
        cout << "Name: " << (*stateItr)->Name() << endl;
        for(multimap<string, int>::const_iterator tranItr = (*stateItr)->transitions.begin();
                tranItr != (*stateItr)->transitions.end();
                ++tranItr)
        {
            cout << "\t" << "T: " << tranItr->first << " : " << tranItr->second << endl;
        }
    }
}

Automaton::Ptr Automaton::opSubsetConversion() const
{
    typedef int StateName;
    typedef string TransitionName;
    int id;
    State::Ptr DFAStartState = states.at(startState);
    set<State::Ptr> NFAStartStateSet;
    NFAStartStateSet.insert(DFAStartState);
    
    set<State::Ptr> DFAStartStateSet;
    EpsilonClosure(NFAStartStateSet, DFAStartStateSet);

    vector<State::Ptr> DFATable;
    DFATable.push_back(DFAStartState);

    vector<State::Ptr> unvisitedStates;
    unvisitedStates.push_back(DFAStartState);

    while(!unvisitedStates.empty())
    {
        State::Ptr CurDFAState = unvisitedStates[unvisitedStates.size()-1];
        unvisitedStates.pop_back();

        for(set<string>::const_iterator itr = Alphabet.begin();
                itr != Alphabet.end();
                ++itr)
        {
            bool found = false;
            set<State::Ptr> moveRes;
            set<State::Ptr> epsilonClosureRes;
            Move(*itr, CurDFAState->NFAStates(), moveRes);
            EpsilonClosure(moveRes, epsilonClosureRes);


            set<State::Ptr>::const_iterator moveResItr;
            set<State::Ptr>::const_iterator epsilonCloResItr;

            State::Ptr s ;
            for(vector<State::Ptr>::const_iterator i = DFATable.begin(); 
                    i != DFATable.end();
                    ++i)
            {
                s = *i;
                if(s->NFAStates() == epsilonClosureRes)
                {
                    found = true;
                    break;
                }
            }

            if(!found)
            {
                State::Ptr U = State::construct(id++, epsilonClosureRes);
                unvisitedStates.push_back(U);
                DFATable.push_back(U);
                CurDFAState->AddTransition(*itr, U);
            }
            else
            {
                CurDFAState->AddTransition(*itr, s);
            }


        }
    }
    
    PrettyPrint(DFATable);
    return Automaton::construct(); 
}

#if 0
int Automaton::ConvertStateName(list<int> stateList) const
{
    stringstream ss;
    for(list<int>::const_iterator i = stateList.begin();
            i != stateList.end();
            ++i)
    {
        ss << *i;
    }
    string s = ss.str();
    int value;
    from_string<int>(value, s, std::dec);
    return value;
}
#endif
void Automaton::Move(string label, set<State::Ptr> NFAState, set<State::Ptr>& result) const
{
    for(set<State::Ptr>::const_iterator itr = NFAState.begin();
            itr != NFAState.end();
            ++itr)
    {
        set<int> trans = (*itr)->GetTransitions(label);
        for(set<int>::const_iterator jtr = trans.begin();
                jtr != trans.end();
                ++jtr)
        {
            result.insert(states.at(*jtr));
        }
    }
}

///////////////// State Class ////////////////////////////

Automaton::State::Ptr  Automaton::State::construct(int Name)
{
    Automaton::State::Ptr c(new Automaton::State(Name));
    c->self = c;
    return c;
}

Automaton::State::Ptr Automaton::State::construct(int Name, set<State::Ptr> NFAStates)
{
    Automaton::State::Ptr c(new Automaton::State(Name, NFAStates));
    c->self = c;
    return c;
}

set<int> Automaton::State::GetTransitions(string label) const
{
    set<int> trans;
    for(multimap<string, int>::const_iterator it = transitions.equal_range(label).first;
            it != transitions.equal_range(label).second;
            ++it)
        trans.insert(it->second);

    return trans;
}

Automaton::State::State(int Name):
    isStart(false),
    isFinal(false),
    name(Name)
{
}
void Automaton::State::AddTransition(string label, State::Ptr ToNode)
{
    State::Ptr workingState = self.lock();
    workingState->transitions.insert( pair<string, int>(label, ToNode->Name()));
}

Automaton::State::State(int Name, set<State::Ptr> NFAStates):
    isStart(false),
    isFinal(false),
    name(Name),
    nfaStates(NFAStates)
{
    for(set<State::Ptr>::const_iterator itr = NFAStates.begin();
            itr != NFAStates.end();
            ++itr)
    {
        if((*itr)->IsFinal())
        {
            IsFinal(true);
        } 
    }
}

set<Automaton::State::Ptr> Automaton::State::NFAStates() const
{
    return nfaStates;
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

