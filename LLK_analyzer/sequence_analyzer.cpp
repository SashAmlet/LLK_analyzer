//
// Created by mikhail.syvachenko on 05.11.2023.
//

#include "sequence_analyzer.h"

#include <utility>

sequence_analyzer::sequence_analyzer(char startAxiom, vector<char> sequenceToAnalyze, unordered_map<string, vector<size_t>> mainTable, const grammar& gr) {
    stack.push_back(startAxiom);
    sequenceToAnalyze = static_definitions::noEpsilonWord(sequenceToAnalyze);
    this->sequenceToAnalyze = sequenceToAnalyze;
    this->mainTable = std::move(mainTable);
    currentGrammar = gr;
}

vector<size_t> sequence_analyzer::analyzeSequence() {
    vector<size_t> result;
    if (stack.empty())
        return {};

    if (mainTable.empty())
        return {};

    while (!stack.empty() || !currentSubSequence.empty())
    {
        if (!move(result)) {
            result.push_back(0);
            return result;
        }
    }

    return result;
}

// Returns false if next transition is not possible
bool sequence_analyzer::move(vector<size_t>& res_vec) {
    // check if the sequence starts with some terminal elements, eliminate them one by one or throw error if they don't match. Update current subsequence with first k, remove first n processed

    int amountOfSymbolsAddedToSubSequence = 0;
    while (currentSubSequence.size() < static_definitions::getK() && !sequenceToAnalyze.empty()) {
        currentSubSequence.push_back(sequenceToAnalyze[amountOfSymbolsAddedToSubSequence]);
        amountOfSymbolsAddedToSubSequence++;
    }
    if (amountOfSymbolsAddedToSubSequence > 0) {
        sequenceToAnalyze = static_definitions::removeFirstN(sequenceToAnalyze, amountOfSymbolsAddedToSubSequence);
    }
    bool flag = 0;
    if (sequenceToAnalyze.empty() && currentSubSequence.empty()) {
        flag = 1;
        vector<size_t> epsilonRules = mainTable["e"];
        for (auto rule : epsilonRules) {
            auto it = find(stack.begin(), stack.end(), currentGrammar.getTransitions()[rule - 1].getFrom());
            if (it != stack.end())
            {
                stack.erase(it);
                res_vec.push_back(rule);
            }
        }
    }
    if (flag) {
        if (stack.empty())
            return true;
        else
            return false;
    }


    vector<char> allTerminals = currentGrammar.getTerminals();
    if (stack[0] == currentSubSequence[0])
    {
        if (find(allTerminals.begin(), allTerminals.end(), stack[0]) ==
            allTerminals.end())
            return false;

        stack = static_definitions::removeFirstN(stack, 1);
        currentSubSequence = static_definitions::removeFirstN(currentSubSequence, 1);

        return true;
    }
    if (find(allTerminals.begin(), allTerminals.end(), stack[0]) !=
        allTerminals.end())
        return false; // non-processable sequence

    // find current rules based on the subsequence we have and the leftest stack letter

    bool isRuleFound = false;
    vector<char> to;
    for (int i = static_definitions::getK(); i >= 0 && !isRuleFound; i--) {
        vector<char> currentWord;
        currentWord.reserve(i);
        for (int j = 0; j < i; j++) {
            currentWord.push_back(currentSubSequence[j]);
        }

        if (currentWord.empty())
            currentWord.push_back(static_definitions::getEpsilon());

        string key(currentWord.begin(), currentWord.end());
        vector<size_t> rules;
        if (mainTable.find(key) != mainTable.end()) {
            rules = mainTable.at(key);
            for (auto rule : rules) {
                if (currentGrammar.getTransitions()[rule - 1].getFrom() == stack[0])
                {
                    to = currentGrammar.getTransitions()[rule - 1].getTo();
                    isRuleFound = true;
                    res_vec.push_back(rule);
                    break;
                }
            }
        }
    }

    if (!isRuleFound)
        return false;

    // update stack according to the getTo for the rules

    stack = static_definitions::removeFirstN(stack, 1);
    if (!static_definitions::isEqualVectors(to, static_definitions::getEpsilonVector())) {
        to.insert(to.end(), stack.begin(), stack.end());
        stack = to;
    }
    return true;
}