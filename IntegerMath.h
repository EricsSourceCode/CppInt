// Copyright Eric Chauvin, 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



#include "../CppBase/BasicTypes.h"
// #include "../CppBase/FileIO.h"
#include "../CppBase/CharBuf.h"
#include "Integer.h"
#include "../CryptoBase/SPrimes.h"



class IntegerMath
  {
  private:
  bool testForCopy = false;


  void setMultiplySign( Integer& result,
                        const Integer& toMul );

  void searchSqrtXPart( const Int32 testIndex,
                        const Integer& fromSqr,
                        Integer& sqrRoot );


   public:
  IntegerMath( void )
    {
    }

  IntegerMath( const IntegerMath& in )
    {
    if( in.testForCopy )
      return;

    throw "Copy constructor: IntegerMath.";
    }

  ~IntegerMath( void )
    {
    }


  static Int64 find64SqrRoot(
                          const Int64 toMatch );

  Int32 isDivisibleBySmallPrime(
                        const Integer& toTest,
                        const SPrimes& sPrimes );

  void add( Integer& result,
                      const Integer& toAdd );

  void subtract( Integer& result,
                         const Integer& toSub );

  void multiplyLong48( Integer& result,
                       const Int64 toMul );

  void multiply( Integer& result,
                 const Integer& toMul );

  void setFromStr( Integer& result,
                   const CharBuf& in );

  // void setFromHexStr( Integer& result,
  //                    const CharBuf& in );

  void toString10( const Integer& from,
                   CharBuf& toSet );

  // void toString16( const Integer& from,
  //                 CharBuf& toSet );

  // void square( Integer& toSquare );


  Int32 getMod24( const Integer& in,
                  const Int64 divisor );

/*
  Int64 getMod48( const Integer& in,
                  const Int64 divisor );
  static Int64 mod48FromTwoLongs( const Int64 P1,
                           const Int64 P0,
                           const Int64 divisor );

*/

  void multiplyTop( Integer& result,
                    const Integer& toMul );

  void multiplyTopOne( Integer& result,
                       const Integer& toMul );

  bool squareRoot( const Integer& fromSqr,
                   Integer& sqrRoot );

  void setsqrRootTop( const Integer& fromSqr,
                      Integer& sqrRoot );


  };
