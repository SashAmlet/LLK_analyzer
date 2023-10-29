//
// Created by mikhail.syvachenko on 28.10.2023.
//

#ifndef LLKANALYZER_GRAMMAR_H
#define LLKANALYZER_GRAMMAR_H
#include <utility>
#include <vector>
#include "transition.h"
#include <unordered_map>
using namespace std;

class grammar {
public:
    grammar(char axiom, vector<char> terminals, vector<char> nonTerminals, vector<transition> transitions);

    ~grammar();

    [[nodiscard]] char getAxiom() const; // Axiom is start non-terminals
    [[nodiscard]] int getTerminalsAmount() const;

    [[nodiscard]] int getNonTerminalsAmount() const;

    [[nodiscard]] int getTransitionsAmount() const;

    vector<char> getTerminals();

    vector<char> getNonTerminals();

    vector<transition> getTransitions();

    // TODO: Rewrite with First K
    vector<char> first(char nonTerminalElement);
    // Follow k // using unordered_map<char, vector<string>> first_k
    vector<string> follow_k(int k, char nonTerminalElement, const unordered_map<char, vector<string>> first_ks);

private:
    int terminalAmount;
    int nonTerminalAmount;
    int transitionsAmount;
    char axiom;
    vector<char> nonTerminals;
    vector<char> terminals;
    vector<transition> transitions;

    vector<char> first(char nonTerminalElement, vector<char> processedNonTerminals);
    // Follow k
    bool linearFilling(int k, string terminals, const vector<char> afterElms, vector<string>* Follow_k, const unordered_map<char, vector<string>>* first_ks);

    static bool isEpsilon(vector<char> word);
    static vector<char> getEpsilonVector();
    static char getEpsilon();
};

#endif //LLKANALYZER_GRAMMAR_H
