#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
using namespace std;

class Grammar
{

public:
    string startSymbol;
    map<string, vector<string>> grammar;
    set<string> terminals;
    set<string> nonTerminals;

    Grammar(string filename)
    {
        ifstream fin(filename);
        if (!fin)
        {
            cout << "Error opening file" << endl;
            return;
        }
        else
        {
            string line;
            while (getline(fin, line))
            {
                if (startSymbol == "")
                {
                    startSymbol = line.substr(0, line.find("->"));
                }
                string key = line.substr(0, line.find("->"));
                string value = line.substr(line.find("->") + 2, line.length());
                grammar[key].push_back(value);
            }
        }
    }

    string getStartSymbol()
    {
        return startSymbol;
    }

    map<string, vector<string>> getGrammarMap()
    {
        return grammar;
    }

    void printMapInJSONFormat()
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
    map<string, vector<string>> getAugmentedGrammarMap()
    {
        map<string, vector<string>> augmentedGrammar = grammar;
        augmentedGrammar[startSymbol + "'"].push_back(startSymbol);
        return augmentedGrammar;
    }

    void printGrammar()
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

    void printAugmentedGrammar()
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

    // Terminals e.g. are (, ), id, +, *, etc. They might be within one of the value of every key's array in the map.
    // Function to identify terminals mentioned above
    void findTerminals()
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

    void findNonTerminals()
    {
        for (auto it = grammar.begin(); it != grammar.end(); it++)
        {
            nonTerminals.insert(it->first);
        }
    }

    void printTerminals()
    {
        for (auto it = terminals.begin(); it != terminals.end(); it++)
            cout << *it << " ";
        cout << endl;
    }

    void printNonTerminals()
    {
        for (auto it = nonTerminals.begin(); it != nonTerminals.end(); it++)
            cout << *it << " ";
        cout << endl;
    }

    // Terminals e.g. are (, ), id, +, *, etc.
    // Function to identify terminals mentioned above
};

int main()
{
    // ifstream fin("input.txt");

    // map<string, vector<string>> grammar;
    // string startSymbol;

    // if (!fin)
    // {
    //     cout << "Error opening file" << endl;
    //     return 1;
    // }
    // else
    // {
    //     string line;
    //     while (getline(fin, line))
    //     {
    //         if (startSymbol == "")
    //         {
    //             startSymbol = line.substr(0, line.find("->"));
    //             grammar[startSymbol + "\b'"].push_back(startSymbol);
    //         }
    //         string key = line.substr(0, line.find("->"));
    //         string value = line.substr(line.find("->") + 2, line.length());
    //         grammar[key].push_back(value);
    //     }
    // }

    // for (auto it = grammar.begin(); it != grammar.end(); it++)
    // {
    //     cout << it->first << " -> ";
    //     for (auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
    //     {
    //         if (it2 != it->second.end() - 1)
    //             cout << *it2 << " | ";
    //         else
    //             cout << *it2;
    //     }
    //     cout << endl;
    // }

    Grammar grammar("input.txt");

    // Testing all the functions
    cout << "printGrammar()" << endl;
    grammar.printGrammar();
    cout << endl;

    cout << "printAugmentedGrammar()" << endl;
    grammar.printAugmentedGrammar();
    cout << endl;

    cout << "printMapInJSONFormat()" << endl;
    grammar.printMapInJSONFormat();
    cout << endl;

    cout << "printTerminals()" << endl;
    grammar.findTerminals();
    grammar.printTerminals();
    cout << endl;

    cout << "printNonTerminals()" << endl;
    grammar.findNonTerminals();
    grammar.printNonTerminals();
    cout << endl;

    return 0;
}
