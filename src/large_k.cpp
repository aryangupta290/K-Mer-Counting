#include "bits/stdc++.h"
#include "helper.h"
#include <thread>
#include <cstdlib>
#define NUM_THREADS 12

using namespace std;
string A;
map<char, int> mm;
#define PB push_back
const int N = 2500100;
const int k = 32;

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

const int MOD[4] = {2500009, 2500027, 2500049, 2500051};
const int p1[4] = {137, 277, 167, 191};
int ip[4];

// change 32 to current value of k
int pw[4][k], ipw[4][k];

void prec()
{
    mm['A'] = 0;
    mm['G'] = 2;
    mm['T'] = 3;
    mm['C'] = 1;
    for (int i = 0; i < 4; i++)
        pw[i][0] = 1;
    for (int i = 1; i < k; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            pw[j][i] = 1LL * pw[j][i - 1] * p1[j] % MOD[(j)];
            // cout << pw[j][i] << " ";
        }
    }
    for (int i = 0; i < 4; i++)
        ipw[i][0] = 1;
    int p[4];
    for (int i = 0; i < 4; i++)
    {
        p[i] = power(p1[i], MOD[i] - 2, MOD[i]);
        ip[i] = p[i];
    }
    for (int i = 0; i < 4; i++)
    {
        ipw[i][0] = 1;
    }
    for (int i = 1; i < k; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            ipw[j][i] = 1LL * ipw[j][i - 1] * p[j] % MOD[j];
        }
    }
}
const int sz = 100001;
pair<string, int> hash_table[NUM_THREADS][sz];
int bloom_filter[NUM_THREADS][N];

void countSort(pair<string, int> *a, int size, size_t k)
{
    // k is
    pair<string, int> *b = NULL;
    int *c = NULL;
    b = new pair<string, int>[size];
    c = new int[4];

    for (int i = 0; i < 4; i++)
    {
        c[i] = 0;
        // cout << c[i] << "\n";
    }
    for (int j = 0; j < size; j++)
    {
        c[mm[a[j].first[k]]]++;
    }
    // cout << "assda" << endl;

    for (int f = 1; f < 4; f++)
    {
        c[f] += c[f - 1];
    }

    for (int r = size - 1; r >= 0; r--)
    {
        b[c[mm[a[r].first[k]]] - 1] = a[r];
        c[mm[a[r].first[k]]]--;
    }

    for (int l = 0; l < size; l++)
    {
        a[l] = b[l];
    }

    // avold memory leak
    delete[] b;
    delete[] c;
}
void radixSort(pair<string, int> *b, int r)
{
    size_t max = k;
    for (size_t digit = max; digit > 0; digit--)
    { // size_t is unsigned, so avoid using digit >= 0, which is always true
        countSort(b, r, digit - 1);
    }
}

