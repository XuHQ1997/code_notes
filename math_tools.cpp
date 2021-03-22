#include <vector>
#include <iostream>

using namespace std;

// Compute the greatest common divisor of a and b.
int gcd(int a, int b) {
    return a % b == 0 ? b : gcd(b, a % b);
}

// Compute the number of combination.
// C^k_n = \frac{n*(n-1)*...*(n-k+1)}{k!}
int combination(int n, int k) {
	int res = 1, j = 1;
	k = min(n - k, k);
	for(int i = 0; i < k; ++i) {
		res *= n--;
		while(j <= k && res % j == 0)
			res /= j++;
	}
	return res;
}

// Get primes smaller than n in time complexity of O(n).
vector<int> linear_prime_sieve(int n) {
    vector<int> primes;
    primes.reserve(n);

    vector<bool> flags(n, true);
    flags[0] = flags[1] = false;

    for(int i = 2; i < n; ++i) {
        if(flags[i])
            primes.push_back(i);
        
        for(int j = 0; j < primes.size() && i * primes[j] < flags.size(); ++j) {
            flags[i * primes[j]] = false;
            if(i % primes[j] == 0)
                break;
        }
    }
    return primes;
}

int main() {
    cout << gcd(1, 2) << endl;
    cout << gcd(2, 4) << endl;
    cout << gcd(12, 8) << endl;
    cout << gcd(7, 3) << endl;

    cout << combination(5, 3) << endl;
    cout << combination(10, 3) << endl;
    
    auto primes = linear_prime_sieve(100);
    for(int i : primes) 
        cout << i << " ";
    cout << endl;

    return 0;
}
