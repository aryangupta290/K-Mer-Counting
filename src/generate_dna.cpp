#include "bits/stdc++.h"
#include "helper.h"
using namespace std;
const int total_length = 12 * 16e7;
string A;
map<int, char> mm;
#define PB push_back
int main(void)
{
    mm[0] = 'A';
    mm[1] = 'G';
    mm[2] = 'T';
    mm[3] = 'C';
    srand(time(0));

    // As input -> We have a string of huge length
    // Creating a 2 gb file would require us 2e9 charactres
    for (int i = 0; i < total_length; i++)
    {
        int r = rand() % 4;
        A.PB(mm[r]);
    }
    std::ofstream out("large_dna.txt");
    out << A;
    out.close();
    return 0;
}