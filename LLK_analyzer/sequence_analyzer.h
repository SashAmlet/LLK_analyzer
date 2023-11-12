//
// Created by mikhail.syvachenko on 05.11.2023.
//

#ifndef LLKANALYZER_SEQUENCE_ANALYZER_H
#define LLKANALYZER_SEQUENCE_ANALYZER_H

#include<vector>
#include <string>
#include <unordered_map>
#include <utility>
#include "static_definitions.h"
#include "grammar.h"

using namespace std;

class sequence_analyzer {
public:
    sequence_analyzer(char startAxiom, vector<char> sequenceToAnalyze, unordered_map<string, vector<size_t>> mainTable, const grammar& gr);
    vector<size_t> analyzeSequence();
private:
    bool move(vector<size_t>& res_vec);
    vector<char> stack;
    // all the input row
    vector<char> sequenceToAnalyze;
    // 3 column
    vector<char> currentSubSequence;
    unordered_map<string, vector<size_t>> mainTable;
    grammar currentGrammar = grammar('0', {}, {}, {});
};


#endif //LLKANALYZER_SEQUENCE_ANALYZER_H