//Jaden Heller
#include "NFA.h"

NFA::NFA(){ //Default vals
    NFAalphabet = "";
    alphaSize = 0;
    numStates = 0;
    startState = "";
}


NFA::~NFA(){
}

void NFA::addState(string state){
    NFAstates.push_back(state);
    numStates = NFAstates.size();
}

void NFA::addLetter(char letter){
    NFAalphabet += letter;
    alphaSize = NFAalphabet.size();
}
//Add a destination-state to a state given on of the letters from the alphabet
void NFA::addTransToState(string state, char letter, string tranState){ 
    /*A simple diagram of stateMap's structure:
    stateMap -> ( {state : transMap} )    //Aka key : val
    transMap -> ( {letter : vector_x<char>} )
    vector_x -> ( transState(s) ) */
    if(stateMap.find(state) != stateMap.end()){ //state is already a key in the map somewhere
        if(stateMap[state].find(letter) != stateMap[state].end()){ //...and the state-letter pair is also in the map
            //Add the destination state to the vector of states
            stateMap[state][letter].push_back(tranState); 
        }
        else{ //state doesn't have a map associated with letter...
            vector<string> transitionStates;
            transitionStates.push_back(tranState);
            stateMap[state].insert({letter, transitionStates});
        }
    }
    else{ //The state is not yet in the map anywhere...
        vector<string> transitionStates;
        transitionStates.push_back(tranState);
        unordered_map<char, vector<string>> transMap;
        transMap.insert({letter, transitionStates});
        stateMap.insert({state, transMap});
    }
}
//Add the State-destinationState pairs that can be reached through epsilon transitions
void NFA::addEpsilonTrans(string state, string tranState){ 
    if(epislonStates.find(state) != epislonStates.end()){
        epislonStates[state].push_back(tranState);
    }
    else{
        vector<string> transitionStates;
        transitionStates.push_back(tranState);
        epislonStates.insert({state, transitionStates});
    }
}
int NFA::getAlphaSize(){
    return alphaSize;
}

string NFA::getAlphabet(){
    return NFAalphabet;
}

int NFA::getNumStates(){
    return numStates;
}

void NFA::printNFA(){

    cout << "States: ";
    for(int p = 0; p < NFAstates.size(); p++){
        cout << NFAstates[p];
    }
    cout << endl;
    cout << "Alphabet: " << NFAalphabet << endl;
    cout << "Start State: " << startState << endl;
    cout << "Accept States: ";
    for(int i = 0; i < accStates.size(); i++){
        cout << accStates[i];
    }
    cout << endl;
    cout << "Transitions: " << endl;
    for(int i = 0; i < numStates; i++){
        if(stateMap.find(NFAstates[i]) != stateMap.end()){
            cout << "(State " << NFAstates[i] << ") ";
            for(int j = 0; j < alphaSize; j++){
                if(stateMap[NFAstates[i]].find(NFAalphabet[j]) != stateMap[NFAstates[i]].end()){
                    cout << NFAalphabet[j] << " = ";
                    for(int s = 0; s < stateMap[NFAstates[i]][NFAalphabet[j]].size(); s++){
                        cout << stateMap[NFAstates[i]][NFAalphabet[j]][s] << ", ";
                    }
                    cout << "   ";
                }
                else{
                    cout << NFAalphabet[j] << " = EM, ";
                }
            }
            cout << endl;
        }
        else{
            cout << "(State " << NFAstates[i] << ") ";
            for(int j = 0; j < alphaSize; j++){
                cout << NFAalphabet[j] << " = EM, " << endl;
            }
        }
    } 
}
