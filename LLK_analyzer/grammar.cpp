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
#include <iostream>


grammar::grammar(char axiom, vector<char> terminals, vector<char> nonTerminals, vector<transition> transitions, int k) {
    this->axiom = axiom;
    this->terminals = std::move(terminals);
    this->nonTerminals = std::move(nonTerminals);
    this->transitions = std::move(transitions);
    this->nonTerminalAmount = int(getNonTerminals().size());
    this->terminalAmount = int(getTerminals().size());
    this->transitionsAmount = int(getTransitions().size());

    // filling first_k and follow_k

    cout << "First " << k << endl;
    this->first_k = first(k);

    for (char NT : this->nonTerminals) {

        cout << NT << " : ";
        for (vector<char> Ts : this->first_k[NT]) {
            for (char T : Ts) {
                cout << T;
            }
            cout << ' ';
        }
        cout << endl;
    }
    cout << "Follow " << k << endl;
    for (char NT : this->nonTerminals) {
        this->follow_k[NT] = follow(k, NT);

        cout << NT << " : ";
        for (vector<char> Ts: this->follow_k[NT]) {
            for (char T : Ts) {
                cout << T;
            }
            cout << ' ';
        }
        cout << endl;
    }
}

#pragma region Helpers

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
#pragma endregion

#pragma region First k
bool grammar::isTerm(vector<char> to) {
    for (char el : to) {
        if (find(nonTerminals.begin(), nonTerminals.end(), el) != nonTerminals.end()) {
            return false;
        }
    }
    return true;
}

vector<vector<char>> grammar::concatenation(int k, vector<vector<char>> a, vector<vector<char>> b) {
    vector<vector<char>> result;
    for (vector<char> termA : a) {
        for (vector<char> termB : b) {
            vector<char> conc;
            if (isEpsilon(termA)) {
                result.push_back(termB);
            }
            if (isEpsilon(termB)) {
                result.push_back(termA);
            }
            else {
                conc.insert(conc.end(), termA.begin(), termA.end());
                conc.insert(conc.end(), termB.begin(), termB.end());
                conc.resize(k);
                result.push_back(conc);
            }
        }
    }
    return result;
}
vector<vector<char>> grammar::iteration(int k, vector<char> To, const unordered_map<char, vector<vector<char>>>& first_k) {
    vector<vector<char>> result;

    if (isTerm({ To[0] })) {
        result = { { To[0] } };
    }
    else {
        result = first_k.at(To[0]);
    }

    for (int ii = 1; ii < To.size(); ii++) {
        vector<vector<char>> next;
        if (isTerm({ To[ii] })) {
            next = { { To[ii] } };
        }
        else {
            next = first_k.at(To[ii]);
        }
        result = concatenation(k, result, next);
    }

    return result;
}

unordered_map<char, vector<vector<char>>> grammar::first(int k) {
    unordered_map<char, vector<vector<char>>> first_k;
    // first iteration
    for (char NT : nonTerminals) {
        vector<vector<char>> vecTerms;
        for (transition tr : transitions) {
            vector<char> terms;
            if (NT == tr.getFrom() && (isTerm(tr.getTo()) || isEpsilon(tr.getTo()))) {
                for (int i = 0; i < tr.getTo().size() && i < k; i++) {
                    terms.push_back(tr.getTo()[i]);
                }
            }
            if (!terms.empty())
                vecTerms.push_back(terms);
        }
        first_k[NT] = vecTerms;
    }

    // k+1 iteration
    for (int i = 0; i < 20; i++) {
        for (char NT : nonTerminals) {
            for (transition tr : transitions) {
                if (tr.getFrom() == NT) {
                    vector<vector<char>> newterms = iteration(k, tr.getTo(), first_k);
                    vector<vector<char>> oldterms = first_k[NT];
                    oldterms.insert(oldterms.end(), newterms.begin(), newterms.end());
                    first_k[NT] = oldterms;

                }
            }
            // Sort the vector so that duplicates are nearby
            sort(first_k[NT].begin(), first_k[NT].end());

            // Removing duplicates
            first_k[NT].erase(unique(first_k[NT].begin(), first_k[NT].end()), first_k[NT].end());
        }
    }

    return first_k;
}
#pragma endregion

