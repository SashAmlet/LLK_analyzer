//
// Created by mikhail.syvachenko on 28.10.2023.
//

#ifndef LLKANALYZER_GRAMMAR_H
#define LLKANALYZER_GRAMMAR_H
#include <utility>
#include <vector>
#include "transition.h"
#include <unordered_map>
#include <string>
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

    void setFirst_k(unordered_map<char, vector<vector<char>>>);
    unordered_map<char, vector<vector<char>>> first(int k);

    void setFollow_k(unordered_map<char, vector<vector<char>>>);
    vector<vector<char>> follow(int k, char nonTerminalElement);

private:
    // vars
    int terminalAmount;
    int nonTerminalAmount;
    int transitionsAmount;
    char axiom;
    vector<char> nonTerminals;
    vector<char> terminals;
    vector<transition> transitions;

    unordered_map<char, vector<vector<char>>> first_k;
    unordered_map<char, vector<vector<char>>> follow_k;

    // methods

    // first
    bool isTerm(vector<char> to);
    vector<vector<char>> concatenation(int k, vector<vector<char>> a, vector<vector<char>> b);
    vector<vector<char>> iteration(int k, vector<char> To, const unordered_map<char, vector<vector<char>>>& first_k);

    // follow
    bool linearFilling(int k, vector<char>& terminals, const vector<char> afterElms, vector<vector<char>>* Follow_k, const transition* curtrans);
    void RuleComposition(int k, vector<char> terminals, vector<vector<char>>* Follow_k, const transition* curtrans, vector<transition>* usedtrans);
    // else
    static bool isEpsilon(vector<char> word);
    static vector<char> getEpsilonVector();
    static char getEpsilon();
};

#endif //LLKANALYZER_GRAMMAR_H
