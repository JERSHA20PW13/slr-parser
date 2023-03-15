#ifndef GRAMMAR_HPP
#define GRAMMAR_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

class Grammar
{
private:
    /* data */
    string startSymbol;
    map<string, vector<string>> grammar;
    set<string> terminals;
    set<string> nonTerminals;
    map<string, set<string>> first;
    map<string, set<string>> follow;

public:
    Grammar();
    Grammar(string filename);
    string getStartSymbol();
    string getAugmentedStartSymbol();
    map<string, vector<string>> getGrammarMap();
    void printMapInJSONFormat();
    map<string, vector<string>> getAugmentedGrammarMap();
    map<string, set<string>> getFirst();
    map<string, set<string>> getFollow();
    void printGrammar();
    void printAugmentedGrammar();
    void findTerminals();
    void findNonTerminals();
    void printTerminals();
    void printNonTerminals();
    void constructFirst();
    void constructFollow();
    // ~Grammar();
};

Grammar::Grammar()
{
}

Grammar::Grammar(string filename)
{
    ifstream fin(filename);
    if (!fin)
    {
        cout << "Error opening file!" << endl;
        return;
    }
    else
    {
        string line;
        while (getline(fin, line))
        {
            if (startSymbol == "")
                startSymbol = line.substr(0, line.find("->"));
            string key = line.substr(0, line.find("->"));
            string value = line.substr(line.find("->") + 2, line.length());
            grammar[key].push_back(value);
        }
        constructFirst();
        constructFollow();
    }
}

string Grammar::getStartSymbol()
{
    return startSymbol;
}

string Grammar::getAugmentedStartSymbol()
{
    return startSymbol + "\'";
}

map<string, vector<string>> Grammar::getGrammarMap()
{
    return grammar;
}

void Grammar::printMapInJSONFormat()
{
    cout << "{" << endl;
    for (auto it = grammar.begin(); it != grammar.end(); it++)
    {
        cout << "\t\"" << it->first << "\": [";
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
            if (it2 != it->second.end() - 1)
                cout << "\"" << *it2 << "\", ";
            else
                cout << "\"" << *it2 << "\"";
        }
        cout << "]," << endl;
    }
    cout << "}" << endl;
}

// Getting augmented grammar
map<string, vector<string>> Grammar::getAugmentedGrammarMap()
{
    map<string, vector<string>> augmentedGrammar = grammar;
    augmentedGrammar[startSymbol + "'"].push_back(startSymbol);
    return augmentedGrammar;
}

void Grammar::printGrammar()
{
    for (auto it = grammar.begin(); it != grammar.end(); it++)
    {
        cout << it->first << " -> ";
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
            if (it2 != it->second.end() - 1)
                cout << *it2 << " | ";
            else
                cout << *it2;
        }
        cout << endl;
    }
}

void Grammar::printAugmentedGrammar()
{
    map<string, vector<string>> augmentedGrammar = getAugmentedGrammarMap();
    for (auto it = augmentedGrammar.begin(); it != augmentedGrammar.end(); it++)
    {
        cout << it->first << " -> ";
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
            if (it2 != it->second.end() - 1)
                cout << *it2 << " | ";
            else
                cout << *it2;
        }
        cout << endl;
    }
}

// Doesn't work
void Grammar::findTerminals()
{
    for (auto it = grammar.begin(); it != grammar.end(); it++)
    {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
            string value = *it2;
            for (int i = 0; i < value.length(); i++)
            {
                if (value[i] >= 'a' && value[i] <= 'z')
                {
                    string terminal = "";
                    terminal += value[i];
                    terminals.insert(terminal);
                }
            }
        }
    }
}

void Grammar::findNonTerminals()
{
    for (auto it = grammar.begin(); it != grammar.end(); it++)
    {
        nonTerminals.insert(it->first);
    }
}

void Grammar::printTerminals()
{
    for (auto it = terminals.begin(); it != terminals.end(); it++)
        cout << *it << " ";
    cout << endl;
}

void Grammar::printNonTerminals()
{
    for (auto it = nonTerminals.begin(); it != nonTerminals.end(); it++)
        cout << *it << " ";
    cout << endl;
}

void Grammar::constructFirst()
{
    for (auto it = grammar.begin(); it != grammar.end(); it++)
    {
        for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
            string value = *it2;
            if (!(value[0] >= 'A' && value[0] <= 'Z'))
            {
                string terminal = "";
                terminal += value[0];
                first[it->first].insert(terminal);
            }
        }
    }

    int counter = grammar.size();
    while(counter--) {
        for (auto it = grammar.begin(); it != grammar.end(); it++)
        {
            for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
            {
                string value = *it2;
                if (value[0] >= 'A' && value[0] <= 'Z')
                {
                    string nonTerminal = "";
                    nonTerminal += value[0];

                    for (auto it3 = first[nonTerminal].begin(); it3 != first[nonTerminal].end(); it3++)
                    {
                        first[it->first].insert(*it3);
                    }
                }
            }
        }
    }
}

map<string, set<string>> Grammar::getFirst()
{
    return first;
}

void Grammar::constructFollow()
{
    follow[startSymbol].insert("$");

    int counter = grammar.size();
    while(counter--) {
        for (auto it = grammar.begin(); it != grammar.end(); it++)
        {
            for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
            {
                string value = *it2;
                for (int i = 0; i < value.length(); i++)
                {
                    if (value[i] >= 'A' && value[i] <= 'Z')
                    {
                        string nonTerminal = "";
                        nonTerminal += value[i];

                        if (i == value.length() - 1)
                        {
                            for (auto it3 = follow[it->first].begin(); it3 != follow[it->first].end(); it3++)
                            {
                                follow[nonTerminal].insert(*it3);
                            }
                        }
                        else
                        {
                            if (value[i + 1] >= 'A' && value[i + 1] <= 'Z')
                            {
                                string nextNonTerminal = "";
                                nextNonTerminal += value[i + 1];

                                for (auto it3 = first[nextNonTerminal].begin(); it3 != first[nextNonTerminal].end(); it3++)
                                {
                                    follow[nonTerminal].insert(*it3);
                                }
                            }
                            else
                            {
                                string terminal = "";
                                terminal += value[i + 1];
                                follow[nonTerminal].insert(terminal);
                            }
                        }
                    }
                }
            }
        }
    }
}

map<string, set<string>> Grammar::getFollow()
{
    return follow;
}

#endif