#pragma region Follow k
void grammar::RuleComposition(int k, vector<char> terminals, vector<vector<char>>* Follow_k, const transition* curtrans, vector<transition>* usedtrans) {
    for (transition tr : getTransitions()) {
        vector<char> trRightPart = tr.getTo();
        for (char el : trRightPart) {
            if (el == curtrans->getFrom()) {
                auto it = find(trRightPart.begin(), trRightPart.end(), el);
                if (it != trRightPart.end()) {
                    // Create a new vector and insert all the elements from 'el'.
                    vector<char> afterElms(it + 1, trRightPart.end());
                    bool res = linearFilling(k, terminals, afterElms, Follow_k, &tr);
                    if (res) { //From this rule, without any composition, we easily derived all possible combinations of k terminals
                        terminals.clear();
                        break;
                    }
                    else {
                        // 2.1
                        if (tr.getFrom() == getAxiom()) {
                            Follow_k->push_back({'e'});
                        }
                        if ((usedtrans == NULL) ){
                            usedtrans = new vector<transition>();
                            usedtrans->push_back(tr);
                            RuleComposition(k, terminals, Follow_k, &tr, usedtrans);
                        }
                        else if (find(usedtrans->begin(), usedtrans->end(), tr) == usedtrans->end()) {
                            usedtrans->push_back(tr);
                            RuleComposition(k, terminals, Follow_k, &tr, usedtrans);
                        }
                    }

                }
                else {
                    // 2.2
                }
            }
        }
    }
}

bool grammar::linearFilling(int k, vector<char>& terminals, const vector<char> afterElms, vector<vector<char>>* Follow_k, const transition* curtrans) { // fill the sequence of terminals in the current rule without transposing the rules
    vector<char> allTerminals = getTerminals();
    char eps = getEpsilon();
    int ii = 1;
    for (char afterElm : afterElms) {
        // if "k" is reached
        if (terminals.size() == k) {
            Follow_k->push_back(terminals);
            return true;
        }

        // if afterElm is epsilon
        if (afterElm == eps) {
            if (terminals.empty()) {
                Follow_k->push_back({'e'});
                RuleComposition(k, terminals, Follow_k, curtrans, {});
            }
        }// if afterElm is terminal
        else if (find(allTerminals.begin(), allTerminals.end(), afterElm) != allTerminals.end()) {
            terminals.push_back(afterElm);
            ii++;
        }
        else { // if afterElm is nonterminal
            for (vector<char> el : first_k.at(afterElm)) {
                vector<char> newAfter = el;
                vector<char> copyTerminals(terminals);
                newAfter.insert(newAfter.end(), afterElms.begin() + ii, afterElms.end());
                bool res = linearFilling(k, terminals, newAfter, Follow_k, curtrans);
                /*if (res) {
                    break;
                }*/
                terminals = copyTerminals;
            }
        }
    }
    Follow_k->push_back(terminals);
    return terminals.size() == k? true:false;
}

vector<vector<char>> grammar::follow(int k, char nonTerminalElement) {
    vector<vector<char>> Follow_k;
    vector<transition> trans; // select all the rules in which the non-terminal we need appears on the right side
    vector<char> allTerminals = getTerminals();

    for (transition tr : getTransitions()) {
        vector<char> trRightPart = tr.getTo();
        vector<char> terminals;
        for (char el : trRightPart) {
            if (el == nonTerminalElement) {
                auto it = find(trRightPart.begin(), trRightPart.end(), el);
                if (it != trRightPart.end()) {
                    // Create a new vector and insert all the elements from 'el'.
                    vector<char> afterElms(it + 1, trRightPart.end());
                    bool res = linearFilling(k , terminals, afterElms, &Follow_k, &tr);
                    if (res) { //From this rule, without any composition, we easily derived all possible combinations of k terminals
                        terminals.clear();
                        break;
                    }
                    else {
                        if (tr.getFrom() == getAxiom()) {
                            Follow_k.push_back({ 'e' });
                        }
                        RuleComposition(k, terminals, &Follow_k, &tr, {});
                    }

                }
                else {
                    // 2.0
                }
            }
        }
    }

    if (nonTerminalElement == getAxiom()) {
        Follow_k.push_back({'e'});
    }

    // Sort the vector so that duplicates are nearby
    std::sort(Follow_k.begin(), Follow_k.end());

    // Removing duplicates
    Follow_k.erase(std::unique(Follow_k.begin(), Follow_k.end()), Follow_k.end());


    return Follow_k;
}
#pragma endregion

#pragma clang diagnostic pop
