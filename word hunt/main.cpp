#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

vector<string> wordSearch(string query, unsigned short int dimension) {
 
}

int main()
{
    system("clear");
    unsigned short int dimension;
    cout << "dimension (?x?): ";
    cin >> dimension;
    string query = string(".", dimension * dimension);
    for (unsigned short int i = 0; i < dimension * dimension; i++)
    {
        char input;
        cin >> input;
        system("clear");
        query[i] = input;

        // row
        for (unsigned short int j = 0; j < dimension; j++)
        {
            // column
            for (unsigned short int k = 0; k < dimension; k++)
            {
                cout << query[(j * dimension) + k];
            }
            cout << endl;
        }
    }
    vector<string> words = wordSearch(query, dimension);
    return 0;
}