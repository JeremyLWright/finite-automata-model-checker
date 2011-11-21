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

using namespace std;
class Automaton
{
public:
       
    typedef std::tr1::shared_ptr<Automaton> Ptr;
    typedef std::tr1::weak_ptr<Automaton> WeakPtr;
    static Automaton::Ptr construct();
    static Automaton::Ptr construct(Automaton::WeakPtr const copy);
    virtual ~Automaton();
    void AddState(int Name);
    void AddTransition(int FromNode, int ToNode, string label);
    void SetStartState(int Name);
    void AddFinalState(int Name);
    bool Run(list<string> input);
    int GetStartState();
    list<int> GetFinalStates();
    // Implements complement!
    Automaton::Ptr operator~() const;
    Automaton::Ptr opIntersect(Automaton::Ptr rhs) const;
    Automaton::Ptr opUnion(Automaton::Ptr rhs) const;
    
protected:
    Automaton (Automaton const & p);
    Automaton operator=(Automaton const &);
 class State
    {
    public:
        typedef std::tr1::shared_ptr<State> Ptr;
        typedef std::tr1::weak_ptr<State> WeakPtr;
        static Ptr construct(int Name);
        bool IsStart();
        void IsStart(bool);
        bool IsFinal();
        void IsFinal(bool);
        int Name();
        void Name(int);
        virtual ~State();
        map<string, int> transitions;
    private:
        bool isStart;
        bool isFinal;
        int name;
        State(int Name);
        State::WeakPtr self;
    };
    
    int startState;
    map<int, State::Ptr> states;
    
private:
    Automaton();
    Automaton::WeakPtr self;
    
};

#endif /* end of include guard: _AUTOMATON */
