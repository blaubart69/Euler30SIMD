#include <immintrin.h>

#define POW(a) (a*a*a*a*a*a*a*a*a)

static __m256i p10[10];

int checkForHit(__m256i equ, __m256i seq, int count)
{
	for (int i = 7; i >= 0; --i)
	{
		if (equ.m256i_u32[i] != 0)
		{
			printf("%lu\n", seq.m256i_u32[i]);
		}
	}
}

int mike256_10digits(void) {

	__m256i ten, equ, sum;
	__m256i seq76543210, seqXXXXXX98;
	__m256i pow76543210, powXXXXXX98;

	int a, b, c, d, e, f, g, h, i;

	p10[0] = _mm256_set1_epi32(POW(1) - POW(0));
	p10[1] = _mm256_set1_epi32(POW(2) - POW(1));
	p10[2] = _mm256_set1_epi32(POW(3) - POW(2));
	p10[3] = _mm256_set1_epi32(POW(4) - POW(3));
	p10[4] = _mm256_set1_epi32(POW(5) - POW(4));
	p10[5] = _mm256_set1_epi32(POW(6) - POW(5));
	p10[6] = _mm256_set1_epi32(POW(7) - POW(6));
	p10[7] = _mm256_set1_epi32(POW(8) - POW(7));
	p10[8] = _mm256_set1_epi32(POW(9) - POW(8));
	p10[9] = _mm256_set1_epi32(POW(0) - POW(9));

	ten = _mm256_set1_epi32(10);
	sum = _mm256_setzero_si256();

	seq76543210 = _mm256_set_epi32(0, 1, 2, 3, 4, 5, 6, 7);
	pow76543210 = _mm256_set_epi32(0, 1, POW(2), POW(3), POW(4), POW(5), POW(6), POW(7));

	seqXXXXXX98 = _mm256_set_epi32(8, 9, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000, 0x80000000);
	powXXXXXX98 = _mm256_set_epi32(POW(8), POW(9), 0, 0, 0, 0, 0, 0);

	for (a = 0; a < 10; a++) {
	for (b = 0; b < 10; b++) {
	for (c = 0; c < 10; c++) {
	for (d = 0; d < 10; d++) {
	for (e = 0; e < 10; e++) {
	for (f = 0; f < 10; f++) {
	for (g = 0; g < 10; g++) {
	for (h = 0; h < 10; h++) {
	for (i = 0; i < 10; i++) {
		equ = _mm256_cmpeq_epi32(seq76543210, pow76543210);
		//checkForHit(equ, seq76543210, 8);

		equ = _mm256_and_si256(equ, pow76543210);
		sum = _mm256_add_epi32(sum, equ);
		seq76543210 = _mm256_add_epi32(seq76543210, ten);
		pow76543210 = _mm256_add_epi32(pow76543210, p10[i]);

		equ = _mm256_cmpeq_epi32(seqXXXXXX98, powXXXXXX98);
		//checkForHit(equ, seqXXXXXX98, 2);

		equ = _mm256_and_si256(equ, powXXXXXX98);
		sum = _mm256_add_epi32(sum, equ);
		seqXXXXXX98 = _mm256_add_epi32(seqXXXXXX98, ten);
		powXXXXXX98 = _mm256_add_epi32(powXXXXXX98, p10[i]);
	}
	pow76543210 = _mm256_add_epi32(pow76543210, p10[h]);
	powXXXXXX98 = _mm256_add_epi32(powXXXXXX98, p10[h]);
	}
	pow76543210 = _mm256_add_epi32(pow76543210, p10[g]);
	powXXXXXX98 = _mm256_add_epi32(powXXXXXX98, p10[g]);
	}
	pow76543210 = _mm256_add_epi32(pow76543210, p10[f]);
	powXXXXXX98 = _mm256_add_epi32(powXXXXXX98, p10[f]);
	}
	pow76543210 = _mm256_add_epi32(pow76543210, p10[e]);
	powXXXXXX98 = _mm256_add_epi32(powXXXXXX98, p10[e]);
	}
	pow76543210 = _mm256_add_epi32(pow76543210, p10[d]);
	powXXXXXX98 = _mm256_add_epi32(powXXXXXX98, p10[d]);
	}
	pow76543210 = _mm256_add_epi32(pow76543210, p10[c]);
	powXXXXXX98 = _mm256_add_epi32(powXXXXXX98, p10[c]);
	}
	pow76543210 = _mm256_add_epi32(pow76543210, p10[b]);
	powXXXXXX98 = _mm256_add_epi32(powXXXXXX98, p10[b]);
	}
	pow76543210 = _mm256_add_epi32(pow76543210, p10[a]);
	powXXXXXX98 = _mm256_add_epi32(powXXXXXX98, p10[a]);
	}

	printf("256_10digits: %lu\n",
		sum.m256i_u32[0]
		+ sum.m256i_u32[1]
		+ sum.m256i_u32[2]
		+ sum.m256i_u32[3]
		+ sum.m256i_u32[4]
		+ sum.m256i_u32[5]
		+ sum.m256i_u32[6]
		+ sum.m256i_u32[7]
		- 1);

	return 0;
}
