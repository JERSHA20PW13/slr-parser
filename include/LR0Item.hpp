#ifndef LR0ITEM_HPP
#define LR0ITEM_HPP

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
        if(right != other.right)
            return right < other.right;
        else
            return dot < other.dot;
    }

    LR0Item() {}

    LR0Item(string left, string right, int dot)
    {
        this->left = left;
        this->right = right;
        this->dot = dot;
    }
};

void displayLR0Items(set<LR0Item> items)
{
    for (const auto &item : items)
    {
        cout << item.left << " -> " << item.right.substr(0, item.dot) << "." << item.right.substr(item.dot) << endl;
    }
}

set<LR0Item> closure(set<LR0Item> items, Grammar &G)
{
    // Getting the map of the augmented grammar.
    map<string, vector<string>> grammar = G.getAugmentedGrammarMap();
    // initializing the closure items with the given items.
    set<LR0Item> closure_items = items;

    // Starting off the closure construction with an infinite loop till no new items are added to the closure.
    while (true)
    {
        // flag to check if any new items are added to the closure.
        bool added = false;

        // for every item in the closure_items
        for (const auto &item : closure_items)
        {
            // if the dot is at the end of the right hand side of the item
            if (item.dot >= item.right.size())
            {
                continue;
            }

            // get the next character after the dot
            string next = item.right.substr(item.dot, 1);

            // for every production of the next character
            for (const auto &production : grammar[next])
            {
                /* make a new LR0 item with the next character as the left hand side,
                the production as the right hand side and the dot at the start. */
                LR0Item new_item = LR0Item(next, production, 0);

                // Flag to check if the new item is already present in the closure.
                bool flag = true;

                // LOGIC TO CHECK IF THE NEW ITEM IS ALREADY PRESENT IN THE CLOSURE.
                // for every item in the closure
                for (const auto &item : closure_items)
                {
                    // if the new item is already present in the closure
                    if (item == new_item)
                    {
                        flag = false;
                        break;
                    }
                }

                // if the new item is not already present in the closure
                if (flag)
                {
                    // add the new item to the closure
                    added = true;
                    closure_items.insert(new_item);
                }
            }
        }
        // if no new items are added to the closure, break out of the infinite loop.
        if (!added)
            break;
    }
    // return the closure items.
    return closure_items;
}

set<LR0Item> goto_set(set<LR0Item> items, string symbol, Grammar &G)
{
    // Initializing the goto items.
    set<LR0Item> goto_items;

    // Getting the map of the augmented grammar.
    map<string, vector<string>> grammar = G.getAugmentedGrammarMap();

    // for every item in the given items
    for (const auto &item : items)
    {
        // if the dot is at the end of the right hand side of the item
        if (item.dot >= item.right.size())
        {
            continue;
        }

        // get the next character after the dot
        string next = item.right.substr(item.dot, 1);

        // if the next character is the given symbol
        if (next == symbol)
        {
            // make a new LR0 item with the same left hand side, right hand side and dot at the next position.
            LR0Item new_item = LR0Item(item.left, item.right, item.dot + 1);
            // add the new item to the goto items.
            goto_items.insert(new_item);
        }
    }
    // Finally return the closure of the new goto items.
    set<LR0Item> closure_items = closure(goto_items, G);

    return closure_items;
}

bool gotoStateAlreadyExists(set<set<LR0Item>> states, set<LR0Item> state)
{
    bool gotoStateExists = false;
    // for every state in the states
    for (const auto &setOfLR0Items : states)
    {
        // flag to check if the given state is already present in the states.
        if (setOfLR0Items == state)
        {
            gotoStateExists = true;
            break;
        }
    }

    return gotoStateExists;
}

#endif