//Jaden Heller
#include "DFA.h"
DFA::DFA(){ //Default constructor
    DFAstates.push_back(" ");
    DFAalphabet = "";
    alphaSize = 0;
    numStates = 0;
    startState = "";
}

DFA::DFA(NFA nfa){ //Overload with nfa
    eps = "";
    NFAepsilonList = nfa.epislonStates; //Get the map of NFA states and their epsilon transitions
    DFAalphabet = nfa.getAlphabet();
    alphaSize = nfa.getAlphaSize();
    startState = findStartState(nfa.startState, NFAepsilonList);
    NFAacceptStates = nfa.accStates;
    NFAstateMap = nfa.stateMap;
}


void DFA::epsilonClosure(string state, stateToStatesMap_t epsStateList){
    if(epsStateList.find(state) != epsStateList.end()){
        for(int i = 0; i < epsStateList[state].size(); i++){
            string currState = epsStateList[state][i];
            //Recursivly check the epsilon closure of the state that each epsilon-transition leads to
            epsilonClosure(currState, epsStateList);
            //Keep track of every state that can be reached by epsilon starting from "state"
            epsStatesVisited += currState; 
        }
    }
    else{ //Base-case: when the state passed as "state" isn't in the list of epsilon transitions 
        return;
    }
}
//Find the DFA start state based-off the NFA start state and it's epsilon closures (stored in the NFAepsilonList)
string DFA::findStartState(string NFAstart, stateToStatesMap_t epsStateList){ 
    epsilonClosure(NFAstart, epsStateList);
    return NFAstart + epsStatesVisited;
}
//Find all states in the DFA that include the DFA's accept state(s)
vector<string> DFA::findAcceptStates(vector<string> NFAaccStates){ 
    vector<string> temp_acceptStates;
    string temp_individualAccState;

    for(int i = 0; i < NFAaccStates.size(); i++){
        temp_individualAccState = NFAaccStates[i];
        for(int p = 0; p < stateKeysList.size(); p++){
            //If the NFA accept state can be found in the DFA
            if(stateKeysList[p].find(temp_individualAccState) != string::npos){ 
                //Add those DFA states to the list of DFA accept states
                temp_acceptStates.push_back(stateKeysList[p]); 
            }
        }
    }
    return temp_acceptStates;
}
//Get the states (values) that are mapped to from the state-letter (key:key) pair
string DFA::getDestinationStates(string inputState, char letter){
    //Holds the individual states in a union state (ex: {2} and {3} in {2,3}) 
    string componentState = ""; 
    string outputState;
    vector<string> NFAdestStates;
    int outputStateNum;
    int nfaStateNum;
    string nfaDestString = "";

    eps = "";
    for(int c = 0; c < inputState.size(); c++){
        componentState += inputState[c]; //Make the chars into strings

        if(NFAstateMap.find(componentState) != NFAstateMap.end()){
            NFAdestStates = NFAstateMap[componentState][letter];
            for(int s = 0; s < NFAdestStates.size(); s++){

                if(NFAepsilonList.find(NFAdestStates[s]) != NFAepsilonList.end()){
                    for(int w = 0; w < NFAepsilonList[NFAdestStates[s]].size(); w++){
                        eps += NFAepsilonList[NFAdestStates[s]][w];
                    }
                }
                outputState += NFAdestStates[s];

                nfaDestString = "";
            }
        }
        //Reset componentState
        componentState = ""; 
    }
    return outputState;
}
//Construct the DFA table:
void DFA::buildDFAtable(string firstState){ 
    //Add the first state to the vector "queue" of states to be added to the list
    stateKeysList.push_back(firstState); 
    string currKeyState;

    string destState;
    for(int s = 0; s < stateKeysList.size(); s++){
        for(int a = 0; a < alphaSize; a++){
            //Check the vector associated with the state-letter pair to see which sate(s) they lead to.
            destState = getDestinationStates(stateKeysList[s], DFAalphabet[a]); 
            if(destState == ""){
                destState = "EM";
            }
            else{
                for(int i = 0; i < eps.size(); i++){
                    if(destState.find(eps[i]) == string::npos){
                        //Epsilon transition added onto the states as necessary
                        destState += eps[i]; 
                    }
                }
            }
            //Sort the states in ascending order, prevents redundancies when the same state has different names (ex: 123 and 231)
            sort(destState.begin(), destState.end());  
            addTransToState(stateKeysList[s], DFAalphabet[a], destState);
            //If the destState isn't already a key in the map, add it to the vector get in line to be added
            if( find(stateKeysList.begin(), stateKeysList.end(), destState) == stateKeysList.end()){      
                stateKeysList.push_back(destState);
            }
        }
    }
}

