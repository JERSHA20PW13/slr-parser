#ifndef PARSER_HPP
#define PARSER_HPP
#include "Grammar.hpp"
#include "LR0Item.hpp"
#include <stack>

class Parser
{
private:
    map<pair<int, string>, string> slr_table;
    string input;
    stack<string> parseStack;
    vector<string> tokenizeInput();

public:
    Parser();
    Parser(map<pair<int, string>, string> slr_table);
    map<pair<int, string>, string> getSLRTable();
    void getInput();
    bool parse();
};

Parser::Parser()
{
}

Parser::Parser(map<pair<int, string>, string> slr_table)
{
    this->slr_table = slr_table;
}

map<pair<int, string>, string> Parser::getSLRTable()
{
    return this->slr_table;
}

void Parser::getInput()
{
    cout << "Enter the input string: ";
    // cin >> this->input;
    this->input = "i * i + i";
    this->input += " $";
}

vector<string> Parser::tokenizeInput()
{
    vector<string> tokens;
    string token = "";
    for (int i = 0; i < this->input.length(); i++)
    {
        if (this->input[i] == ' ')
        {
            tokens.push_back(token);
            token = "";
        }
        else
        {
            token += this->input[i];
        }
    }
    tokens.push_back(token);
    return tokens;
}

bool Parser::parse()
{
    parseStack.push("0");

    vector<string> tokens = tokenizeInput();

    for (int i = 0; i < tokens.size(); i++)
    {
        string token = tokens[i];
        string top = parseStack.top();
        string action = slr_table[make_pair(stoi(top), token)];

        cout << endl
             << "-----------------------" << endl;
        // STACK | ACTION | INPUT
        cout << "Stack: ";
        stack<string> temp = parseStack;
        while (!temp.empty())
        {
            cout << temp.top() << " ";
            temp.pop();
        }
        cout << endl;
        cout << "Action: " << action << endl;
        cout << "Input: ";
        for (int j = i; j < tokens.size(); j++)
        {
            cout << tokens[j] << " ";
        }
        cout << endl;

        cout << "-----------------------" << endl;

        if (action == "")
        {
            cout << "Error: Invalid input string." << endl;
            return false;
        }
        else if (action[0] == 'S')
        {
            parseStack.push(token);
            parseStack.push(action.substr(2, action.length() - 1));
        }
        else if (action[0] == 'R')
        {
            string rule = action.substr(2, action.length() - 1);
            string left = rule.substr(0, rule.find("->"));
            string right = rule.substr(rule.find("->") + 2, rule.length() - 1);
            for (int i = 0; i < right.length(); i++)
            {
                parseStack.pop();
                parseStack.pop();
            }
            string stackTopWhenPushing = parseStack.top();
            parseStack.push(left);
            parseStack.push(slr_table[make_pair(stoi(stackTopWhenPushing), left)]);
            i--;
        }
        else if (action == "A")
        {
            cout << "String accepted." << endl;
            return true;
        }
    }
    return false;
}

#endif // PARSER_HPP