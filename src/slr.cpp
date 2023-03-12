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

    Grammar() {}

    Grammar(string filename)
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
        }
    }

    string getStartSymbol()
    {
        return startSymbol;
    }

    string getAugmentedStartSymbol()
    {
        return startSymbol + "\'";
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
};

class LR0Item {
public:
    string left;
    string right;
    int dot;

    bool operator==(const LR0Item& other) const {
        return left == other.left && right == other.right && dot == other.dot;
    }

    bool operator<(const LR0Item& other) const {
        return right < other.right;
    }

    LR0Item() {}

    LR0Item(string left, string right, int dot) {
        this->left = left;
        this->right = right;
        this->dot = dot;
    }
};

set<LR0Item> closure(set<LR0Item> items, Grammar &G) {
    map<string, vector<string>> grammar = G.getGrammarMap();
    set<LR0Item> closure_items = items;

    while (true) {
        bool added = false;

        for (const auto& item : closure_items) {
            if (item.dot >= item.right.size()) {
                continue;
            }

            char next_ch = item.right[item.dot];
            string next;
            next += next_ch;

            for(const auto& production : grammar[next]) {
                LR0Item new_item = LR0Item(next, production, 0);
                bool flag = true;

                for(const auto& item : closure_items) {
                    if (item == new_item) {
                        flag = false;
                        break;
                    }
                }

                if(flag) {
                    added = true;
                    closure_items.insert(new_item);
                }
            }
        }
        if (!added)
            break;
    }
    return closure_items;
}

set<LR0Item> goto_set(set<LR0Item> items, string symbol, Grammar &G) {
    set<LR0Item> goto_items;
    map<string, vector<string>> grammar = G.getGrammarMap();

    for (const auto& item : items) {
        if (item.dot >= item.right.size()) {
            continue;
        }

        char next_ch = item.right[item.dot];
        string next;
        next += next_ch;

        if (next == symbol) {
            LR0Item new_item = LR0Item(item.left, item.right, item.dot + 1);
            goto_items.insert(new_item);
        }
    }

    return closure(goto_items, G);
}

void display(set<LR0Item> &items)
{
    for (const auto& item : items) {
        cout << item.left << " -> ";
        for (int i = 0; i < item.right.size(); i++) {
            if (i == item.dot) {
                cout << ".";
            }
            cout << item.right[i];
        }
        if (item.dot == item.right.size()) {
            cout << ".";
        }
        cout << endl;
    }
}

int main()
{
    Grammar G("input.txt");
    map<string, vector<string>> grammar = G.getAugmentedGrammarMap();

    LR0Item item = LR0Item(G.getAugmentedStartSymbol(), grammar[G.getAugmentedStartSymbol()][0], 0);
    set<LR0Item> items;
    items.insert(item);

    set<LR0Item> closure_items = closure(items, G);

    cout << "\nCLOSURE" << endl;
    display(closure_items);

    int state = 0;
    vector<set<LR0Item>> states;
    states.push_back(closure_items);

    for(const auto& s : states) {
        for(const auto& item : s) {
            if(item.dot >= item.right.size()) {
                if(item.left == G.getAugmentedStartSymbol() && item.right == grammar[G.getAugmentedStartSymbol()][0] && item.dot == 1) {
                    cout << "ACCEPT" << endl;
                }
                else {
                    cout << "REDUCE" << endl;
                }
            }
            else {
                char next_ch = item.right[item.dot];
                string next;
                next += next_ch;

                if(grammar.find(next) != grammar.end()) {
                    set<LR0Item> goto_items = goto_set(s, next, G);
                    cout << "GOTO" << endl;
                    display(goto_items);
                    state++;
                }
                // else {
                //     set<LR0Item> next_items = goto_set(s, next, G);
                //     cout << "GOTO" << endl;
                //     display(next_items);
                //     state++;
                // }
                cout << "STATE " << state << endl;
            }
        }
    }

    return 0;
}
