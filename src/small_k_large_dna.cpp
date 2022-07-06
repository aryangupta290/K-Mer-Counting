#include "bits/stdc++.h"
#include "helper.h"
#include <thread>
#include <cstdlib>
#define NUM_THREADS 12
#define SZ (int)16e7
using namespace std;
map<char, int> mm;
#define PB push_back

void prec()
{
    mm['A'] = 0;
    mm['G'] = 1;
    mm['T'] = 2;
    mm['C'] = 3;
}
int comp_and_swap(int *adr, int old_val, int new_val)
{
    int ret = *adr;
    if (ret == old_val)
    {
        *adr = new_val;
    }
    return ret;
}
int k;

vector<int> result;

void thread_function(int threadid)
{
    // first read
    int tid = threadid;
    ifstream info("large_dna.txt");
    string A;

    for (char c; A.size() != SZ && info.get(c);)
        A += c;

    // cout << tid << endl;
    long long int start = tid * (A.length() / NUM_THREADS);
    long long int end = (tid + 1) * (A.length() / NUM_THREADS);
    int val = 0;
    int mul = 1;
    for (int i = start; i < start + k - 1; i++)
    {
        val += mm[A[i]] * mul;
        mul *= 4;
    }
    for (long long int i = start + k - 1; i < min(end, (long long int)A.length()); i++)
    {
        val += mm[A[i]] * mul;

        int old_val = result[val];
        int new_val = old_val + 1;
        int ret = comp_and_swap(&result[val], old_val, new_val);
        while (ret != old_val)
        {
            old_val = ret;
            new_val = old_val + 1;
            ret = comp_and_swap(&result[val], old_val, new_val);
        }
        val -= mm[A[i - k + 1]];
        val /= 4;
    }
}

int main(void)
{
    prec();
    struct timeval calc;
    double calctime;
    srand(time(0));
    // read file into a string
    // start time
    for (k = 3; k <= 10; k++)
    {

        std::thread myThreads[NUM_THREADS];
        fstream fout;
        string output_file = "k-mer_" + to_string(k) + ".csv";
        int tot = (1 << (2 * k));
        result = vector<int>(1 << (2 * k), 0);
        tick(&calc);


        // buffer contains the dna sequence
        for (int i = 0; i < NUM_THREADS; i++)
        {
            myThreads[i] = std::thread(thread_function, i);
        }
        for (int i = 0; i < NUM_THREADS; i++)
        {
            // join
            myThreads[i].join();
        }

        fout.open(output_file, ios::out | ios::app);
        int total = 0;
        for (int i = 0; i < tot; i++)
        {
            if (result[i])
            {
                string f;
                for (int j = 0; j < 2 * k; j += 2)
                {
                    // bits at j and j+1
                    int r1 = (i >> j) & 3;
                    if (r1 == 0)
                    {
                        f.PB('A');
                    }
                    else if (r1 == 1)
                    {
                        f.PB('G');
                    }
                    else if (r1 == 2)
                    {
                        f.PB('T');
                    }
                    else if (r1 == 3)
                    {
                        f.PB('C');
                    }
                }
                fout << f << "," << result[i] << "\n";
            }
        }
        // cout << "asdsa" << total << endl;
        fout.close();
        calctime = tock(&calc);
        cout << k << " "<< calctime << endl;
    }
}