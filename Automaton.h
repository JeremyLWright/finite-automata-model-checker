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
using namespace std;
class Automaton
{
public:
    typedef string StateName_t;
    typedef string Label_t;
    typedef std::tr1::shared_ptr<Automaton> Ptr;
    typedef std::tr1::weak_ptr<Automaton> WeakPtr;
    typedef list<string> Sequence; 
    static Automaton::Ptr construct();
    static Automaton::Ptr construct(Automaton::WeakPtr const copy);
    virtual ~Automaton();
    void AddState(StateName_t Name);
    void AddState(int Name);
    void AddTransition(StateName_t FromNode, StateName_t ToNode, Label_t label);
    void AddTransition(int FromNode, int ToNode, Label_t label);
    void SetStartState(int Name);
    void AddFinalState(int Name);
    void SetStartState(StateName_t Name);
    void AddFinalState(StateName_t Name);
    void PrettyPrint() const;
    bool Run(Sequence input) const;
    StateName_t GetStartState() const;
    list<StateName_t> GetFinalStates() const;
    ///Returns a List of sequences accepted by the state machine.
    bool FindSequence(Sequence& acceptedSequence) const;
    bool FindPath(StateName_t start, list<StateName_t>& result) const;
    // Implements complement!
    Automaton::Ptr opIntersect(Automaton::Ptr const rhs) const;
    Automaton::Ptr opComplement() const;
protected:
    set<string> Alphabet;
    StateName_t CreateCombinedStatesName(StateName_t, StateName_t) const;
    list<StateName_t> GetAdjecentStates(StateName_t state) const;
    string FindTransitionToState(StateName_t a, StateName_t b) const;
    Automaton (Automaton const & p);
    Automaton operator=(Automaton const &);
 class State
    {
    public:
        typedef std::tr1::shared_ptr<State> Ptr;
        typedef std::tr1::weak_ptr<State> WeakPtr;
        static Ptr construct(int Name);
        static Ptr construct(StateName_t Name);
        bool IsStart() const;
        void IsStart(bool);
        bool IsFinal() const;
        void IsFinal(bool);
        StateName_t Name() const;
        void Name(string);
        StateName_t Move(string label) const;
        virtual ~State();
        map<StateName_t, string> transitions;
    private:
        bool isStart;
        bool isFinal;
        StateName_t name;
        State(StateName_t Name);
        State::WeakPtr self;
    };
    
    map<StateName_t, State::Ptr> states;
    
private:
    Automaton();
    Automaton::WeakPtr self;
    Automaton::Ptr operator~() const;
    
};

#endif /* end of include guard: _AUTOMATON */
