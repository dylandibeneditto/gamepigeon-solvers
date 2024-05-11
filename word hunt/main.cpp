#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstring> // for strlen
#include <algorithm> // for memset

using namespace std;

#define char_int(c) ((int)c - (int)'A')

#define SIZE (26)

#define DIMENSION (5)

// Trie Node
struct TrieNode {
    TrieNode* Child[SIZE];
    // isLeaf is true if the node represents end of a word
    bool leaf;
};

// Returns new trie node (initialized to NULLs)
TrieNode* getNode() {
    TrieNode* newNode = new TrieNode;
    newNode->leaf = false;
    for (int i = 0; i < SIZE; i++) newNode->Child[i] = NULL;
    return newNode;
}

// If not present, inserts a key into the trie
// If the key is a prefix of trie node, just
// marks leaf node
void insert(TrieNode* root, const string& Key) {
    TrieNode* pChild = root;
    for (int i = 0; i < Key.length(); i++) {
        int index = char_int(Key[i]);
        if (pChild->Child[index] == NULL) pChild->Child[index] = getNode();
        pChild = pChild->Child[index];
    }
    // make last node as leaf node
    pChild->leaf = true;
}

// function to check that current location
// (i and j) is in matrix range
bool isSafe(int i, int j, bool visited[DIMENSION][DIMENSION]) {
    return (i >= 0 && i < DIMENSION && j >= 0 && j < DIMENSION && !visited[i][j]);
}

// A recursive function to print all words present on boggle
void searchWord(TrieNode* root, char boggle[DIMENSION][DIMENSION], int i, int j,
                bool visited[][DIMENSION], string str) {
    // if we found word in trie / dictionary
    if (root->leaf == true) cout << str << endl;

    // If both I and j in  range and we visited
    // that element of matrix first time
    if (isSafe(i, j, visited)) {
        // make it visited
        visited[i][j] = true;
        // traverse all children of current root
        for (int K = 0; K < SIZE; K++) {
            if (root->Child[K] != NULL) {
                // current character
                char ch = (char)K + (char)'A';
                // Recursively search remaining character of word
                // in trie for all 8 adjacent cells of boggle[i][j]
                if (isSafe(i + 1, j + 1, visited) && boggle[i + 1][j + 1] == ch)
                    searchWord(root->Child[K], boggle, i + 1, j + 1, visited, str + ch);
                if (isSafe(i, j + 1, visited) && boggle[i][j + 1] == ch)
                    searchWord(root->Child[K], boggle, i, j + 1, visited, str + ch);
                if (isSafe(i - 1, j + 1, visited) && boggle[i - 1][j + 1] == ch)
                    searchWord(root->Child[K], boggle, i - 1, j + 1, visited, str + ch);
                if (isSafe(i + 1, j, visited) && boggle[i + 1][j] == ch)
                    searchWord(root->Child[K], boggle, i + 1, j, visited, str + ch);
                if (isSafe(i + 1, j - 1, visited) && boggle[i + 1][j - 1] == ch)
                    searchWord(root->Child[K], boggle, i + 1, j - 1, visited, str + ch);
                if (isSafe(i, j - 1, visited) && boggle[i][j - 1] == ch)
                    searchWord(root->Child[K], boggle, i, j - 1, visited, str + ch);
                if (isSafe(i - 1, j - 1, visited) && boggle[i - 1][j - 1] == ch)
                    searchWord(root->Child[K], boggle, i - 1, j - 1, visited, str + ch);
                if (isSafe(i - 1, j, visited) && boggle[i - 1][j] == ch)
                    searchWord(root->Child[K], boggle, i - 1, j, visited, str + ch);
            }
        }
        // make current element unvisited
        visited[i][j] = false;
    }
}

// Prints all words present in dictionary.
void findWords(char boggle[DIMENSION][DIMENSION], TrieNode* root) {
    // Mark all characters as not visited
    bool visited[DIMENSION][DIMENSION];
    memset(visited, false, sizeof(visited));

    string str = "";

    // traverse all matrix elements
    for (int i = 0; i < DIMENSION; i++) {
        for (int j = 0; j < DIMENSION; j++) {
            // we start searching for word in dictionary
            // if we found a character which is child
            // of Trie root
            if (root->Child[char_int(boggle[i][j])]) {
                str = boggle[i][j];
                searchWord(root->Child[char_int(boggle[i][j])], boggle, i, j, visited, str);
                str = "";
            }
        }
    }
}

int main() {
    fstream newfile;
    newfile.open("../dict.txt", ios::in);
    system("clear");

    char query[DIMENSION][DIMENSION];
    for (unsigned short int i = 0; i < DIMENSION * DIMENSION; i++) {
        char input;
        cin >> input;
        system("clear");
        query[i % DIMENSION][i / DIMENSION] = input;

        // row
        for (unsigned short int j = 0; j < DIMENSION; j++) {
            // column
            for (unsigned short int k = 0; k < DIMENSION; k++) {
                cout << query[k][j];
            }
            cout << endl;
        }
    }

    TrieNode* root = getNode();

    vector<string> dictionary;
    string word;
    while (newfile >> word) {
        if (word.size() >= 3) {
            dictionary.push_back(word);
        }
    }

    // insert all words of dictionary into trie
    for (const string& word : dictionary) {
        insert(root, word);
    }

    findWords(query, root);
    return 0;
}
