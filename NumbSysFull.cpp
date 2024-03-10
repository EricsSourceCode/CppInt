/*

// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "NumbSysFull.h"
#include "Division.h"



#include "../CppMem/MemoryWarnTop.h"


NumbSysFull::NumbSysFull( void )
{
intAr = new Integer[last];
intArQ = new Integer[last];
}


NumbSysFull::NumbSysFull( const NumbSysFull& in )
{
intAr = new Integer[last];
intArQ = new Integer[last];

if( in.testForCopy )
  return;

throw "Copy constructor for NumbSysFull.";
}


NumbSysFull::~NumbSysFull( void )
{
delete[] intAr;
delete[] intArQ;
}



void NumbSysFull::setupBaseArray(
                        const Integer& setBase,
                        IntegerMath& intMath )
{
StIO::putS( "NumbSysFull.setupBaseArray top" );

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
  intArQ[count].copy( quotient );

  CharBuf showBuf;
  StIO::putS( "Quotient" );
  intMath.toString10( quotient, showBuf );
  StIO::putCharBuf( showBuf );
  StIO::putLF();

  // Done at the bottom for the next round of
  // the loop.
  // Use the full base value.  Not just the
  // remainder.
  // baseValue.copy( remainder );

  intMath.multiply( baseValue, base2 );
  }

// StIO::putS( "setupBaseArray bottom" );
}



void NumbSysFull::reduce( Integer& result,
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
  accumRow.multiply28( val );

  result.add( accumRow );
  }

// StIO::putS( "reduce bottom" );
}



#include "../CppMem/MemoryWarnBottom.h"
*/

