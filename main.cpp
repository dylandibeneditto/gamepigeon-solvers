#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

bool isSubset(const string &query, const string &word)
{
    unordered_map<char, int> queryFreq, wordFreq;
    for (char c : query)
    {
        queryFreq[c]++;
    }
    for (char c : word)
    {
        wordFreq[c]++;
    }
    for (auto &pair : wordFreq)
    {
        if (queryFreq[pair.first] < pair.second)
        {
            return false;
        }
    }
    return true;
}

void findWords(const string &query, const vector<string> &dictionary, vector<string> &foundWords, string currentWord, int startIndex)
{
    if (currentWord.size() >= 3 && currentWord.find(' ') == string::npos)
    {
        foundWords.push_back(currentWord);
    }
    for (int i = startIndex; i < dictionary.size(); ++i)
    {
        string word = dictionary[i];
        if (word.size() > query.size())
        {
            continue;
        }
        if (isSubset(query, word))
        {
            string remaining = query;
            for (char c : word)
            {
                auto it = find(remaining.begin(), remaining.end(), c);
                if (it != remaining.end())
                {
                    remaining.erase(it);
                }
            }
            findWords(remaining, dictionary, foundWords, currentWord.empty() ? word : currentWord, i + 1);
        }
    }
}

int main()
{
    fstream newfile;
    newfile.open("dict.txt", ios::in);

    string query;
    cout << "Enter query: ";
    cin >> query;

    vector<string> dictionary;
    string word;
    while (newfile >> word)
    {
        if (word.size() >= 3)
        {
            dictionary.push_back(word);
        }
    }

    vector<string> foundWords;
    findWords(query, dictionary, foundWords, "", 0);

    // Sort the found words based on length
    sort(foundWords.begin(), foundWords.end(), [](const string &a, const string &b)
         { return a.size() < b.size(); });

    for (const string &word : foundWords)
    {
        if (word.find(' ') == string::npos)
        {
            cout << word << endl;
        }
    }

    return 0;
}
