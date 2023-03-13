#ifndef LR0ITEM_HPP
#define LR0ITEM_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include "Grammar.hpp"
using namespace std;

class LR0Item
{
public:
    string left;
    string right;
    int dot;

    bool operator==(const LR0Item &other) const
    {
        return left == other.left && right == other.right && dot == other.dot;
    }

    bool operator<(const LR0Item &other) const
    {
        return right < other.right;
    }

    LR0Item() {}

    LR0Item(string left, string right, int dot)
    {
        this->left = left;
        this->right = right;
        this->dot = dot;
    }
};

set<LR0Item> closure(set<LR0Item> items, Grammar &G)
{
    map<string, vector<string>> grammar = G.getAugmentedGrammarMap();
    set<LR0Item> closure_items = items;

    while (true)
    {
        bool added = false;

        for (const auto &item : closure_items)
        {
            if (item.dot >= item.right.size())
            {
                continue;
            }

            char next_ch = item.right[item.dot];
            string next;
            next += next_ch;

            for (const auto &production : grammar[next])
            {
                LR0Item new_item = LR0Item(next, production, 0);
                bool flag = true;

                for (const auto &item : closure_items)
                {
                    if (item == new_item)
                    {
                        flag = false;
                        break;
                    }
                }

                if (flag)
                {
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

set<LR0Item> goto_set(set<LR0Item> items, string symbol, Grammar &G)
{
    set<LR0Item> goto_items;
    map<string, vector<string>> grammar = G.getAugmentedGrammarMap();

    for (const auto &item : items)
    {
        if (item.dot >= item.right.size())
        {
            continue;
        }

        char next_ch = item.right[item.dot];
        string next;
        next += next_ch;

        if (next == symbol)
        {
            LR0Item new_item = LR0Item(item.left, item.right, item.dot + 1);
            goto_items.insert(new_item);
        }
    }

    return closure(goto_items, G);
}

void display(set<LR0Item> &items)
{
    for (const auto &item : items)
    {
        cout << item.left << " -> ";
        for (int i = 0; i < item.right.size(); i++)
        {
            if (i == item.dot)
            {
                cout << ".";
            }
            cout << item.right[i];
        }
        if (item.dot == item.right.size())
        {
            cout << ".";
        }
        cout << endl;
    }
}

#endif