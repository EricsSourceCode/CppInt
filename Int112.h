/*

// Copyright Eric Chauvin 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once


#include "../CppBase/BasicTypes.h"
#include "../CppBase/CharBuf.h"
#include "Int56.h"
#include "../CryptoBase/SPrimes.h"



class Int112
  {
  private:
  bool testForCopy = false;

  public:
  Int56 N0;
  Int56 N1;
  Int56 N2;
  Int56 N3;

  Int112( void )
    {
    setZero();
    }

  Int112( const Int112& in )
    {
    if( in.testForCopy )
      return;

    throw "Int112 copy constructor.";
    }

  ~Int112( void )
    {
    }


  inline void setZero( void )
    {
    N0.setZero();
    N1.setZero();
    N2.setZero();
    N3.setZero();
    }


  inline void setOne( void )
    {
    N0.setOne();
    N1.setZero();
    N2.setZero();
    N3.setZero();
    }

  inline Int64 getAsLong( void )
    {
    if( !N1.isZero())
      throw "Int112.getAsLong() too big.";

    return N0.getAsLong();
    }


  inline void setFromLong( const Int64 toSet )
    {
    if( toSet >= Int56::Base56 )
      throw "Int56 setFromLong toSet too big.";

    N0.setFromLong( toSet );
    N1.setZero();
    N2.setZero();
    N3.setZero();
    }


  inline void setFromInt56( Int56& toSet )
    {
    N0.setFromLong( toSet.getAsLong());
    N1.setZero();
    N2.setZero();
    N3.setZero();
    }


  inline bool isZero( void ) const
    {
    if( !N0.isZero())
      return false;

    if( !N1.isZero())
      return false;

    if( !N2.isZero())
      return false;

    if( !N3.isZero())
      return false;

    return true;
    }


  inline bool isOne( void ) const
    {
    if( !N0.isOne())
      return false;

    if( !N1.isZero())
      return false;

    if( !N2.isZero())
      return false;

    if( !N3.isZero())
      return false;

    return true;
    }

  inline bool isEven( void ) const
    {
    if( N0.isEven())
      return true;

    return false;
    }


  inline void inc( void )
    {
    N0.inc();
    if( !N0.highPartIsZero())
      cleanCarry();

    }

  inline void dec( void )
    {
    N0.dec();
    if( N0.isNegative())
      cleanBorrow();

    }


  inline Int64 shiftRight(
                     const Int32 howMany )
    {
    if( !N2.isZero() || !N3.isZero())
      throw "Int112 shiftRight top bits.";

    Int64 result = N0.shiftRight( howMany );

    Int64 rightBits = N1.shiftRight( howMany );
    rightBits <<= 56 - howMany; // Rotate left.

    Int64 N0bits = N0.getAsLong();
    N0bits |= rightBits;
    N0.setFromLong( N0bits );
    return result;
    }



  void copy( const Int112& from );

  bool makeRandomOdd(
               const Int32 howManyBytes );

  void setByteAt( const Int32 where,
                  const Uint8 toSet );

  void setFromCharBufBigEnd(
                       const CharBuf& cBuf );

  Int64 getMod( const Int64 x ) const;

  Int32 isDivisibleBySmallPrime(
              const SPrimes& sPrimes ) const;

  bool paramIsGreater(
                      const Int112& x ) const;
  bool paramIsGreaterOrEq(
                      const Int112& x ) const;
  bool isEqual( const Int112& x ) const;

  void multiply( const Int112& x,
                 const Int112& y );

  void setFromFullInt56( const Int56& setFrom );
  void cleanCarry( void );
  void add( const Int112& toAdd );
  void subtract( const Int112& toSub );
  void cleanBorrow( void );

  };

*/

