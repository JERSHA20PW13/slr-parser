#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <set>

using namespace std;


int main()
{
    ifstream fin("input.txt");

    map<string, vector<string>> grammar;
    string startSymbol;

    if(!fin)
    {
        cout << "Error opening file" << endl;
        return 1;
    }
    else {
        string line;
        while(getline(fin, line))
        {
            if(startSymbol == "") {
                startSymbol = line.substr(0, line.find("->"));
                grammar[startSymbol+"\b'"].push_back(startSymbol);
            }
            string key = line.substr(0, line.find("->"));
            string value = line.substr(line.find("->") + 2, line.length());
            grammar[key].push_back(value);
        }
    }

    for(auto it = grammar.begin(); it != grammar.end(); it++)
    {
        cout << it->first << " -> ";
        for(auto it2 = it->second.begin(); it2 != it->second.end(); it2++)
        {
            if(it2 != it->second.end() - 1)
                cout << *it2 << " | ";
            else
                cout << *it2;
        }
        cout << endl;
    }

    return 0;
}
