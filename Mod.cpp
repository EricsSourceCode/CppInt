// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html

#include "Mod.h"
#include "Division.h"
// #include "Exponents.h"
#include "../CryptoBase/Euclid.h"
#include "../CppBase/StIO.h"




void Mod::reduce( Integer& result,
               const Integer& toReduce,
               const Integer& modulus,
               IntegerMath& intMath )
{
numbSys.reduce( result,
                toReduce,
                modulus,
                intMath );

}



void Mod::makeExact( Integer& exact,
                     const Integer& modulus,
                     IntegerMath& intMath )
{
// Most of the time the math is not exact,
// like in the modular exponentiation function
// toPower().  It leaves some small
// multiple of the modulus.  So this gets rid
// of that small left over multiple of the
// modulus.

Integer quotient;
Integer remainder;

Division::divide( exact, modulus,
                  quotient, remainder, intMath );

exact.copy( remainder );
}



// This is the standard modular power algorithm
// that you could find in any standard textbook,
// but its use of the new modular reduction
// algorithm is new (in 2015).

void Mod::toPower( Integer& result,
                   const Integer& exponent,
                   const Integer& modulus,
                   IntegerMath& intMath )
{
// StIO::putS( "toPower()." );

if( result.isZero())
  {
  return; // With Result still zero.
  }

if( result.isEqual( modulus ))
  {
  // It is congruent to zero % ModN.
  result.setToZero();
  return;
  }

// Result is not zero at this point.
if( exponent.isZero() )
  {
  result.setToOne();
  return;
  }

// Integer quotient;
// Integer remainder;

if( modulus.paramIsGreater( result ))
  {
  makeExact( result, modulus, intMath );
  }


if( exponent.isOne())
  return;

Integer X;
Integer expCopy;
Integer temp;

// =====
// Notice that if the number being raised to
// a power, which is assigned to X at this
// point, stays the same, but the exponent
// is different, then the X values can be
// precalculated once, like they are in
// Exponents.cpp.  That separates the
// Double from the Multiply in the
// Double-and-Multiply algorithm.

// The PlainText (or CipherText) is what
// is assigned to X here.  The exponent
// is the unknown.

// For testing:
// Exponents exponTest;
// Integer testResult;
// testResult.copy( result );

// exponTest.toPower( testResult,
//                    exponent,
//                    modulus,
//                    intMath );

X.copy( result );


expCopy.copy( exponent );

// CharBuf showBuf;
// StIO::putS( "expCopy:" );
// intMath.toString10( expCopy, showBuf );
// StIO::putCharBuf( showBuf );
// StIO::putLF();

result.setToOne();

Int32 howBig = 0;

// StIO::putS( "toPower() loop." );

// For each bit.
while( true )
  {
  if( (expCopy.getD( 0 ) & 1) == 1 )
    {
    intMath.multiply( result, X );

    // temp is the result of this
    // reduce() operation.
    reduce( temp, result, modulus, intMath );
    result.copy( temp );
    }

  // StIO::putS( "expCopy:" );
  // intMath.toString10( expCopy, showBuf );
  // StIO::putCharBuf( showBuf );
  // StIO::putLF();

  expCopy.shiftRight( 1 ); // Divide by 2.
  if( expCopy.isZero())
    break;


  intMath.multiply( X, X );
  // intMath.square( X );
  reduce( temp, X, modulus, intMath );
  X.copy( temp );
  }

// When reduce() gets called it multiplies a base
// number by a uint sized digit.  So that can
// make the result one digit bigger than the
// modulus.  Then when they are added up
// you can get carry bits that can make it a
// little bigger.

howBig = result.getIndex() - modulus.getIndex();
// if( howBig > 1 )
  // throw "This does happen.";

if( howBig > 2 )
  throw "This never happens yet. howBig.";

reduce( temp, result, modulus, intMath );
result.copy( temp );

// Notice that this Divide() is done once.
// Not a thousand or two thousand times.
makeExact( result, modulus, intMath );

// if( !testResult.isEqual( result ))
  // throw "Exponents test is bad.";

}



void Mod::verifyInBaseRange(
                     const Integer& toCheck,
                     const Integer& modulus,
                     const char* errorS )
{
if( toCheck.getNegative() )
  {
  StIO::putS( errorS );
  throw "mod.verifyInBaseRange() is negative.";
  }

if( modulus.paramIsGreaterOrEq( toCheck ))
  {
  StIO::putS( errorS );
  throw
     "mod.verifyInBaseRange() toCheck >= modulus";
  }
}



void Mod::verifyMoreThanZero(
                        const Integer& toCheck )
{
if( toCheck.getNegative() )
  throw "mod.verifyMoreThanZero() is negative.";

if( toCheck.isZero() )
  throw "mod.verifyMoreThanZero() is zero.";

}



void Mod::add( Integer& result,
               const Integer& toAdd,
               const Integer& modulus,
               IntegerMath& intMath )
{
verifyInBaseRange( result, modulus,
                          "Mod.add() result" );
verifyInBaseRange( toAdd, modulus,
                          "Mod.add() toAdd" );

// Integer temp;

intMath.add( result, toAdd );

// numbSys.reduce( temp, result, modulus, intMath );
// result.copy( temp );
makeExact( result, modulus, intMath );
}


