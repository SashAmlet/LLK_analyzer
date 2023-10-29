//
// Created by mikhail.syvachenko on 28.10.2023.
//
#pragma clang diagnostic push
#pragma ide diagnostic ignored "misc-no-recursion"
#include <algorithm>
#include <set>
#include <utility>
#include "grammar.h"
#include <string>


grammar::grammar(char axiom, vector<char> terminals, vector<char> nonTerminals, vector<transition> transitions) {
    this->axiom = axiom;
    this->terminals = std::move(terminals);
    this->nonTerminals = std::move(nonTerminals);
    this->transitions = std::move(transitions);
    this->nonTerminalAmount = int(getNonTerminals().size());
    this->terminalAmount = int(getTerminals().size());
    this->transitionsAmount = int(getTransitions().size());
}

grammar::~grammar() = default;

vector<char> grammar::getTerminals() {
    return terminals;
}

vector<char> grammar::getNonTerminals() {
    return nonTerminals;
}

vector<transition> grammar::getTransitions() {
    return transitions;
}

int grammar::getTerminalsAmount() const {
    return terminalAmount;
}

int grammar::getNonTerminalsAmount() const {
    return nonTerminalAmount;
}

int grammar::getTransitionsAmount() const {
    return transitionsAmount;
}

char grammar::getAxiom() const {
    return axiom;
}

vector<char> grammar::first(char nonTerminalElement) {
    return this->first(nonTerminalElement, {});
}

vector<char> grammar::first(char nonTerminalElement, vector<char> processedNonTerminals) {
    if (find(processedNonTerminals.begin(), processedNonTerminals.end(), nonTerminalElement) != processedNonTerminals.end())
        return {};

    vector<char> currentTerminals = getTerminals();

    if (find(currentTerminals.begin(), currentTerminals.end(), nonTerminalElement) !=
        currentTerminals.end())
        return { nonTerminalElement };

    processedNonTerminals.push_back(nonTerminalElement);

    vector<transition> transitionsFromSpecifiedNonTerminal;
    vector<char> result;

    for (int i = 0; i < getTransitionsAmount(); i++) {
        if (this->transitions[i].getFrom() == nonTerminalElement)
            transitionsFromSpecifiedNonTerminal.push_back(this->transitions[i]);
    }

    for (int i = 0; i < transitionsFromSpecifiedNonTerminal.size(); i++) {
        int epsilonTrimmedIndex = 0;
        while (transitionsFromSpecifiedNonTerminal[i].getTo()[epsilonTrimmedIndex] == getEpsilon()) {
            epsilonTrimmedIndex++;
            if (epsilonTrimmedIndex == transitionsFromSpecifiedNonTerminal[i].getTo().size()) {
                epsilonTrimmedIndex = -1;
                break;
            }
        }

        if (epsilonTrimmedIndex == -1)
        {
            result.push_back(getEpsilon());
            continue;
        }

        vector<char> subFirst = first(transitionsFromSpecifiedNonTerminal[i].getTo()[epsilonTrimmedIndex], processedNonTerminals);
        for (char terminal : subFirst)
            result.push_back(terminal);
    }

    set<char> resultDistinctSet(result.begin(), result.end());
    vector<char> resultDistinct(resultDistinctSet.begin(), resultDistinctSet.end());

    return resultDistinct;
}

bool grammar::isEpsilon(vector<char> word) {
    vector<char> epsilonVector = getEpsilonVector();
    return word.size() == epsilonVector.size() && equal(word.begin(), word.end(), epsilonVector.begin());
}

vector<char> grammar::getEpsilonVector() {
    vector<char> epsilonVector = { 'e' };
    return epsilonVector;
}

char grammar::getEpsilon() {
    return 'e';
}

// Follow k
bool grammar::linearFilling(int k, string terminals, const vector<char> afterElms, vector<string>* Follow_k, const unordered_map<char, vector<string>> *first_ks) { // fill the sequence of terminals in the current rule without transposing the rules
    vector<char> allTerminals = getTerminals();
    char eps = getEpsilon();

    for (char afterElm : afterElms) {
        // if "k" is reached
        if (terminals.length() == k) {
            Follow_k->push_back(terminals);
            return true;
        }

        // if afterElm is epsilon
        if (afterElm == eps) {
            if (terminals.empty()) {
                Follow_k->push_back("e");
            }
            else {
                Follow_k->push_back(*terminals);
            }
        }// if afterElm is terminal
        else if (find(allTerminals.begin(), allTerminals.end(), afterElm) != allTerminals.end()) {
            terminals.push_back(afterElm);
        }
        else { // if afterElm is nonterminal
            for (string el : first_ks->at(afterElm)) {
                vector<char> newAfter;
                for (char e : el) {
                    newAfter.push_back(e);
                }
                newAfter.insert(newAfter.end(), afterElms.begin() + 1, afterElms.end());
                linearFilling(k, terminals, newAfter, Follow_k, first_ks);
            }
        }
    }
}

vector<string> grammar::follow_k(int k, char nonTerminalElement, const unordered_map<char, vector<string>> first_ks) {
    vector<string> Follow_k;
    vector<transition> trans; // select all the rules in which the non-terminal we need appears on the right side
    vector<char> allTerminals = getTerminals();

    for (transition tr : getTransitions()) {
        vector<char> trRightPart = tr.getTo();
        string terminals;
        for (char el : trRightPart) {
            if (el == nonTerminalElement) {
                auto it = find(trRightPart.begin(), trRightPart.end(), el);
                if (it != trRightPart.end()) {
                    // Create a new vector and insert all the elements from 'el'.
                    vector<char> afterElms(it + 1, trRightPart.end());
                    bool res = linearFilling(k , "", afterElms, &Follow_k, &first_ks);
                    if (res) { // из этого правила, без транспонирований, спокойно вывели все возможные комбинации из k терминалов
                        break;
                    }
                    else {
                        // 2.1
                    }

                }
                else {
                    // 2.0
                }
            }
        }
    }

    return Follow_k;
}

#pragma clang diagnostic pop
