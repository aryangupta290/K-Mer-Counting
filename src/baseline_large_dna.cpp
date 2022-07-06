// large data set small k

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
    for (int k = 32; k <= 32; k++)
    {
        fstream fout;
        string output_file = "k-mer_" + to_string(k) + ".csv";
        map<string, int> count;
        tick(&calc);
        std::ifstream t("small_dna.txt");
        std::stringstream buffer;
        buffer << t.rdbuf();

        // buffer contains the dna sequence
        A = buffer.str();
        int len = A.length();
        // end time
        string f = "";
        for (int i = 0; i < len - k + 1; i++)
        {
            f = A.substr(i, k);
            count[f]++;
        }
        fout.open(output_file, ios::out | ios::app);
        for (auto p : count)
        {
            fout << p.first << "," << p.second << "\n";
        }
        fout.close();
        calctime = tock(&calc);
        cout << "k-value " << k << " took " << calctime << " seconds" << endl;
    }
}
const int N = 6e5 + 9;

int power(long long n, long long k, const int mod)
{
    int ans = 1 % mod;
    n %= mod;
    if (n < 0)
        n += mod;
    while (k)
    {
        if (k & 1)
            ans = (long long)ans * n % mod;
        n = (long long)n * n % mod;
        k >>= 1;
    }
    return ans;
}