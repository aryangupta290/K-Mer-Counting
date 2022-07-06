#include "bits/stdc++.h"
#include "helper.h"
using namespace std;
string A;
map<int, char> mm;
#define PB push_back

void prec()
{
    mm['A'] = 0;
    mm['G'] = 1;
    mm['T'] = 2;
    mm['C'] = 3;
}
int main(void)
{
    struct timeval calc;
    double calctime;
    srand(time(0));
    // read file into a string
    // start time
    for (int k = 3; k <= 12; k++)
    {
        fstream fout;
        string output_file = "k-mer_" + to_string(k) + ".csv";
        tick(&calc);
        map<string, int> result;
        std::ifstream t("small_dna.txt");
        std::stringstream buffer;
        buffer << t.rdbuf();

        // buffer contains the dna sequence
        A = buffer.str();
        int len = A.length();
        // end time

        for (int i = 0; i < len - k + 1; i++)
        {
            string f;
            for (int j = i; j < i + k; j++)
            {
                f.PB(A[j]);
            }
            result[f]++;
        }
        fout.open(output_file, ios::out | ios::app);
        for (auto p : result)
        {
            fout << p.first << "," << p.second << "\n";
        }
        fout.close();
        calctime = tock(&calc);
        cout << calctime << endl;
    }
}