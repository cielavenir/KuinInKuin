int64_t n=\{n__.id};
if (n <= 1)
	return False;
if ((n & 1) == 0)
	return n == 2;
if (n <= 1920000)
{
	if (n == 3)
		return True;
	if (n % 6 != 1 && n % 6 != 5)
		return False;
	int64_t m = n / 6 * 2 + (n % 6 == 1 ? 0 : 1);
	size_t size;
	const uint8_t* primes_bin = GetPrimesBin(&size);
	return (primes_bin[m / 8] & (1 << (m % 8))) != 0;
}

// Miller-Rabin primality test.
uint64_t enough;
if (n < 2047)
	enough = 1;
else if (n < 1373653)
	enough = 2;
else if (n < 25326001)
	enough = 3;
else if (n < 3215031751)
	enough = 4;
else if (n < 2152302898747)
	enough = 5;
else if (n < 3474749660383)
	enough = 6;
else if (n < 341550071728321)
	enough = 7;
else if (n < 3825123056546413051)
	enough = 9;
else
{
	// n < 2^64 < 318665857834031151167461
	enough = 12;
}
{
	uint64_t d = (uint64_t)n - 1;
	uint64_t s = 0;
	while ((d & 1) == 0)
	{
		s++;
		d >>= 1;
	}
	for (uint64_t i = 0; i < enough; i++)
	{
		uint64_t x = ModPow(Primes[i], d, (uint64_t)n);
		uint64_t j;
		if (x == 1 || x == (uint64_t)n - 1)
			continue;
		bool probablyPrime = false;
		for (j = 0; j < s; j++)
		{
			x = ModPow(x, 2, (uint64_t)n);
			if (x == (uint64_t)n - 1)
			{
				probablyPrime = True;
				break;
			}
		}
		if (!probablyPrime)
			return False;
	}
	return True;
}
