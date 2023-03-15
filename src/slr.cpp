#include "../include/Includes.hpp"

// macros
#define INPUT_FILE "input.txt"

int main()
{
    // Getting the grammar as input from an input file.
    Grammar G(INPUT_FILE);

    // Getting the map of the augmented grammar.
    map<string, vector<string>> grammar = G.getAugmentedGrammarMap();

    // Making the first LR0 item with the augmented start symbol.
    LR0Item item = LR0Item(G.getAugmentedStartSymbol(), grammar[G.getAugmentedStartSymbol()][0], 0);

    set<LR0Item> items;
    items.insert(item);

    // Getting the closure of the first LR0 item.
    set<LR0Item> closure_items = closure(items, G);

    // Logging the closure.
    // cout << "\nCLOSURE" << endl;
    // displayLR0Items(closure_items);


    // cout << "FIRST" << endl;
    // map<string, set<string>> first = G.getFirst();
    // for (const auto &f : first)
    // {
    //     cout << f.first << " : ";
    //     for (const auto &s : f.second)
    //     {
    //         cout << s << " ";
    //     }
    //     cout << endl;
    // }

    // cout << "FOLLOW" << endl;
    // map<string, set<string>> follow = G.getFollow();
    // for (const auto &f : follow)
    // {
    //     cout << f.first << " : ";
    //     for (const auto &s : f.second)
    //     {
    //         cout << s << " ";
    //     }
    //     cout << endl;
    // }


    // Construction of the transition table.
    map<pair<int, string>, string> slr_table;

    // Construction of the set of LR0 items.
    int state_count = 0;
    map<int, set<LR0Item>> states;
    // Pushing in the closure of the first LR0 item and starting the construction.
    states[0] = closure_items;

    int count = 0;

    // For every state in the set of states
    while(count <= state_count)
    {
        set<LR0Item> s = states[count];

        // for every item in the state
        for (const auto &item : s)
        {
            if(item.dot < item.right.size()) {
                // get the next character after the dot
                string next = item.right.substr(item.dot, 1);
                
                // getting the goto_set of the state from (state, the next character and the grammar)
                set<LR0Item> goto_items = goto_set(s, next, G);

                // Insert into states if not already present
                if (!gotoStateAlreadyExists(states, goto_items))
                {
                    states[++state_count] = goto_items;

                    if(next >= "A" && next <= "Z") {
                        slr_table[make_pair(count, next)] = to_string(state_count);
                    }
                    else {
                        slr_table[make_pair(count, next)] = "shift " + to_string(state_count);
                    }
                }
            }
            else {
                if(item.left == G.getAugmentedStartSymbol()) {
                    slr_table[make_pair(count, "$")] = "accept";
                }
                else {
                    set<string> follow = G.getFollow()[item.left];
                    for (const auto &f : follow)
                    {
                        slr_table[make_pair(count, f)] = "reduce " + item.left + " -> " + item.right;
                    }
                }
            }
        }
        count++;
    }

    cout << "state_count: " << state_count << endl;

    // Logging the states.
    // cout << "\nSTATES" << endl;
    // count = 0;
    // for (int i=0; i<=state_count; i++)
    // {
    //     cout << "State " << count++ << endl;
    //     displayLR0Items(states[i]);
    //     cout << endl;
    // }

    return 0;
}
