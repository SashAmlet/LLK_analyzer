#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_map>

#include "grammar.h"
#include "transition.h"
#include "helper_class.h"
#include "sequence_analyzer.h"

using namespace std;

int main() {
    ifstream inputFile("grammar1.txt");
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
        /*case 3: {
            stringstream steam(line);
            while (steam >> ws) {
                char token;
                if (steam >> token) {
                    resultingSequence.push_back(token);
                }
            }
        } break;*/
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

    // filling first_k and follow_k
    unordered_map<char, vector<vector<char>>> firstK, followK;
    cout << "First " << static_definitions::getK() << endl;
    firstK = inputGrammar->first(static_definitions::getK());
    inputGrammar->setFirst_k(firstK);

    for (char NT : inputGrammar->getNonTerminals()) {

        cout << NT << " : ";
        for (vector<char> Ts : firstK[NT]) {
            for (char T : Ts) {
                cout << T;
            }
            cout << ' ';
        }
        cout << endl;
    }
    cout << "Follow " << static_definitions::getK() << endl;
    for (char NT : inputGrammar->getNonTerminals()) {
        followK[NT] = inputGrammar->follow(static_definitions::getK(), NT);

        cout << NT << " : ";
        for (vector<char> Ts : followK[NT]) {
            for (char T : Ts) {
                cout << T;
            }
            cout << ' ';
        }
        cout << endl;
    }
    inputGrammar->setFollow_k(followK);
    cout << "--------------------" << endl;

    auto* table = new helper_class(firstK, followK, *inputGrammar);
    table->printMainTable();

    cout << "--------------------" << endl;

    vector<char> resultingSequence = vector<char>({ '(', 'a', '+', 'a',')','*', 'a' }); // grammar 1
    //vector<char> resultingSequence = vector<char>({ 'i','+','n','*','i','+','(',')','!', }); // grammar 2
    //vector<char> resultingSequence = vector<char>({ '(','*','!','n','!','*',')', }); //grammar 2
    //vector<char> resultingSequence = vector<char>({ '(','a','+','a',')', }); // grammar 3
    auto* analyzer = new sequence_analyzer(Axiom, resultingSequence, table->getMainTable(), *inputGrammar);
    vector<size_t> result = analyzer->analyzeSequence();
    for (auto elem : result)
        cout << elem << endl;
   
    return 0;
}