int compact(int tid, int cur, int last)
{
    int track = last;
    for (int i = last; i < cur; i++)
    {
        int j = i;
        int sum = 0;
        for (; j < cur; j++)
        {
            if (hash_table[tid][i].first == hash_table[tid][j].first)
            {
                sum += hash_table[tid][j].second;
            }
            else
            {
                break;
            }
        }
        hash_table[tid][track].second = sum;
        hash_table[tid][track].first = hash_table[tid][i].first;
        i = j - 1;
        track++;
    } // from 0 to last-1 and last to track - 1
    int ptr1 = 0;
    int ptr2 = last;
    pair<string, int> temp[track];
    int ctr = 0;
    while (ptr1 < last && ptr2 < track)
    {
        if (hash_table[tid][ptr1].first < hash_table[tid][ptr2].first)
        {
            temp[ctr++] = hash_table[tid][ptr1];
            ptr1++;
        }
        else if (hash_table[tid][ptr1].first > hash_table[tid][ptr2].first)
        {
            temp[ctr++] = hash_table[tid][ptr2];
            ptr2++;
        }
        else
        {
            temp[ctr] = hash_table[tid][ptr1];
            temp[ctr++].second += hash_table[tid][ptr2].second;
            ptr1++;
            ptr2++;
        }
    }
    while (ptr1 < last)
    {
        temp[ctr++] = hash_table[tid][ptr1];
        ptr1++;
    }
    while (ptr2 < track)
    {
        temp[ctr++] = hash_table[tid][ptr2];
        ptr2++;
    }
    for (int i = 0; i < ctr; i++)
    {
        hash_table[tid][i] = temp[i];
    }
    for (int i = ctr; i < sz; i++)
    {
        hash_table[tid][i].first = "";
        hash_table[tid][i].second = 0;
    }
    return ctr;
}
int threshold = 25;
void thread_function(int threadid)
{
    int tid = threadid;
    // cout << tid << endl;
    long long int start = tid * (A.length() / NUM_THREADS);
    long long int end = (tid + 1) * (A.length() / NUM_THREADS);
    memset(bloom_filter[tid], 0, sizeof(bloom_filter[tid]));

    long long int hash[4];
    for (int i = 0; i < 4; i++)
    {
        hash[i] = 0;
    }

    for (int i = start; i < start + k - 1; i++)
    {

        for (int j = 0; j < 4; j++)
        {
            hash[j] = (hash[j] + 1LL * pw[j][i - start] * A[i] % MOD[j]) % MOD[j];
        }
    }

    // cout << cur << endl;

    int cur = 0;
    int last = 0;

    for (int i = start + k - 1; i < min(end, (long long int)A.length()); i++)
    {
        for (int j = 0; j < 4; j++)
        {
            hash[j] = (hash[j] + 1LL * pw[j][k - 1] * A[i] % MOD[j]) % MOD[j];
        } //
          // apply bloom filter
        pair<string, int> p;
        p.first = A.substr(i - k + 1, k);
        p.second = 1;
        if (bloom_filter[tid][hash[0]] >= threshold && bloom_filter[tid][hash[1]] >= threshold && bloom_filter[tid][hash[2]] >= threshold && bloom_filter[tid][hash[3]] >= threshold)
        {
            if (cur >= sz && last < sz)
            {
                // sort from (last to cur-1)
                // using two pointer technique technique we
                radixSort(&hash_table[tid][0], cur);
                // do combination
                last = compact(tid, cur, last);
                cur = last;
            }
            if (cur < sz)
            {
                hash_table[tid][cur] = p;
                cur++;
            }
        }
        else
        {
            bloom_filter[tid][hash[0]]++;
            bloom_filter[tid][hash[1]]++;
            bloom_filter[tid][hash[2]]++;
            bloom_filter[tid][hash[3]]++;
        }

        // fix hash value
        for (int j = 0; j < 4; j++)
        {
            hash[j] = (hash[j] - 1LL * A[i - k + 1] + MOD[j]) % MOD[j];
            hash[j] = (hash[j] * ip[j]) % MOD[j];
        }
    }
}
vector<pair<string, int>> result;

void combine()
{
    for (int i = 0; i < sz; i++)
    {
        if (hash_table[0][i].first != "")
        {
            result.push_back(hash_table[0][i]);
            result.back().second += threshold;
        }
        else
            break;
    }
    for (int i = 1; i < NUM_THREADS; i++)
    {
        vector<pair<string, int>> temp;

        int ptr1 = 0;
        int ptr2 = 0;
        int to1 = result.size();
        int to2 = sz;
        for (int l = 0; l < sz; l++)
        {
            if (hash_table[i][l].second == 0)
            {
                to2 = l;
                break;
            }
        }
        while (ptr1 < to1 && ptr2 < to2)
        {
            if (result[ptr1].first < hash_table[i][ptr2].first)
            {
                temp.push_back(result[ptr1]);
                ptr1++;
            }
            else if (result[ptr1].first > hash_table[i][ptr2].first)
            {
                temp.push_back(hash_table[i][ptr2]);
                temp.back().second += threshold;
                ptr2++;
            }
            else
            {
                temp.push_back(result[ptr1]);
                temp.back().second += hash_table[i][ptr2].second + threshold;
                ptr1++;
                ptr2++;
            }
        }
        while (ptr1 < to1)
        {
            temp.push_back(result[ptr1]);
            ptr1++;
        }
        while (ptr2 < to2)
        {
            temp.push_back(hash_table[i][ptr2]);
            temp.back().second += threshold;
            ptr2++;
        }
        result = temp;
    }
}

int main(void)
{
    prec();
    struct timeval calc;
    double calctime;
    srand(time(0));

    // for (; k <= 32; k++)
    {

        std::thread myThreads[NUM_THREADS];
        fstream fout;
        string output_file = "k-mer_" + to_string(k) + ".csv";
        int tot = (1 << (2 * k));
        tick(&calc);

        std::ifstream t("small_dna.txt");
        std::stringstream buffer;
        buffer << t.rdbuf();
        A = buffer.str();

        for (int i = 0; i < NUM_THREADS; i++)
        {
            myThreads[i] = std::thread(thread_function, i);
        }
        for (int i = 0; i < NUM_THREADS; i++)
        {
            // join
            myThreads[i].join();
        }
        combine();

        fout.open(output_file, ios::out | ios::app);
        int total = 0;
        for (int i = 0; i < result.size(); i++)
        {
            fout << result[i].first << "," << result[i].second << endl;
        }
        // // cout << "asdsa" << total << endl;
        fout.close();
        calctime = tock(&calc);
        cout << "k-value " << k << " took " << calctime << " seconds" << endl;
    }
}
