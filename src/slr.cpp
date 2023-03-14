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

    // Construction of the set of LR0 items.
    int state_count = 0;
    set<set<LR0Item>> states;
    // Pushing in the closure of the first LR0 item and starting the construction.
    states.insert(closure_items);

    // For every state in the set of states
    for (const auto &s : states)
    {
        // for every item in the state
        for (const auto &item : s)
        {
            // get the next character after the dot
            string next = item.right.substr(item.dot, 1);

            if(item.dot < item.right.size()) {
                if (grammar.find(next) != grammar.end())
                {
                    // getting the goto_set of the state from (state, the next character and the grammar)
                    set<LR0Item> goto_items = goto_set(s, next, G);

                    // Insert into states if not already present
                    if (!gotoStateAlreadyExists(states, goto_items))
                    {
                        states.insert(goto_items);
                        state_count++;
                    }
                }
                // if the next character is a terminal
                else
                {
                    set<LR0Item> next_items = goto_set(s, next, G);

                    // Insert into states if not already present
                    if (!gotoStateAlreadyExists(states, next_items))
                    {
                        states.insert(next_items);
                        state_count++;
                    }
                }
            }
        }
    }

    // Logging the states.
    cout << "\nSTATES" << endl;
    int cou = 0;
    for (const auto &state : states)
    {
        cout << "State " << cou++ << endl;
        displayLR0Items(state);
        cout << endl;
    }

    return 0;
}
