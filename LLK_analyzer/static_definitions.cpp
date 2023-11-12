//
// Created by mikhail.syvachenko on 05.11.2023.
//

#include "static_definitions.h"

bool static_definitions::isEpsilon(vector<char> word) {
    vector<char> epsilonVector = getEpsilonVector();
    return word.size() == epsilonVector.size() && equal(word.begin(), word.end(), epsilonVector.begin());
}

vector<char> static_definitions::getEpsilonVector() {
    return { 'e' };
}

char static_definitions::getEpsilon() {
    return 'e';
}

vector<char> static_definitions::noEpsilonWord(const vector<char>& word) {
    vector<char> result;

    for (auto letter : word) {
        if (letter != static_definitions::getEpsilon())
            result.push_back(letter);
    }

    return result;
}

vector<char> static_definitions::removeFirstN(const vector<char>& word, int n) {
    vector<char> result;

    for (int i = n; i < word.size(); i++) {
        result.push_back(word[i]);
    }

    return result;
}

int static_definitions::getK() {
    return 1;
}

bool static_definitions::isEqualVectors(const vector<char>& vec1, const vector<char>& vec2) {
    if (vec1.size() != vec2.size())
        return false;
    for (int i = 0; i < vec1.size(); i++) {
        if (vec1[i] != vec2[i])
            return false;
    }

    return true;
}