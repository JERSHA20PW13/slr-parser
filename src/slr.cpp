#include "../include/Grammar.hpp"
#include "../include/LR0Item.hpp"
using namespace std;

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

    for (const auto &s : states)
    {
        for (const auto &item : s)
        {
            if (item.dot >= item.right.size())
            {
                if (item.left == G.getAugmentedStartSymbol() && item.right == grammar[G.getAugmentedStartSymbol()][0] && item.dot == 1)
                {
                    cout << "ACCEPT" << endl;
                }
                else
                {
                    cout << "REDUCE" << endl;
                }
            }
            else
            {
                char next_ch = item.right[item.dot];
                string next;
                next += next_ch;

                if (grammar.find(next) != grammar.end())
                {
                    set<LR0Item> goto_items = goto_set(s, next, G);
                    cout << "GOTO" << endl;
                    display(goto_items);
                    state++;
                }
                // else
                // {
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