void DFA::addTransToState(string state, char letter, string tranState){
    
    //Vector to hold the state-vector associated with state-letter pairs
    vector<string> transStateVect; 
    if(DFAstateMap.find(state) != DFAstateMap.end()){
        if(DFAstateMap[state].find(letter) != DFAstateMap[state].end()){
            transStateVect = DFAstateMap[state][letter];
            if(find(transStateVect.begin(), transStateVect.end(), tranState) == transStateVect.end()){
                DFAstateMap[state][letter][0] = tranState;
            }
        }
        else{ //state doesn't have a map associated with letter
            vector<string> transitionStates;
            transitionStates.push_back(tranState);
            DFAstateMap[state].insert({letter, transitionStates});
        }
    }
    else{
        vector<string> transitionStates;
        transitionStates.push_back(tranState);
        letterToStatesMap_t transMap;
        transMap.insert({letter, transitionStates});
        DFAstateMap.insert({state, transMap});
    }
}

void DFA::printDFA(string fileName){
    //output DFA file has same name as the nfa but different extention
    string outFile = fileName + ".DFA"; 

    ofstream out(outFile);
    //save the old buffer (cout ouput to terminal)
    streambuf *coutbuf = cout.rdbuf();
    //change cout to output to filename.DFA 
    cout.rdbuf(out.rdbuf()); 
    //States:
    for(int p = 0; p < stateKeysList.size(); p++){
        cout << "{";
        for(int i = 0; i < stateKeysList[p].size(); i++){
            cout << stateKeysList[p][i];
            if(i != stateKeysList[p].size() - 1 && stateKeysList[p] != "EM"){
                cout << ", ";
            }
        }
        cout << "}  ";
    }
    cout << endl;
    //Alphabet:
    for(int a = 0; a < DFAalphabet.size(); a++){
        cout << DFAalphabet[a] << "   ";
    }
    cout << endl;
    //Start State:
    cout << "{";
    for(int s = 0; s < startState.size(); s++){
        cout << startState[s];
        if(s != startState.size() - 1){
            cout << ", ";
        }
    }
    cout << "}" << endl;
    //Accept States:
    accStates = findAcceptStates(NFAacceptStates);
    for(int a = 0; a < accStates.size(); a++){
        cout << "{";
        for(int p = 0; p < accStates[a].size(); p++){
            cout << accStates[a][p];
            if(p != accStates[a].size() -1){
                cout << ", ";
            }
        }
        cout << "}  ";
    }
    cout << endl;
    //Transitions:
    for(int i = 0; i < stateKeysList.size(); i++){
        if(DFAstateMap.find(stateKeysList[i]) != DFAstateMap.end()){
            for(int j = 0; j < alphaSize; j++){

                cout << "{";
                for (int a = 0; a < stateKeysList[i].size(); a++){
                    cout << stateKeysList[i][a];
                    if (a != stateKeysList[i].size() - 1 && stateKeysList[i] != "EM"){
                        cout << ", ";
                    }
                }
                cout << "}, ";

                if(DFAstateMap[stateKeysList[i]].find(DFAalphabet[j]) != DFAstateMap[stateKeysList[i]].end()){
                    cout << DFAalphabet[j] << " = {";
                    
                    for(int y = 0; y < DFAstateMap[stateKeysList[i]][DFAalphabet[j]][0].size(); y++){
                        cout << DFAstateMap[stateKeysList[i]][DFAalphabet[j]][0][y];
                        if(y != DFAstateMap[stateKeysList[i]][DFAalphabet[j]][0].size() - 1 && DFAstateMap[stateKeysList[i]][DFAalphabet[j]][0] != "EM"){
                            cout << ", ";
                        }
                    }

                    cout << "}" << endl;
                }
                else{
                    cout << DFAalphabet[j] << " = EM, ";
                }
            }
        }
    }
    //Reset to the original output (that was saved above)
    cout.rdbuf(coutbuf); 
}