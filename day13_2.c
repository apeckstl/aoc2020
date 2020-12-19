#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

int N;

long long int inv(long long int a, long long int m) 
{ 
    long long int m0 = m, t, q; 
    long long int x0 = 0, x1 = 1; 
 
    if (m == 1) 
    return 0; 
 
    // Apply extended Euclid Algorithm 
    while (a > 1) 
    { 
        // q is quotient 
        q = a / m; 
 
        t = m; 
 
        // m is remainder now, process same as 
        // euclid's algo 
        m = a % m, a = t; 
 
        t = x0; 
 
        x0 = x1 - q * x0; 
 
        x1 = t; 
    } 
 
    // Make x1 positive 
    if (x1 < 0) 
    x1 += m0; 
 
    return x1; 
} 
 
// k is size of num[] and rem[]. Returns the smallest 
// number x such that: 
// x % num[0] = rem[0], 
// x % num[1] = rem[1], 
// .................. 
// x % num[k-2] = rem[k-1] 
// Assumption: Numbers in num[] are pairwise coprime 
// (gcd for every pair is 1) 
long long int findMinX(int num[], int rem[], int k) 
{ 
    // Compute product of all numbers 
    long long int prod = 1; 
    for (long long int i = 0; i < k; i++) 
        prod *= num[i]; 
 
    // Initialize result 
    long long int result = 0; 
 
    // Apply above formula 
    for (int i = 0; i < k; i++) 
    { 
        long long int pp = prod / num[i]; 
        result += rem[i] * inv(pp, num[i]) * pp; 
    } 
 
    return result % prod; 
} 


int main(int argc, char **argv) {
	int busnos[9] = {23,41,733,13,17,19,29,449,37};
	int indices[9] = {0,13,23,36,37,42,52,54,91};
	int offsets[9];

	for (int i = 0; i < 9; i++) 
		offsets[i] = (busnos[i] - indices[i]) % busnos[i];

	int k = 9;
	printf("Answer = %llu\n", findMinX(busnos, offsets, k));
	return 0;
}
