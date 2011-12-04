/**
 * @brief High level class reqpresentation a finite state automaton.
 *
 * @par
 * Copyright Jeremy Wright (c) 2011
 * Creative Commons Attribution-ShareAlike 3.0 Unported License.
 */
  
#ifndef _AUTOMATON
#define _AUTOMATON

#include <map>
#include <list>
#include <tr1/memory>
#include <string>
#include <set>
#include <sstream>
#include <vector>
using namespace std;
class Automaton
{
friend class SystemLevel;
friend class NFATests;
    protected:    
class State
    {
    public:
        typedef std::tr1::shared_ptr<State> Ptr;
        typedef std::tr1::weak_ptr<State> WeakPtr;
        void AddTransition(string label, State::Ptr ToNode);
        static Ptr construct(int Name);
        static Ptr construct(int Name, set<State::Ptr> NFAStates);
        bool IsStart() const;
        void IsStart(bool);
        bool IsFinal() const;
        void IsFinal(bool);
        int Name() const;
        void Name(int);
        virtual ~State();
        set<int> GetTransitions(string) const;
        multimap<string, int> transitions;
        set<State::Ptr> NFAStates() const;
    private:
        bool isStart;
        bool isFinal;
        int name;
        set<State::Ptr> nfaStates;
        State(int Name);
        State(int Name, set<Ptr> NFAStates);
        State::WeakPtr self;
    };
     
    public:
    typedef std::tr1::shared_ptr<Automaton> Ptr;
    typedef std::tr1::weak_ptr<Automaton> WeakPtr;
    typedef list<string> Sequence; 
    static Automaton::Ptr construct();
    static Automaton::Ptr construct(Automaton::WeakPtr const copy);
    virtual ~Automaton();
    void AddState(int Name);
    void AddTransition(int FromNode, int ToNode, string label);
    void SetStartState(int Name);
    void AddFinalState(int Name);
    bool Run(Sequence input) const;
    int GetStartState() const;
    bool IsNFA() const;
    list<int> GetFinalStates() const;
    ///Returns a List of sequences accepted by the state machine.
    bool FindSequence(Sequence& acceptedSequence) const;
    bool FindPath(int start, list<int>& result) const;
    Automaton::Ptr opSubsetConversion() const;
    Automaton::Ptr opIntersect(Automaton::Ptr const rhs) const;
    Automaton::Ptr opUnion(Automaton::Ptr const rhs) const;
    Automaton::Ptr opComplement() const;
    void Move(string label, set<State::Ptr> NFAState, set<State::Ptr>& result) const;
    string const EPSILON; 
protected:
    set<string> Alphabet;
    list<int> GetAdjecentStates(int state) const;
    string FindTransitionToState(int a, int b) const;
    Automaton (Automaton const & p);
    Automaton operator=(Automaton const &);

    void AddState(State::Ptr State);
    int startState;
    map<int, State::Ptr> states;
    bool EpsilonClosure(set<State::Ptr>, set<State::Ptr>& states) const;
    bool EpsilonClosure(int state, set<State::Ptr>& closure) const;
    
private:
    Automaton();
    Automaton::WeakPtr self;
    Automaton::Ptr operator~() const;
    
};


namespace {
template <typename T>
bool from_string(T& t, const std::string& s, std::ios_base& (*f)(std::ios_base&))
{
        std::istringstream iss(s, std::istringstream::in);
        return !(iss >> f >> t).fail();
}

}

#endif /* end of include guard: _AUTOMATON */
