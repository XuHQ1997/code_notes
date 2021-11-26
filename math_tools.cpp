#include <vector>
#include <iostream>

using namespace std;

// return lower case of a alpha
char lower(char c) {
    return c | 0x20;
}

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

namespace average {
long long v1(const vector<long long>& nums) {
    long long sum = 0;
    for(auto i : nums)
        sum += i;  // overflow
    return sum / nums.size();
}

long long v2(const vector<long long>& nums) {
    long long size = nums.size();
    long long avg = 0;
    long long remainder = 0;

    for(auto i : nums) {
        avg += i / size;
        remainder += i % size;  // overflow

        avg += remainder / size;
        remainder %= size;
    }
    return avg;
}

long long v3(const vector<long long>& nums) {
    long long size = nums.size();
    long long avg = 0;
    long long remainder = 0;

    for(auto i : nums) {
        avg += i / size;

        long long current_remainder = i % size;
        if(remainder > LLONG_MAX - current_remainder) {
            ++avg;
            remainder -= size;
        }
        remainder += current_remainder;

        avg += remainder / size;
        remainder %= size;
    }
    return avg;
}
}  // namespace average

int main() {
    // ========================================
    // simple test for lower
    // ========================================
    cout << 'Z' << " => " << lower('Z') << endl;
    cout << 'A' << " => " << lower('A') << endl;
    cout << 'a' << " => " << lower('a') << endl;
    cout << 'z' << " => " << lower('z') << endl;

    // ========================================
    // simple test for gcd
    // ========================================
    cout << gcd(1, 2) << endl;
    cout << gcd(2, 4) << endl;
    cout << gcd(12, 8) << endl;
    cout << gcd(7, 3) << endl;

    // ========================================
    // simple test for combination
    // ========================================
    cout << combination(5, 3) << endl;
    cout << combination(10, 3) << endl;
    
    // ========================================
    // simple test for linear_prime_sieve
    // ========================================
    auto primes = linear_prime_sieve(100);
    for(int i : primes) 
        cout << i << " ";
    cout << endl;

    // ========================================
    // simple test for average
    // ========================================
    vector<long long> nums{
        LLONG_MAX - 1, LLONG_MAX - 2, LLONG_MAX - 3, LLONG_MAX - 4, LLONG_MAX - 5
    };
    long long avg = average::v3(nums);
    cout << avg << " == " << LLONG_MAX - 3 << endl;
    
    nums = {
        LLONG_MIN + 1, LLONG_MIN + 2, LLONG_MIN + 3, LLONG_MIN + 4, LLONG_MIN + 5
    };
    avg = average::v3(nums);
    cout << avg << " == " << LLONG_MIN + 3 << endl;

    return 0;
}
