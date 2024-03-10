/*


// Copyright Eric Chauvin 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "Int112.h"
#include "../CppBase/StIO.h"
#include "../CppBase/Casting.h"
#include "../CryptoBase/Randomish.h"


void Int112::copy( const Int112& from )
{
N0.copy( from.N0 );
N1.copy( from.N1 );
N2.copy( from.N2 );
N3.copy( from.N3 );
}



void Int112::setFromCharBufBigEnd(
                        const CharBuf& cBuf )
{
Int32 where = 0;
const Int32 last = cBuf.getLast();
for( Int32 count = last - 1; count >= 0;
                                   count-- )
  {
  Uint8 aByte = cBuf.getU8( where );
  where++;
  setByteAt( count, aByte );
  }
}



Int64 Int112::getMod( const Int64 x ) const
{
if( x == 0 )
  throw "Int112 getMod() x == 0.";

if( N1.isZero())
  return N0.getMod( x );

// If you had a base-10 number and you
// wanted to find the remainder when
// divided by a single digit from
// 1 to 9 then you would be doing
// this same thing.
// 7123 divided by 6 is
// 7 / 6 leaves remainder 1.
// 1123 / 6 is
// 11 / 6 leaves remainder 5.
// 523 / 6 is
// 52 / 6 leaves remainder 4.
// 43 / 6 leave remainder 1.

// N1 and N0 are 56 bit digits.
// 7 bytes.
// 0x1  00 00 00  00 00 00 00
// The number is N1 * 0x100000000000000LL
//               + N0.

// If x was 2, or some power of 2, then
// result would be zero here.
Int64 result = Int56::Base56 % x;

result = result * N1.getMod( x );
result = result % x;
result += N0.getMod( x );
result = result % x;
return result;
}



bool Int112::makeRandomOdd(
                const Int32 howManyBytes )
{
if( howManyBytes > 14 )
  throw "Int112 makeRandomOdd too big.";

setZero();

CharBuf cBuf;
// More than enough random bytes.
Randomish::makeRandomBytes( cBuf,
                            howManyBytes + 100 );

CharBuf bigEndBytes;
for( Int32 count = 0; count < howManyBytes;
                               count++ )
  {
  bigEndBytes.appendU8( cBuf.getU8( count ));
  }

setFromCharBufBigEnd( bigEndBytes );

// Make it odd.
Uint8 aByte = N0.getByteAt( 0 );
aByte |= 1;
N0.setByteAt( 0, aByte );

// if( isZero())
//  throw "random odd is zero.";

return true;
}



void Int112::setByteAt( const Int32 where,
                        const Uint8 toSet )
{
if( where >= 14 )
  throw "Int112::setByteAt where <= 14.";

if( where <= 6 )
  N0.setByteAt( where, toSet );
else
  N1.setByteAt( where % 7, toSet );

}



Int32 Int112::isDivisibleBySmallPrime(
                const SPrimes& sPrimes ) const
{
if( isEven())
  return 2;

if( N1.isZero())
  return N0.isDivisibleBySmallPrime( sPrimes );

for( Int32 count = 1; count <
             SPrimes::primesArraySize; count++ )
  {
  Int32 prime = sPrimes.getPrimeAt( count );
  if( 0 == getMod( prime ))
    return prime;

  }

// No small primes divide it.
return 0;
}


void Int112::setFromFullInt56(
                       const Int56& setFrom )
{
Int56 low;
Int56 high;
setFrom.getLowPart( low );
setFrom.getHighPart( high );

Int64 low64 = low.getAsLong();
Int64 high64 = high.getAsLong();
N0.setFromLong( low64 );
N1.setFromLong( high64 );
N2.setZero();
N3.setZero();
}



bool Int112::isEqual( const Int112& x ) const
{
if( !N0.isEqual( x.N0 ))
  return false;

if( !N1.isEqual( x.N1 ))
  return false;

if( !N2.isEqual( x.N2 ))
  return false;

if( !N3.isEqual( x.N3 ))
  return false;

return true;
}



bool Int112::paramIsGreater(
                      const Int112& x ) const
{
if( N1.paramIsGreater( x.N1 ))
  return true;

if( x.N1.paramIsGreater( N1 ))
  return false;

// N1s are equal here.
if( N0.paramIsGreater( x.N0 ))
  return true;

// They might be equal.
return false;
}



bool Int112::paramIsGreaterOrEq(
                    const Int112& x ) const
{
if( N1.paramIsGreater( x.N1 ))
  return true;

if( x.N1.paramIsGreater( N1 ))
  return false;

// N1s are equal here.
if( N0.paramIsGreaterOrEq( x.N0 ))
  return true;

return false;
}



void Int112::multiply( const Int112& x,
                       const Int112& y )
{
Int56 low;
Int56 mid1;
Int56 mid2;
Int56 high;

// Multiplying 56 bit numbers.

low.multiply( x.N0, y.N0 );

low.getLowPart( N0 );

if( x.N1.isZero() && y.N1.isZero() )
  {
  low.getHighPart( N1 );
  N2.setZero();
  N3.setZero();
  return;
  }

Int56 carry;
Int56 mid;
Int56 high2;

low.getHighPart( carry );

if( y.N1.isZero())
  {
  // mid1 = 0;
  mid2.multiply( x.N1, y.N0 );
  // high = 0;
  // mid = carry + mid1 + mid2;
  mid.copy( carry );
  mid.add( mid2 );

  mid.getLowPart( N1 );
  mid.getHighPart( carry );
  carry.getLowPart( N2 );
  carry.getHighPart( N3 );
  return;
  }

if( x.N1.isZero())
  {
  mid1.multiply( x.N0, y.N1 );
  // mid2 = 0; // x.N1 * y.N0;
  // high = 0; // x.N1 * y.N1;
  low.getHighPart( carry );
  // mid = carry + mid1 + mid2;
  mid.copy( carry );
  mid.add( mid1 );
  mid.getLowPart( N1 );
  mid.getHighPart( carry );
  carry.getLowPart( N2 );
  carry.getHighPart( N3 );
  return;
  }

mid1.multiply( x.N0, y.N1 );
mid2.multiply( x.N1, y.N0 );
high.multiply( x.N1, y.N1 );

low.getHighPart( carry );

mid.copy( carry );
mid.add( mid1 );
mid.add( mid2 );

mid.getLowPart( N1 );
mid.getHighPart( carry );

high2.copy( carry );
high2.add( high );
high2.getLowPart( N2 );
high2.getHighPart( N3 );
}



void Int112::cleanCarry( void )
{
Int56 carry;
N0.getHighPart( carry );
N0.clearHighPart();

N1.add( carry );
N1.getHighPart( carry );
N1.clearHighPart();

N2.add( carry );
N2.getHighPart( carry );
N2.clearHighPart();

N3.add( carry );
N3.getHighPart( carry );
N3.clearHighPart();

if( !carry.isZero())
  throw "Int112.cleanCarry Overflow.";

}



void Int112::add( const Int112& toAdd )
{
N0.add( toAdd.N0 );
N1.add( toAdd.N1 );
N2.add( toAdd.N2 );
N3.add( toAdd.N3 );

cleanCarry();
}



void Int112::subtract( const Int112& toSub )
{
N0.subtract( toSub.N0 );
N1.subtract( toSub.N1 );
N2.subtract( toSub.N2 );
N3.subtract( toSub.N3 );

cleanBorrow();
}



void Int112::cleanBorrow( void )
{
if( N0.isNegative() )
  {
  N1.dec();
  N0.addBase56();
  }

if( N1.isNegative() )
  {
  N2.dec();
  N1.addBase56();
  }

if( N2.isNegative() )
  {
  N3.dec();
  N2.addBase56();
  }

// if( N3 < 0 )
  // It is negative.

}

*/

