// Jaden Heller

#include <iostream>
#include <string>
#include <map>
#include<unordered_map>
#include <vector>

#ifndef NFA_H
#define NFA_H

using namespace std;

class NFA{

    public:
        NFA();
        ~NFA();
        //Member variables for each of the elements in the 5-tuple:
        //(states, alphabet, start state, accept states, delta-transitions)
        vector<string> NFAstates;
        string NFAalphabet;
        string startState;
        vector<string> accStates;
        //Transitions:
        unordered_map< string, unordered_map<char, vector<string>> > stateMap; 
        //All states that have an epsilon-transion and the state they lead to following that transition
        unordered_map< string, vector<string>> epislonStates; 

        //"Set" functions for the member variables:
        void addState(string state);
        void addLetter(char letter);
        void addTransToState(string state, char letter, string tranState);
        void addEpsilonTrans(string state, string tranState);

        //"Get" functions:
        int getAlphaSize();
        string getAlphabet();
        int getNumStates();
        //Print the Nfa's tuple
        void printNFA();

    private:
        int alphaSize;
        int numStates;    
};
#endif