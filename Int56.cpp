// Copyright Eric Chauvin 2023



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "Int56.h"
// #include <experimental/simd>
// namespace stdx = std::experimental;



void Int56::multiply( const Int56& x,
                      const Int56& y )
{
// Multiplying 28 bit numbers.

Int64 low =  x.N0 * y.N0;
N0 = low & BitMask28;

if((x.N1 == 0) && (y.N1 == 0))
  {
  N1 = (low >> 28) & BitMask28;
  N2 = 0;
  N3 = 0;
  return;
  }

Int64 mid1 = 0;
Int64 mid2 = 0;
Int64 high = 0;
Int64 carry = low >> 28;
Int64 mid = 0;

if( y.N1 == 0 )
  {
  // mid1 = 0;
  mid2 = x.N1 * y.N0;
  // high = 0;
  // mid = carry + mid1 + mid2;
  mid = carry + mid2;
  N1 = mid & BitMask28;
  carry = mid >> 28;
  // high2 = carry;
  N2 = carry  & BitMask28;
  N3 = (carry >> 28) & BitMask28;
    return;

  }

if( x.N1 == 0 )
  {
  mid1 = x.N0 * y.N1;
  // mid2 = 0; // x.N1 * y.N0;
  // high = 0; // x.N1 * y.N1;
  carry = low >> 28;
  // mid = carry + mid1 + mid2;
  mid = carry + mid1;
  N1 = mid & BitMask28;
  carry = mid >> 28;
  // high2 = carry; //  + high;
  N2 = carry  & BitMask28;
  N3 = (carry >> 28) & BitMask28;
  return;
  }

Int64 high2 = 0;

mid1 = x.N0 * y.N1;
mid2 = x.N1 * y.N0;
high = x.N1 * y.N1;

carry = low >> 28;

mid = carry + mid1 + mid2;
N1 = mid & BitMask28;
carry = mid >> 28;

high2 = carry + high;
N2 = high2 & BitMask28;

N3 = (high2 >> 28) & BitMask28;

carry = high2 >> 56;
if( carry != 0 )
  throw "Int56 Multiply overflow.";

}


bool Int56::divide( const Int56& toDivide,
                    const Int56& divideBy,
                    Int56& remainder )
{
Int64 toDiv = toDivide.getAsLong();
Int64 divBy = divideBy.getAsLong();

Int64 quotient = toDiv / divBy;
Int64 remain = toDiv % divBy;
remainder.setFromLong( remain );
setFromLong( quotient );

// ==========
Int56 testQuot;
testQuot.setFromLong( quotient );
Int56 testDiv;
testDiv.multiply( testQuot, divideBy );
testDiv.add( remainder );
if( !testDiv.isEqual( toDivide ))
  throw "Int56 divide is bad.";

// If it divides it exactly.
if( remain == 0 )
  return true;

return false;
}



Int32 Int56::isDivisibleBySmallPrime(
               const SPrimes& sPrimes ) const
{
if( isEven())
  return 2;

Int64 toDivide = N1;
toDivide <<= 28;
toDivide |= N0;

for( Int32 count = 1; count <
             SPrimes::primesArraySize; count++ )
  {
  Int32 prime = sPrimes.getPrimeAt( count );
  if( 0 == (toDivide % prime))
    return prime;

  }

// No small primes divide it.
return 0;
}
