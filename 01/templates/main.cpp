#include <cstdio>
#include <ctime>
#include <cmath>
#include <vector>

long naive_count(long N);
long simple_count(long N);
long basic_count(long N);
long cycle_count(long N);
long cache_count(long N);
long naive_sieve(long N);
long simple_sieve(long N);
long basic_sieve(long N);

//long prime_count(long N);
long (*prime_count)(long) = basic_count;

int main()
{
	long N = 1e7;
	printf("Calculating...\n");
	long t = clock();
	long primes = prime_count(N);
	t = clock() - t;
	printf("Primes in an interval [1:%g]: ", (double)N);
	printf("%ld\n", primes);
	printf("Calculated in %.2f s\n", ((float)t) / CLOCKS_PER_SEC);
}

long naive_count(long N)
{
	long total = 0;
	for (long i = 2; i <= N; i += 1)
	{
		total += 1;
		for (long j = 2; j < i; j += 1)
		{
			if (i % j == 0)
			{
				total -= 1; break;
			}
		}
	}
	return total;
}

long simple_count(long N)
{
	long total = 1; // Įskaitykime 2 iš anksto
	for (long i = 3; i <= N; i += 2) // Praleiskime lyginius skaičius
	{
		total += 1;
		for (long j = 3; j <= sqrt(i); j += 2) // Praleiskime lyginius
		{ // ...ir didelius daliklius
			if (i % j == 0)
			{
				total -= 1; break;
			}
		}
	}
	return total;
}

long basic_count(long N)
{
	long total = 1; // Įskaitykime 2 iš anksto
	for (long i = 3; i <= N; i += 2) // Praleiskime lyginius skaičius
	{
		total += 1; float sqi = sqrt(i);// Apskaičiuokime šaknį iš anksto
		for (long j = 3; j <= sqi; j += 2) // Praleiskime lyginius
		{ // ...ir didelius daliklius
			if (i % j == 0)
			{
				total -= 1; break;
			}
		}
	}
	return total;
}

/* Iš 30 skaičių tik 8 nesidalija iš 2, 3 arba 5 */
const int C = 2 * 3 * 5, D = 3, N = 8;
const int divs[D] = { 2, 3, 5 };
const int rems[N] = { 1, 7, 11, 13, 17, 19, 23, 29 };
const int gaps[N] = { +6, +4, +2, +4, +2, +4, +6, +2 };
const int poss[C] = { 0, 0, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 4,
4, 4, 4, 5, 5, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7 };

struct Cycle
{
	long pos, num;
	Cycle(long p = 1) : pos(p) { num = rems[pos]; }
	int next() { return gaps[(pos++) % N]; }
	void set(long n) { pos = poss[n % C]; num = n - n % C + rems[pos]; }
};

long cycle_count(long N)
{
	Cycle ci; long total = D; // Įskaitykime daliklius (2, 3, 5)
	for (long i = ci.num; i <= N; i += ci.next()) // Praleiskime kartotinius
	{
		Cycle cj; total += 1; float sqi = sqrt(i);
		for (long j = cj.num; j <= sqi; j += cj.next()) // -"-
		{
			if (i % j == 0)
			{
				total -= 1; break;
			}
		}
	}
	return total;
}

struct Cache
{
	Cycle cycle; // Ratukas kartotiniams praleisti
	std::vector<long> nums; // Rastų pirminių skaičių masyvas
	// Kelios funkcijos naudojimui supaprastinti
	void add(long num) { nums.push_back(num); }
	int size() { return nums.size(); }
	long operator[] (int i) { return nums[i]; }
	// Žinomus skaičius galima pridėti į atmintinę iš anksto
	Cache() : cycle() { for (auto i : divs) add(i); }
};

Cache cycle_cache(long N) {
	Cache cache;
	Cycle& ci = cache.cycle; long i;
	for (i = ci.num; i <= N; i += ci.next())
	{
		Cycle cj; bool isPrime = true; float sqi = sqrt(i);
		for (long j = cj.num; j <= sqi; j += cj.next())
		{
			if (i % j == 0)
			{
				isPrime = false; break;
			}
		} if (isPrime) cache.add(i);
	}
	ci.set(i); return cache;
}

long cache_count(long N) {
	Cache cache = cycle_cache(sqrt(N));
	Cycle& ci = cache.cycle; long total = cache.size();
	for (long i = ci.num; i <= N; i += ci.next())
	{
		total += 1; float sqi = sqrt(i); long j;
		for (int k = D; k < cache.size() && (j = cache[k]) <= sqi; ++k)
		{
			if (i % j == 0)
			{
				total -= 1; break;
			}
		}
	}
	return total;
}

long naive_sieve(long N)
{
	char* nums = new char[N + 1]; // Skaičius atitinka masyvo indeksai
	for (long n = 2; n <= N; n += 1)
		nums[n] = 1; // Pažymėkime visus skaičius
	for (long j = 2; j <= N; j += 1)
	{
		if (nums[j] == 1) // Sudėtinius skaičius galima praleisti
		{
			for (long i = j * 2; i <= N; i += j)
			{
				nums[i] = 0; // Išbraukime skaičiaus kartotinius
			}
		}
	}
	long total = 0;
	for (long n = 2; n <= N; n += 1)
		total += nums[n]; // Suskaičiuokime, kiek liko pažymėta
	delete[] nums; return total;
}

long simple_sieve(long N)
{
	char* nums = new char[N + 1];
	for (long n = 3; n <= N; n += 1)
		nums[n] = 1;
	for (long j = 3; j <= sqrt(N); j += 2)
	{
		if (nums[j] == 1)
		{
			for (long i = j * j; i <= N; i += j * 2)
			{
				nums[i] = 0;
			}
		}
	}
	long total = 1; // '2' yra pirminis
	for (long n = 3; n <= N; n += 2)
		total += nums[n];
	delete[] nums; return total;
}

long basic_sieve(long N)
{
	char* nums = new char[(N + 1) / 2]; // [0], [1], [2], ... -> 1, 3, 5, ...
	for (long n = 0; n < (N + 1) / 2; n += 1)
		nums[n] = 1;
	float sqN = sqrt(N);
	for (long j = 3; j <= sqN; j += 2)
	{
		if (nums[j / 2] == 1)
		{
			for (long i = j * j; i <= N; i += j * 2)
			{
				nums[i / 2] = 0;
			}
		}
	}
	long total = 0;
	for (long n = 0; n < (N + 1) / 2; n += 1)
		total += nums[n];
	delete[] nums; return total;
}
