#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

#include "grammar.h"
#include "transition.h"

using namespace std;

int main() {
    ifstream inputFile("grammar.txt");
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

    auto* inputGrammar = new grammar(Axiom, terminals, nonTerminals, transitions);
    vector<char> firstS = inputGrammar->first('S');
    vector<char> firstA = inputGrammar->first('A');

    unordered_map<char, vector<string>> first_k;
    vector<string> valuesForS = { "a", "a+", "a*", "(a", "((" };
    vector<string> valuesForA = { "e", "+a", "+("};
    vector<string> valuesForB = { "a", "a*", "(a", "((" };
    vector<string> valuesForC = { "e", "*a", "*("};
    vector<string> valuesForD = { "a", "(a", "((" };
    first_k['S'] = valuesForS;
    first_k['A'] = valuesForA;
    first_k['B'] = valuesForB;
    first_k['C'] = valuesForC;
    first_k['D'] = valuesForD;
    vector<string> followS = inputGrammar->follow_k(1, 'S', first_k);
    vector<string> followA = inputGrammar->follow_k(1, 'A', first_k);
    vector<string> followB = inputGrammar->follow_k(1, 'B', first_k);
    vector<string> followC = inputGrammar->follow_k(1, 'C', first_k);
    vector<string> followD = inputGrammar->follow_k(1, 'D', first_k);

    return 0;
}