void Mod::addL( Integer& result,
                const Int64 toAdd,
                const Integer& modulus,
                IntegerMath& intMath )
{
if( toAdd == 0 )
  return;

verifyInBaseRange( result, modulus,
                    "Mod.addL() result" );

// Integer temp;

result.addLong48( toAdd );

// numbSys.reduce( temp, result, modulus, intMath );
// result.copy( temp );
makeExact( result, modulus, intMath );
}



void Mod::negate( Integer& result,
                  const Integer& modulus,
                  IntegerMath& intMath )
{
// If y was 1 and base was 5.
// 1 + (-1) = 0;
// y = -1 + 5 = 4.
// 1 + 4 = 0 mod 5.
// The definition of the word 'negate':
// y + negate( y ) = 0.

verifyInBaseRange( result, modulus,
                        "Mod.negate() result" );

// Since it was checked to see if it's in
// the base range, it is not negative.

result.setNegative( true );
// Adding a positive number to a negative
// number to make it positive.
intMath.add( result, modulus );
}



void Mod::subtract( Integer& result,
                    const Integer& toSub,
                    const Integer& modulus,
                    IntegerMath& intMath )
{
verifyInBaseRange( result, modulus,
                  "Mod.Subtract() result" );
verifyInBaseRange( toSub, modulus,
                      "Mod.Subtract() toSub" );


intMath.subtract( result, toSub );
if( result.getNegative())
  intMath.add( result, modulus );

// This add() can't add a negative number.
//  result.add( modulus );

verifyInBaseRange( result, modulus,
               "Mod.Subtract() result at end." );

// numbSys.reduce( temp, result, modulus, intMath );
// result.copy( temp );
// makeExact( result, modulus, intMath );
}


/*
void Mod::subtractL( Integer& result,
                     const Int64 toSub,
                     const Integer& modulus,
                     IntegerMath& intMath )
{
verifyInBaseRange( result, modulus,
                      "Mod.SubtractL() result" );

result.subLong56( toSub );
//======
if( result.getNegative())
  intMath.add( result, modulus );

// This add() can't add a negative number.
//  result.add( modulus );

verifyInBaseRange( result, modulus,
          "Mod.SubtractL() result at end." );

// numbSys.reduce( temp, result, modulus, intMath );
// result.copy( temp );
// makeExact( result, modulus, intMath );
}
*/


void Mod::multiply( Integer& result,
                    const Integer& toMul,
                    const Integer& modulus,
                    IntegerMath& intMath )
{
verifyInBaseRange( result, modulus,
                     "Mod.Multiply() result" );
verifyInBaseRange( toMul, modulus,
                     "Mod.Multiply() toMul" );

intMath.multiply( result, toMul );

Integer temp;

reduce( temp, result, modulus, intMath );
result.copy( temp );
makeExact( result, modulus, intMath );
}


void Mod::multiplyL( Integer& result,
                      const Int64 toMul,
                      const Integer& modulus,
                      IntegerMath& intMath )
{
verifyInBaseRange( result, modulus,
                    "Mod.MultiplyL() result" );
// verifyInBaseRange( toMul, modulus );

intMath.multiplyLong48( result, toMul );

Integer temp;
reduce( temp, result, modulus, intMath );
result.copy( temp );
makeExact( result, modulus, intMath );
}



/*
void Mod::square( Integer& result,
                  const Integer& modulus,
                  IntegerMath& intMath )
{
verifyInBaseRange( result, modulus,
                        "Mod.square() result" );

intMath.square( result );

Integer temp;

reduce( temp, result, modulus, intMath );
result.copy( temp );
makeExact( result, modulus, intMath );
}
*/



bool Mod::divide( Integer& result,
                  const Integer& numerator,
                  const Integer& divisor,
                  const Integer& modulus,
                  Integer& gcd,
                  IntegerMath& intMath )
{
verifyInBaseRange( divisor, modulus,
                       "Mod.divide() divisor" );
verifyInBaseRange( numerator, modulus,
                    "Mod.divide() numerator" );
verifyMoreThanZero( divisor );

if( numerator.isZero())
  {
  result.setToZero();
  return true;
  }

if( divisor.isOne())
  {
  result.copy( numerator );
  return true;
  }

// The definition of multiplicative inverse:
// X * multInverse == 1.
// You can't have a multiplicative inverse
// of zero.  0 * anything = 0, so it can't
// equal 1.

Integer inverse;

// Get the multiplicative inverse.
if( !Euclid::multInverse( divisor,
                          modulus,
                          inverse,
                          gcd,
                          intMath ))
  return false;

verifyInBaseRange( result, modulus,
                "Mod.divide() result at end" );

result.copy( numerator );
intMath.multiply( result, inverse );

Integer temp;

reduce( temp, result, modulus, intMath );
result.copy( temp );
makeExact( result, modulus, intMath );
return true;
}
