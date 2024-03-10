// Copyright Eric Chauvin 2021 - 2023.



#include "Exponents.h"



#include "../CppMem/MemoryWarnTop.h"



Exponents::Exponents( void )
{
intAr = new Integer[last];
}



Exponents::Exponents( const Exponents& in )
{
intAr = new Integer[last];

if( in.testForCopy )
  return;

throw "Exponents copy constructor.";
}


Exponents::~Exponents( void )
{
delete[] intAr;
}



void Exponents::setupBases( const Integer& base,
                   const Integer& modulus,
                   IntegerMath& intMath )
{
// mainIO.appendChars( "Setting bases.\n" );

if( base.isEqual( modulus ))
  throw "Base = modulus in setupBase().";

currentBase.copy( base );
currentModulus.copy( modulus );

if( currentModulus.paramIsGreater( currentBase ))
  {
  mod.makeExact( currentBase, currentModulus,
                                      intMath );
  }

Integer X;
Integer temp;

// X starts out equal to current base for
// the index at bit zero.
X.copy( currentBase );

// In toPower the result value starts out as
// one.  So the first base value, at an index of
// zero, gets multiplied by 1.

// For each bit of the exponent.
for( Int32 count = 0; count < last; count++ )
  {
  intAr[count].copy( X );

  intMath.multiply( X, X );
  // intMath.square( X );

  // Reduce it first to speed up the division
  // in makeExact().
  mod.reduce( temp, X,
                    currentModulus, intMath );
  X.copy( temp );
  mod.makeExact( X, currentModulus, intMath );
  }
}



void Exponents::toPower( Integer& result,
                   const Integer& exponent,
                   const Integer& modulus,
                   IntegerMath& intMath )
{
// Notice that this has precedence over
// if the exponent is zero, which comes later.
if( result.isZero())
  return; // With Result still zero.

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

if( modulus.paramIsGreater( result ))
  mod.makeExact( result, modulus, intMath );

if( exponent.isOne())
  {
  // Result stays the same.
  return;
  }

// Only set up the base values when they
// change to something different.
if( !(currentBase.isEqual( result ) &&
      currentModulus.isEqual( modulus )))
  {
  setupBases( result, modulus, intMath );
  }

Integer expCopy;
Integer temp;

expCopy.copy( exponent );

// Notice how setting this to one makes it
// multiply 1 with the first base value at
// index zero.
result.setToOne();

Integer oneBase;

// For each bit.
for( Int32 count = 0; count < last; count++ )
  {
  if( (expCopy.getD( 0 ) & 1) == 1 )
    {
    oneBase.copy( intAr[count] );
    if( oneBase.isZero())
      throw "oneBase is zero.";

    intMath.multiply( result, oneBase );
    if( result.isZero())
      throw "result is zero.";

    mod.reduce( temp, result, modulus, intMath );
    result.copy( temp );
    }

  expCopy.shiftRight( 1 ); // Divide by 2.
  if( expCopy.isZero())
    break;

  }

mod.reduce( temp, result, modulus, intMath );
result.copy( temp );

mod.makeExact( result, modulus, intMath );
}


#include "../CppMem/MemoryWarnBottom.h"
