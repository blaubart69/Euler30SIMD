// Euler30SIMD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

#define LLONG __int64
#define ULONGLONG unsigned __int64

// NOTE: n is the length of all 3 arrays
void saxpy_tsimd(LLONG a, int n, LLONG x[], LLONG y[], LLONG out[])
{
	using namespace tsimd;

	for (int i = 0; i < n; i += tsimd::vllong::static_size) {
		const vllong xi = load<vllong>(&x[i]);
		const vllong yi = load<vllong>(&y[i]);
		const vllong result = a * xi + yi; // same formula!
		store(result, &out[i]);
	}
}

void runsaxpy()
{
	printf("vllong::static_size: %d\n", tsimd::vllong::static_size);

	//LLONG x[4] = { 10, 10, 10, 10 };
	//LLONG y[4] = { 2, 3, 4, 5 };

	const int len = 10;

	LLONG z[len] = { 1, 2, 3, 4, 5, 6, 7, 8,  9, 10 };
	LLONG z2[len] = { 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
	LLONG o[len];
	LLONG a = 5;

	saxpy_tsimd(a, len, z, z2, o);

	for (int i = 0; i < len; ++i)
	{
		printf("i: %d\t%lld * %lld + %lld = %lld\n", i, a, z[i], z2[i], o[i]);;
	}
}

void loop8digits( )
{
	ULONGLONG d[8];
	for (d[0] = 0; d[0] < 10; ++d[0])
	{
		for (d[1] = 0; d[1] < 10; ++d[1])
		{
			for (d[2] = 0; d[2] < 10; ++d[2])
			{
				for (d[3] = 0; d[3] < 10; ++d[3])
				{
					for (d[4] = 0; d[4] < 10; ++d[4])
					{
						for (d[5] = 0; d[5] < 10; ++d[5])
						{
							for (d[6] = 0; d[6] < 10; ++d[6])
							{
								for (d[7] = 0; d[7] < 10; ++d[7])
								{

								}
							}
						}
					}
				}
			}
		}
	}
}

ULONGLONG euler30(void)
{
	using namespace tsimd;

	ULONGLONG POW5[] = {
		0 * 0 * 0 * 0 * 0
	,	1 * 1 * 1 * 1 * 1
	,	2 * 2 * 2 * 2 * 2
	,	3 * 3 * 3 * 3 * 3
	,	4 * 4 * 4 * 4 * 4
	,	5 * 5 * 5 * 5 * 5
	,	6 * 6 * 6 * 6 * 6
	,	7 * 7 * 7 * 7 * 7
	,	8 * 8 * 8 * 8 * 8
	,	9 * 9 * 9 * 9 * 9
	};

	ULONGLONG sum = 0;
	ULONGLONG num = 0;

	ULONGLONG _0to9[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	for (int a = 0; a < 10; ++a)
	{
		ULONGLONG powA = POW5[a];
		for (int b = 0; b < 10; ++b)
		{
			ULONGLONG SumAB = powA + POW5[b];
			for (int c = 0; c < 10; ++c)
			{
				ULONGLONG sumABC = SumAB + POW5[c];
				for (int d = 0; d < 10; ++d)
				{
					ULONGLONG SumABCD = sumABC + POW5[d];
					for (int e = 0; e < 10; ++e)
					{
						ULONGLONG SumABCDE = SumABCD + POW5[e];

						ULONGLONG numbers[10] = { num,num,num,num,num,num,num,num,num,num };
						
						ULONGLONG resultSum[10];
						//const vllong vBaseSum = load<vllong>(&baseSum[0]);

						for (int i = 0; i < 10; i += tsimd::vllong::static_size)
						{
							/*
							const vllong vNum     = load<vllong>(&numbers[i]);
							const vllong v0to9    = load<vllong>(&_0to9[i]);
							const vllong vNumbers = vNum + v0to9;
							*/

							const vllong vPow5    = load<vllong>(&POW5[i]);
							const vllong result   = (__int64)SumABCDE + vPow5;
							store(result, &resultSum[i]);

							/*
							auto vCmp = (result == vNumbers);
							int j = 0;
							for (auto iter = vCmp.begin(); iter != vCmp.end(); ++iter)
							{
								if (iter->value)
								{
									sum += num + j;
								}
								++j;
							}*/
						}

						
						for (int i = 0; i < 10; ++i)
						{
							if (num == resultSum[i])
							{
								printf("jucksi: %llu\n", num);
								sum += resultSum[i];
							}

							num++;
						}
					}
				}
			}
		}
	}
	sum -= 1;
	return sum;
}

int main()
{
	try
	{
		ULONGLONG result = euler30();
		printf("sum: %llu\n", result);
	}
	catch (std::exception ex)
	{
		printf("%s\n", ex.what());
	}
	printf("bye\n");
}

