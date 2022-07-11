# K-Mer-Counting

Team Members :

1. ```Anmoldeep Kaur Dhillon``` (2020101085) <br />
2. ```Aryan Gupta``` (2020101091)

# Introduction

* In bioinformatics, k-mers are substrings of length ```k``` contained within a biological sequence.  
* It can be observed a sequence of length ```L``` will have ```L-k+1```  k-mers and **n<sup>k</sup>** total possible k-mers, where n is number of possible monomer.  
* The task is to count the number of all possible occurences of k-mers in the sequence.  

# Approach

* For a given ```K``` , we count the number of all occurrences of k-mer in the sequence.   
* K-mer counting is simple if we do not care about efficiency since we can store the count of each string in a map and maintain the count , however  due to memory limit ,the efficient execution of this task is far from trivial . 
* We divided the problem into three cases as follows :  
     * Small sequence size (less than 256 MB) and k<13.
     * Small sequence size (less than 256 MB) and k<13.
     * K >= 13  
* For case 1 and 2 (where k is small) , we can diretly maintain a map and store all the possible k-mers.
* However in case 3 , the number of possible k-mers will grow beyond the available memory and so instead of giving the count of each k-mer , we will output the count of the most frequent k-mers .
 # Optimization Techniques
 
 ## Threading and OpenMP
 We used Threading and OpenMP library to achieve parallization and ensure maximum utilisation of CPU .  
 ## Compare and swap
 We used a ```lock free``` data structure to ensure that when multiple threads are trying to change the value at same array index , they don't encounter a race  condition and unnecessarily waste CPU time. Compare and Swap ensures that at least oen thread is non-starving .  
 ## Bloom Filter with Min Alignment 
* They are a space-efficient probabilistic data structure that test whether an element is a member of a set and if they are approximately how many times with have been same .  
* We begin with a empty array and use 4 different hash function . If all the hash values for a k-mer is above a threshold , we add it to an array else we just increase the count at each of the four hash values .  
## Soring and compaction

* After the array A is filled , we sort the newly added elements , then do a linear traversal through them combine those which are the same .  
* After this , we employ a two pointer method on the already sorted string and the newly sorted string so that each k-mer appear only once and in the new space generated , the remaining elements are added
until array is full and process is repeated .  
* For best performance , we used Radix Sort .

# Performance Evaluation Framework 

We used ```Execution_Time``` as a metric to compare various implementations . We used the tick and tock function of the original BLAS implementation to effetively find the execution time of any section of the code .

# Results 

We observed around ```100``` times speed up in execution time in each of the three cases . This was specially remarkable in case of large value of ```k```.  
* Small Data set :
<img src="small_k.png" alt="Alt text" title="Optional title">  

* Large Data set :
<img src="large_k.png" alt="Alt text" title="Optional title">    

* More extended results can be found in the attached presentation .  

# File Structure 
* ```baseline_large_dna.cpp```:  Baseline code for the case of small k (<15) and small data size (around 250 MB) .  
* ```baseline_small_k_large_dna.cpp```:  Baseline code for the case of small k (<15) and large data size (around 2 GB) .
* ```baseline_large_k.cpp```:  Baseline code for the case of large k.  
* ```small_k_small_dna.cpp```:  Optimised code for the case of small k and small data size .  
* ```small_k_large_dna.cpp```:  Optimised code for the case of small k  and large data size.  
* ```large_k.cpp```:  Optmised code for large k (Bloom Filter Implementation). 



