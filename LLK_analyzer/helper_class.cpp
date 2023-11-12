//
// Created by mikhail.syvachenko on 05.11.2023.
//

#include "helper_class.h"

helper_class::helper_class(const unordered_map<char, vector<string>>& firsts,
    const unordered_map<char, vector<string>>& follows, const grammar& gr) {
    firstK = firsts;
    followK = follows;
    internalGrammar = gr;
    createTransitionRelation();
    createHelperTable();
    createMainTable();
}

helper_class::helper_class(unordered_map<char, vector<vector<char>>> firstChars,
    unordered_map<char, vector<vector<char>>> followChars, grammar gr) {
    unordered_map<char, vector<string>> first, follow;

    for (char nTerminal : gr.getNonTerminals()) {
        vector<string> tempVector;

        for (const auto& str : firstChars[nTerminal])
            tempVector.emplace_back(str.data(), str.size());
        if (!tempVector.empty())
            first[nTerminal] = tempVector;
        tempVector.clear();

        for (const auto& str : followChars[nTerminal])
            tempVector.emplace_back(str.data(), str.size());
        if (!tempVector.empty())
            follow[nTerminal] = tempVector;
    }

    firstK = first;
    followK = follow;
    internalGrammar = gr;
    createTransitionRelation();
    createHelperTable();
    createMainTable();
}

unordered_map<size_t, vector<string>> helper_class::getHelperTable() {
    return helperTable;
}

unordered_map<string, vector<size_t>> helper_class::getMainTable() {
    return mainTable;
}

void helper_class::printMainTable() {
    /*for (size_t i = 1; i <= internalGrammar.getTransitionsAmount(); ++i)
        for (const auto& elem : helperTable[i]) {
            cout << elem << " ";
            for (const auto& numToPrint : mainTable[elem])
                cout << numToPrint << " ";
            cout << endl;
        }*/
    for (auto XYU: mainTable) {
        cout << XYU.first << "   ";
        for (auto xui : XYU.second) {
            cout << xui << " ";
        }
        cout << endl;
    }
}

void helper_class::createTransitionRelation() {
    size_t i = 0;
    for (const transition& tr : internalGrammar.getTransitions()) {
        numberTransitionRelation.emplace_back(i, tr);
        i++;
    }
}

transition helper_class::getTransitionFromRelation(size_t num) {
    for (auto tupleRel : numberTransitionRelation) {
        if (get<0>(tupleRel) == num - 1) return get<1>(tupleRel);
    }
    throw exception();
}

void helper_class::createHelperTable() {
    unordered_map<size_t, vector<string>> tempTable;
    for (size_t i = 1; i <= numberTransitionRelation.size(); ++i) {
        auto tr = getTransitionFromRelation(i);
        char firstChar = tr.getTo()[0];

        if (firstChar == 'e') tempTable.insert({ i, followK[tr.getFrom()] });
        else {
            if (isTerminal(firstChar)) {
                string tempStr;
                vector<string> tempVec = { tempStr + firstChar };
                tempTable.insert({ i, tempVec });
            }
            else {
                tempTable.insert({ i, firstK[firstChar] });
            }
        }
    }
    helperTable = tempTable;
}

bool helper_class::isTerminal(char ch) {
    for (char terminal : internalGrammar.getTerminals())
        if (ch == terminal) return true;
    return false;
}

void helper_class::createMainTable() {
    unordered_map<string, vector<size_t>> tempTable;
    for (size_t i = 1; i <= internalGrammar.getTransitionsAmount(); ++i)
        for (const auto& elem : helperTable[i])
            tempTable[elem].push_back(i);
    mainTable = tempTable;
}