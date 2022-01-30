// Jaden Heller
#include "NFA.h"
#include <algorithm>
#include <fstream>

#ifndef DFA_H
#define DFA_H

class DFA{

    public:
        DFA();
        //Extract needed variables from the given NFA
        DFA(NFA nfa); 
        //Member variables for each of the elements in the 5-tuple:
        //(states, alphabet, start state, accept states, delta-transitions)
        vector<string> DFAstates;
        string DFAalphabet;
        string startState;
        vector<string> accStates;
        //Transitions:
        typedef unordered_map<string, vector<string>> stateToStatesMap_t;
        typedef unordered_map<char, vector<string>> letterToStatesMap_t;

        //Created transitions
        void addTransToState(string state, char letter, string tranState);

        string findStartState(string NFAstart, stateToStatesMap_t epsStateList);

        vector<string> findAcceptStates(vector<string> NFAaccStates);
        //Return the state(s) that a given state leads to on given letter of the dfa alphabet
        string getDestinationStates(string inputState, char letter);
        //Construct a dfa transition map of the format {state : { letter : destination states}}
        void buildDFAtable(string firstState);
        //Print the dfa's tuple
        void printDFA(string fileName);


    private:
        int alphaSize;
        int numStates;
        //DFA transition map
        unordered_map<string, letterToStatesMap_t> DFAstateMap;
        //transition map from the given NFA
        unordered_map<string, letterToStatesMap_t> NFAstateMap;
        //Adds to the list of states that a given state "visits" along its path of epsilon-transitions
        void epsilonClosure(string state, stateToStatesMap_t epsStateList);
        string epsStatesVisited;
        //A "queue"-like vector that holds all the sates that need to be added to the DFA table
        vector<string> stateKeysList; 
        //List of states that have an epsilon transition from them in the NFA
        stateToStatesMap_t NFAepsilonList;
        //Temporarily holds each individual epsilon transition as they're added to the epsilon list
        string eps;
        //All accepts states in the NFA
        vector<string> NFAacceptStates;
};

#endif