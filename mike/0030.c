#include <emmintrin.h>

#define POW(a) (a*a*a*a*a)

static __m128i p10[ 10 ];

int mike( void ) {

    __m128i ten, equ, sum;
    __m128i seq3210, seq7654, seqXX98;
    __m128i pow3210, pow7654, powXX98;
    int a, b, c, d, e, f;

    p10[ 0 ] = _mm_set1_epi32( POW(1)-POW(0));
    p10[ 1 ] = _mm_set1_epi32( POW(2)-POW(1));
    p10[ 2 ] = _mm_set1_epi32( POW(3)-POW(2));
    p10[ 3 ] = _mm_set1_epi32( POW(4)-POW(3));
    p10[ 4 ] = _mm_set1_epi32( POW(5)-POW(4));
    p10[ 5 ] = _mm_set1_epi32( POW(6)-POW(5));
    p10[ 6 ] = _mm_set1_epi32( POW(7)-POW(6));
    p10[ 7 ] = _mm_set1_epi32( POW(8)-POW(7));
    p10[ 8 ] = _mm_set1_epi32( POW(9)-POW(8));
    p10[ 9 ] = _mm_set1_epi32( POW(0)-POW(9));

    ten = _mm_set1_epi32( 10 );
    sum = _mm_setzero_si128();

    seq3210 = _mm_set_epi32( 0, 1, 2, 3 );
    seq7654 = _mm_set_epi32( 4, 5, 6, 7 );
    seqXX98 = _mm_set_epi32( 8, 9, 0x80000000, 0x80000000 );
    pow3210 = _mm_set_epi32( 0, 1, POW(2), POW(3));
    pow7654 = _mm_set_epi32( POW(4), POW(5), POW(6), POW(7));
    powXX98 = _mm_set_epi32( POW(8), POW(9), 0, 0 );
    
    for ( a = 0; a < 10; a++ ) {
        for ( b = 0; b < 10; b++ ) {
            for ( c = 0; c < 10; c++ ) {
                for ( d = 0; d < 10; d++ ) {
                    for ( e = 0; e < 10; e++ ) {
                        equ = _mm_cmpeq_epi32( seq3210, pow3210 );
                        equ = _mm_and_si128( equ, pow3210 );
                        sum = _mm_add_epi32( sum, equ );
                        seq3210 = _mm_add_epi32( seq3210, ten );
                        pow3210 = _mm_add_epi32( pow3210, p10[ e ] );

                        equ = _mm_cmpeq_epi32( seq7654, pow7654 );
                        equ = _mm_and_si128( equ, pow7654 );
                        sum = _mm_add_epi32( sum, equ );
                        seq7654 = _mm_add_epi32( seq7654, ten );
                        pow7654 = _mm_add_epi32( pow7654, p10[ e ] );

                        equ = _mm_cmpeq_epi32( seqXX98, powXX98 );
                        equ = _mm_and_si128( equ, powXX98 );
                        sum = _mm_add_epi32( sum, equ );
                        seqXX98 = _mm_add_epi32( seqXX98, ten );
                        powXX98 = _mm_add_epi32( powXX98, p10[ e ] );
                    }
                    pow3210 = _mm_add_epi32( pow3210, p10[ d ] );
                    pow7654 = _mm_add_epi32( pow7654, p10[ d ] );
                    powXX98 = _mm_add_epi32( powXX98, p10[ d ] );
                }
                pow3210 = _mm_add_epi32( pow3210, p10[ c ] );
                pow7654 = _mm_add_epi32( pow7654, p10[ c ] );
                powXX98 = _mm_add_epi32( powXX98, p10[ c ] );
            }
            pow3210 = _mm_add_epi32( pow3210, p10[ b ] );
            pow7654 = _mm_add_epi32( pow7654, p10[ b ] );
            powXX98 = _mm_add_epi32( powXX98, p10[ b ] );
        }
        pow3210 = _mm_add_epi32( pow3210, p10[ a ] );
        pow7654 = _mm_add_epi32( pow7654, p10[ a ] );
        powXX98 = _mm_add_epi32( powXX98, p10[ a ] );
    }

    printf( "128: %lu\n", sum.m128i_u32[ 0 ] + sum.m128i_u32[ 1 ] + sum.m128i_u32[ 2 ] + sum.m128i_u32[ 3 ] - 1 );

    return 0;
}
