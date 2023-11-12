//
// Created by mikhail.syvachenko on 05.11.2023.
//

#ifndef LLKANALYZER_STATIC_DEFINITIONS_H
#define LLKANALYZER_STATIC_DEFINITIONS_H

#include<vector>

using namespace std;


class static_definitions {
public:
    static bool isEpsilon(vector<char> word);
    static vector<char> getEpsilonVector();
    static char getEpsilon();
    static vector<char> noEpsilonWord(const vector<char>& word);
    static vector<char> removeFirstN(const vector<char>& word, int n);
    static int getK();
    static bool isEqualVectors(const vector<char>& vec1, const vector<char>& vec2);
};


#endif //LLKANALYZER_STATIC_DEFINITIONS_H