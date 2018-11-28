#include <amp.h> 

using namespace concurrency; 

#define N 100000000
#define M 100

unsigned long int sum[ N ];  // calc 100 per GPU

unsigned long int mikeAMP( void ) {

    array_view<unsigned long int> asum( N, sum );
    asum.discard_data();
    
    parallel_for_each( asum.extent, [=]( index< 1 > idx ) restrict( amp )
        { 
            unsigned long int pow, seq, i, a, b;
            unsigned long int const x[] = { 0*0*0*0*0
                                          , 1*1*1*1*1
                                          , 2*2*2*2*2
                                          , 3*3*3*3*3
                                          , 4*4*4*4*4
                                          , 5*5*5*5*5
                                          , 6*6*6*6*6
                                          , 7*7*7*7*7
                                          , 8*8*8*8*8
                                          , 9*9*9*9*9
                                          , 0*0*0*0*0
                                          };

            i = idx[ 0 ];
            seq = i * M;
            pow = 0;

            while ( i > 0 ) {
                pow += x[ i % 10 ]; i /= 10;
            } /* endwhile */

            for ( a = 0; a < 10; a++ ) {
                for ( b = 0; b < 10; b++ ) {
                    i += ( seq == pow ) ? seq : 0;
                    seq++;
                    pow += x[ b + 1 ] - x [ b ];
                } /* endfor */
                pow += x[ a + 1 ] - x[ a ];
            } /* endfor */

            asum[ idx ] = i;
        }
    );

    for ( unsigned int i = 1; i < N; i++ ) {
        asum(0) += asum(i);
    } /* endfor */

	return asum(0) - 1;
}
constexpr unsigned long int calc_pow(unsigned long int n, int power)
{
	unsigned long int pow = n;
	for (int i = 0; i < power - 1; ++i)
	{
		pow = pow * n;
	}

	return pow;
}
/*
unsigned long int const x[] = { 0 * 0 * 0 * 0 * 0
							  , 1 * 1 * 1 * 1 * 1
							  , 2 * 2 * 2 * 2 * 2
							  , 3 * 3 * 3 * 3 * 3
							  , 4 * 4 * 4 * 4 * 4
							  , 5 * 5 * 5 * 5 * 5
							  , 6 * 6 * 6 * 6 * 6
							  , 7 * 7 * 7 * 7 * 7
							  , 8 * 8 * 8 * 8 * 8
							  , 9 * 9 * 9 * 9 * 9
							  , 0 * 0 * 0 * 0 * 0
};
*/

unsigned long int mikeAMP(int power) {

	array_view<unsigned long int> asum(N, sum);
	asum.discard_data();

	unsigned long int x[11];
	for (int i = 0; i < 10; ++i)
	{
		x[i] = calc_pow(i, power);
	}
	x[10] = 0;

	parallel_for_each(asum.extent, [=](index< 1 > idx) restrict(amp)
	{
		unsigned long int pow, seq, i, a, b;

		i = idx[0];
		seq = i * M;
		pow = 0;

		while (i > 0) {
			pow += x[i % 10]; i /= 10;
		} /* endwhile */

		for (a = 0; a < 10; a++) {
			for (b = 0; b < 10; b++) {
				i += (seq == pow) ? seq : 0;
				seq++;
				pow += x[b + 1] - x[b];
			} /* endfor */
			pow += x[a + 1] - x[a];
		} /* endfor */

		asum[idx] = i;
	}
	);

	for (unsigned int i = 1; i < N; i++) {
		asum(0) += asum(i);
	} /* endfor */

	return asum(0) - 1;
}
