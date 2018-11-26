// Euler30SIMD.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"

extern "C" int mike(void);
extern "C" int mike256(void);

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

constexpr int myPow(int num, int power)
{
	int pow = num;
	for (int i = 0; i < power - 1; ++i)
	{
		pow = pow * num;
	}

	return pow;
}

ULONGLONG g_powers[10];

template<typename InnerLoopCallback>
void loop8digits(InnerLoopCallback OnInnerLoop)
{
	int d[8];
	ULONGLONG sum[8];
	
	for (d[0] = 0; d[0] < 10; ++d[0])
	{
		sum[0] = g_powers[d[0]];
		for (d[1] = 0; d[1] < 10; ++d[1])
		{
			sum[1] = sum[0] + g_powers[d[1]];
			for (d[2] = 0; d[2] < 10; ++d[2])
			{
				sum[2] = sum[1] + g_powers[d[2]];
				for (d[3] = 0; d[3] < 10; ++d[3])
				{
					sum[3] = sum[2] + g_powers[d[3]];
					for (d[4] = 0; d[4] < 10; ++d[4])
					{
						sum[4] = sum[3] + g_powers[d[4]];
						for (d[5] = 0; d[5] < 10; ++d[5])
						{
							sum[5] = sum[4] + g_powers[d[5]];
							for (d[6] = 0; d[6] < 10; ++d[6])
							{
								sum[6] = sum[5] + g_powers[d[6]];
								for (d[7] = 0; d[7] < 10; ++d[7])
								{
									sum[7] = sum[6] + g_powers[d[7]];
									OnInnerLoop(sum[7]);
								}
							}
						}
					}
				}
			}
		}
	}
}
template<typename InnerLoopCallback>
void loop6digits(InnerLoopCallback OnInnerLoop)
{
	int d[8];
	ULONGLONG sum[8];

	for (d[0] = 0; d[0] < 10; ++d[0])
	{
		sum[0] = g_powers[d[0]];
		for (d[1] = 0; d[1] < 10; ++d[1])
		{
			sum[1] = sum[0] + g_powers[d[1]];
			for (d[2] = 0; d[2] < 10; ++d[2])
			{
				sum[2] = sum[1] + g_powers[d[2]];
				for (d[3] = 0; d[3] < 10; ++d[3])
				{
					sum[3] = sum[2] + g_powers[d[3]];
					for (d[4] = 0; d[4] < 10; ++d[4])
					{
						sum[4] = sum[3] + g_powers[d[4]];
						OnInnerLoop(sum[4]);
					}
				}
			}
		}
	}
}

inline ULONGLONG v0(void)
{
	ULONGLONG sum = 0;
	ULONGLONG num = 0;

	loop8digits([&sum, &num](ULONGLONG outerSum)
	{
		for (int i = 0; i < 10; ++i)
		{
			if ( outerSum + g_powers[i] == num )
			{
				printf("%llu\n", num);
				sum += num;
			}
			++num;
		}
	});

	return sum-1;
}

inline ULONGLONG v2_powerX(void)
{
	ULONGLONG sum = 0;
	ULONGLONG num10 = 0;
	const ULONGLONG _0to9[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

	using namespace tsimd;

	loop8digits([&sum, &_0to9, &num10](ULONGLONG outerSum)
	{
		ULONGLONG numbers[10] = { num10,num10,num10,num10,num10,num10,num10,num10,num10,num10 };

		int j = 0;
		ULONGLONG resultSum[16];
		bool found = false;

		for (int i = 0; i < 10; i += tsimd::vllong::static_size)
		{
			const vllong vNum = load<vllong>(&numbers[i]);
			const vllong v0to9 = load<vllong>(&_0to9[i]);
			const vllong vNumbers = vNum + v0to9;

			const vllong vPow5 = load<vllong>(&g_powers[i]);
			const vllong result = (__int64)outerSum + vPow5;

			auto vCmp = (result == vNumbers);
			if (tsimd::any(vCmp))
			{
				store(result, &resultSum[i]);
				found = true;
			}
		}

		if (found)
		{
			ULONGLONG currNum = num10;
			for (int i = 0; i < 10; ++i)
			{
				if (currNum == resultSum[i])
				{
					printf("%llu\n", currNum);
					sum += resultSum[i];
				}
				++currNum;
			}
		}

		num10 += 10;
	});

	return sum-1;
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
	ULONGLONG num10 = 0;
	ULONGLONG foundCount = 0;

	const ULONGLONG _0to9[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

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

						ULONGLONG numbers[10] = { num10,num10,num10,num10,num10,num10,num10,num10,num10,num10 };
						
						int j = 0;
						ULONGLONG resultSum[16];
						bool found = false;

						for (int i = 0; i < 10; i += tsimd::vllong::static_size)
						{
							const vllong vNum     = load<vllong>(&numbers[i]);
							const vllong v0to9    = load<vllong>(&_0to9[i]);
							const vllong vNumbers = vNum + v0to9;

							const vllong vPow5    = load<vllong>(&POW5[i]);
							const vllong result   = (__int64)SumABCDE + vPow5;
							
							auto vCmp = (result == vNumbers);
							if (tsimd::any(vCmp))
							{
								store(result, &resultSum[i]);
								found = true;
								++foundCount;
							}
						}

						if (found)
						{
							ULONGLONG currNum = num10;
							for (int i = 0; i < 10; ++i)
							{
								if (currNum == resultSum[i])
								{
									printf("%llu\n", currNum);
									sum += resultSum[i];
								}
								++currNum;
							}
						}

						num10 += 10;
					}
				}
			}
		}
	}

	printf("foundCount: %llu\n", foundCount);

	sum -= 1;
	return sum;
}

int main()
{
	try
	{
		/*
		int power = 8;

		for (int i = 0; i < 10; ++i)
		{
			g_powers[i] = myPow(i, power);
		}

		//ULONGLONG result = euler30();
		//ULONGLONG result = v2_powerX();
		ULONGLONG result = v0();
		printf("sum: %llu\n", result);
		*/

		mike();
		mike256();
	}
	catch (std::exception ex)
	{
		printf("%s\n", ex.what());
	}
	printf("bye\n");
}

