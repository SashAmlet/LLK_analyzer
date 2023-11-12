//
// Created by mikhail.syvachenko on 05.11.2023.
//

#include <vector>
#include <string>
#include <tuple>
#include <iostream>
#include <unordered_map>
#include "grammar.h"
using namespace std;

class helper_class {
public:
    helper_class(const unordered_map<char, vector<string>>& firsts, const unordered_map<char, vector<string>>& follows,
        const grammar& gr);

    helper_class(unordered_map<char, vector<vector<char>>> firstChars, unordered_map<char, vector<vector<char>>> followChars,
        grammar gr);

    unordered_map<size_t, vector<string>> getHelperTable();

    unordered_map<string, vector<size_t>> getMainTable();

    void printMainTable();

private:
    // Creates relation number-transition
    void createTransitionRelation();
    transition getTransitionFromRelation(size_t num);
    /*
     * This function will be executed only in constructors, so to get helper table, all we need to do is to call
     * constructor.
     * Here I used epsilon as e, we can also use here method internalGrammar.getEpsilon(), but it is private in
     * original class.
     * Table consists of number of transition(because we cannot hash transition as it is) and it's value like in table
     * NOTE: originally, this function returns unordered_map in reversed order. We need to reverse it in the end.
     */
    void createHelperTable();
    bool isTerminal(char ch);
    // There fields are for creating a helper table.
    /*
     * Creates the last table for our llk analyzer.
     * Returns dictionary: sequence of terminals -> vector of numbers of transitions (for each terminal here we have
     * some transitions)
     */
    void createMainTable();

    unordered_map<char, vector<string>> firstK;
    unordered_map<char, vector<string>> followK;
    vector<tuple<size_t, transition>> numberTransitionRelation;
    grammar internalGrammar = grammar('0', {}, {}, {});
    unordered_map<size_t, vector<string>> helperTable;
    unordered_map<string, vector<size_t>> mainTable;
};