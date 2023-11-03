#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

#include "grammar.h"
#include "transition.h"

using namespace std;

int main() {
    ifstream inputFile("grammar2.txt");
    if (!inputFile.is_open())
    {
        cout << "Error opening file." << endl;
        return 1;
    }

    string line;
    int rowCount = -1;

    vector<char> terminals;
    vector<char> nonTerminals;
    char Axiom;

    vector<transition> transitions;

    while (getline(inputFile, line)) {
        rowCount++;
        switch (rowCount) {
        case 0: {
            stringstream stream(line);
            while (stream >> ws) {
                char token;
                if (stream >> token) {
                    terminals.push_back(token);
                }
            }
        } break;
        case 1: {
            stringstream steam(line);
            while (steam >> ws) {
                char token;
                if (steam >> token) {
                    nonTerminals.push_back(token);
                }
            }
        } break;
        case 2: Axiom = line[0];  break;
        default: {
            char from = line[0];
            vector<char> to;
            string toSequence = line.substr(5);
            for (char symbol : toSequence) {
                to.push_back(symbol);
            }
            transitions.emplace_back(from, to);
        }
        }
    }

    auto* inputGrammar = new grammar(Axiom, terminals, nonTerminals, transitions, 1);
   
    return 0;
}