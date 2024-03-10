// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


// A Number System used for Modular Reduction.

// As far as I know I invented this idea for
// doing fast modular reduction.



#include "NumbSys.h"
#include "Division.h"



#include "../CppMem/MemoryWarnTop.h"


NumbSys::NumbSys( void )
{
intAr = new Integer[last];
}


NumbSys::NumbSys( const NumbSys& in )
{
intAr = new Integer[last];

if( in.testForCopy )
  return;

throw "Copy constructor for NumbSys.";
}


NumbSys::~NumbSys( void )
{
delete[] intAr;
}


void NumbSys::setupBaseArray(
                        const Integer& setBase,
                        IntegerMath& intMath )
{
// StIO::putS( "setupBaseArray top" );

currentBase.copy( setBase );

Integer base2;
base2.setFromLong48(
                Integer::Int24BitMask + 1 );

Integer baseValue;
Integer quotient;
Integer remainder;

baseValue.setToOne();

for( Int32 count = 0; count < last; count++ )
  {
  Division::divide( baseValue, currentBase,
                    quotient, remainder,
                    intMath );

  intAr[count].copy( remainder );

  // Done at the bottom for the next round of
  // the loop.
  baseValue.copy( remainder );
  intMath.multiply( baseValue, base2 );
  }

// StIO::putS( "setupBaseArray bottom" );
}


// This gets called a lot!

void NumbSys::reduce( Integer& result,
                      const Integer& toReduce,
                      const Integer& modulus,
                      IntegerMath& intMath )
{
// StIO::putS( "reduce top" );

// currentBase would start out being set to
// zero, so it has to be set the first time
// it gets called.
if( !modulus.isEqual( currentBase ))
  setupBaseArray( modulus, intMath );

if( toReduce.paramIsGreater( currentBase ))
  {
  result.copy( toReduce );
  return;
  }

const Int32 ind = toReduce.getIndex();

Integer accumRow;

result.setToZero();
for( Int32 row = 0; row <= ind; row++ )
  {
  Int64 val = toReduce.getD( row );
  accumRow.copy( intAr[row] );
  accumRow.multiply24( val );

  result.add( accumRow );
  }

// StIO::putS( "reduce bottom" );
}



#include "../CppMem/MemoryWarnBottom.h"
