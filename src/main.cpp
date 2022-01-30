// Jaden Heller

#include "NFA.h"
#include "DFA.h"
#include <iostream>
#include <fstream>
#include <ctype.h>

//Main handles general execution as well as file stream operations
int main(int argc, char **argv){

    NFA nfa;

    //File I/O variables:
    string fileName;
    fstream readFile;
    string lineAsString;
    string outFile;

    cout << "Enter NFA file name: " << endl;
    cin >> fileName;

    for(int i = 0; i < fileName.size() - 4; i++){
        outFile += fileName[i];
    }

    readFile.open(fileName, ios::in);
    if (!readFile.is_open()){
        cout << "Couldnt open file!" << endl;
    }
    else{
        cout << "Opened!" << endl;
        int fileLine = 1;
        while(getline(readFile, lineAsString)){ //Read each line of the input file:
            int openBrackCount = 0;
            int commaCount = 0;
            string state = "";
            char letter = ' ';
            string tranState = "";
            int epsCheck = 0; //if == 3 then it's an EPS
            string charAsString = ""; //All the following for parsing the info out of the file
            for(int i = 0; i < lineAsString.size(); i++){
                if(fileLine == 1){ //the "list of states" line of the file
                    if(lineAsString[i] != '{' && lineAsString[i] != '}' && !isspace(lineAsString[i])){  
                        charAsString += lineAsString[i];
                        nfa.addState(charAsString);
                        charAsString = "";
                    }
                }
                else if(fileLine == 2){//List of alphabet
                    if(!isspace(lineAsString[i])){
                        nfa.addLetter(lineAsString[i]);
                    }
                }
                else if(fileLine == 3 || fileLine == 4){//Starting state, accept state
                    if(lineAsString[i] != '{' && lineAsString[i] != '}' && !isspace(lineAsString[i])){
                        if(fileLine == 3){
                            nfa.startState = lineAsString[i];
                        }
                        else{ //fileLine == 4
                        charAsString += lineAsString[i];
                        nfa.accStates.push_back(charAsString);
                        charAsString = "";
                        }
                        
                    }
                }
                else{
                    if(lineAsString[i] == '{'){
                        openBrackCount++;
                    }
                    else if(lineAsString[i] == ','){
                        commaCount++;
                    }

                    else if(lineAsString[i] != '}' && lineAsString[i] != '=' && !isspace(lineAsString[i])){
                        if(openBrackCount == 1 && commaCount == 0){
                            state = lineAsString[i];
                        }
                        else if(openBrackCount == 1 && commaCount == 1){
                            letter = lineAsString[i];
                            
                            if (lineAsString[i] == 'E'){ //Checking if the letters "E-P-S" are in a row
                                epsCheck++;
                            }
                            else if(lineAsString[i] == 'P' && epsCheck == 1){
                                epsCheck++;
                            }
                            else if(lineAsString[i] == 'S' && epsCheck == 2){
                                epsCheck++;
                            }
                        }
                        else{
                            tranState = lineAsString[i];
                            if(epsCheck != 3){
                                nfa.addTransToState(state, letter, tranState);
                            }
                            else{
                                nfa.addEpsilonTrans(state, tranState);
                            }
                        }
                    }
                }
            }
            fileLine++;
        }
        readFile.close();
    }

    DFA dfa(nfa); //Create a dfa outline based off of the nfa

    dfa.buildDFAtable(dfa.startState); //Construct the dfa table
    dfa.printDFA(outFile); //Print out to the .dfa file

    return 0;
}