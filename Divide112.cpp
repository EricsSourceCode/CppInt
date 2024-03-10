/*


// Copyright Eric Chauvin 2023.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "Divide112.h"
#include "../CppBase/StIO.h"



bool Divide112::divide( const Int112& toDivide,
                     const Int112& divideBy,
                     Int112& quotient,
                     Int112& remainder )
{
if( divideBy.isZero())
  throw "Divide112 divide() divideBy.isZero()";

if( divideBy.isOne())
  {
  StIO::putS( "divideBy is one." );
  quotient.copy( toDivide );
  remainder.setZero();
  return true;
  }

if( toDivide.isEqual( divideBy ))
  {
  StIO::putS( "toDivide equals divideBy." );
  quotient.setOne();
  remainder.setZero();
  return true;
  }

if( toDivide.isZero())
  {
  StIO::putS( "toDivide is zero." );
  quotient.setZero();
  remainder.setZero();
  return false; // divideBy is not zero.
  }

quotient.setZero();

if( toDivide.paramIsGreater( divideBy ))
  {
  StIO::putS( "toDivide paramGreater." );

  remainder.copy( toDivide );
  return false;
  }

if( toDivide.N1.isZero())
  {
  StIO::putS( "toDivide.N1.isZero." );

  // Then divideBy.N1 is zero here too.

  Int56 quotient56;
  Int56 remainder56;

  quotient56.divide( toDivide.N0,
                     divideBy.N0,
                     remainder56 );

   quotient.N0.copy( quotient56 );
   quotient.N1.setZero();
   quotient.N2.setZero();
   quotient.N3.setZero();

   remainder.N1.setZero();
   remainder.N0.copy( remainder56 );

  if( remainder56.isZero())
    return true;

  return false;
  }

if( divideBy.N1.isZero())
  {
  StIO::putS( "divideBy.N1.isZero." );

  // toDivide.N1 is not zero here.
  // Base for N1 is Int56::Base56.

  // divBy is > 1 here.
  Int64 divBy = divideBy.N0.getAsLong();

  // ((N1 * base) + N0) / divBy
  // ((N1 * base) / divBy))    + (N0 / divBy)
  // (N1 * (base / divBy))     + (N0 / divBy)

  // Base is Q * divBy + remain
  // So  (Q * divBy) / divBy + (remain / divBy)
  // N1 * (Q + (remain / divBy))
  // (N1 * Q) + (N1 * (remain / divBy))
  // (N1 * Q) + (N1 * remain) / divBy))

  Int64 leftBase64 = Int56::Base56 / divBy;
  // Int64 leftBaseRemain = Int56::Base56 % divBy;

  Int56 leftBase;
  leftBase.setFromLong( leftBase64 );

  Int56 leftTerm56;
  Int112 leftTerm112;

  leftTerm56.multiply( leftBase, toDivide.N1 );
  leftTerm112.setFromFullInt56( leftTerm56 );

  Int56 rightRemainder56;
  Int56 rightTerm;
  rightTerm.divide( toDivide.N0,
                    divideBy.N0,
                    rightRemainder56 );

  Int112 quotient112;
  quotient112.setFromFullInt56( rightTerm );
  quotient112.add( leftTerm112 );
  quotient.copy( quotient112 );

  Int112 testDiv;
  Int112 testQuot;

  testQuot.copy( quotient112 );
  testDiv.multiply( testQuot, divideBy );

  Int112 diffRemain;
  diffRemain.copy( toDivide );
  diffRemain.subtract( testDiv );

  if( divideBy.paramIsGreaterOrEq( diffRemain ))
    throw "Int112 divide diffRemain too big.";

  // rightRemainder56
  // remainder.setFromLong( leftBaseRemain );
  // testDiv.add( remainder );

  // if( !testDiv.isEqual( toDivide ))
    // throw "Int112 divide is bad for N1 0.";

  // Base is Q * divBy + remain
  // So  (Q * divBy / divBy + (remain / divBy)
  // Then multiply that by N1.

  if( diffRemain.isZero())
    return true;

  return false;
  }


// The number toDivide can have N2 and N3
// not zero.  Because they just got multiplied.

throw "This needs work.";

// Divide two big numbers...
// What was that leftShift() thing called?
// For the top digits?
// Top two digits of ToDivide,
// which is all of its digits,
// but the top one digit of divideBy.


// return false;
}


*/
