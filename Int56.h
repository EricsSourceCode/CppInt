// Copyright Eric Chauvin 2023


// This will evolve in to using SIMD
// instructions.  SSE.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once


#include "../CppBase/BasicTypes.h"
#include "../CryptoBase/SPrimes.h"



class Int56
  {
  private:
  bool testForCopy = false;

  Int64 N0 = 0;
  Int64 N1 = 0;
  Int64 N2 = 0;
  Int64 N3 = 0;

  static inline Uint8 getByte( const Int32 where,
                               const Int64 from )
    {
    Int64 from2 = from;
    from2 >>= 8 * where;
    Uint8 aByte = from2 & 0xFF;
    return aByte;
    }


  static inline Int64 eraseByte(
                      const Int32 where,
                      const Int64 toErase )
    {
    Int64 bitsMask = 0xFF;
    bitsMask <<= 8 * where;
    bitsMask = ~bitsMask;

    Int64 result = toErase & bitsMask;

    // Some of these tests can be commented
    // out once it is tested.

    if( result < 0 )
      throw "Int56.eraseByte result < 0";

    return result;
    }



  static inline Int64 setByte( const Int32 where,
                               const Int64 toSet,
                               const Uint8 aByte )
    {
    if( where < 0 )
      throw "Int56.setByte() where < 0 )";

    if( where > 6 )
      throw "Int56.setByte() where > 6 )";

    if( toSet >= Base56 )
      throw
       "Int56.setByte() toSet too big Base56.";

    Int64 result = eraseByte( where, toSet );

    if( result >= Base56 )
      throw
       "Int56.setByte() erased too big Base56.";

    Int64 bigByte = 0;
    bigByte |= aByte;

    bigByte <<= 8 * where;

    if( bigByte >= Base56 )
      throw
       "Int56.setByte() bigByte too big Base56.";

    result |= bigByte;

    if( result >= Base56 )
      throw "Int56.setByte()  too big Base56.";

    return result;
    }


  public:
  //                             0xFFF FFFF;
  static const Int64 BitMask28 =  0xFFFFFFF;
  static const Int64 BitMask56 =
                          0xFFFFFFFFFFFFFF;
  static const Int64 Base28    = 0x10000000LL;
  static const Int64 Base56  = Base28 * Base28;

  Int56( void )
    {
    setZero();
    }

  Int56( const Int56& in )
    {
    if( in.testForCopy )
      return;

    throw "Int56 copy constructor.";
    }

  ~Int56( void )
    {
    }

  inline Uint8 getByteAt(
                    const Int32 where ) const
    {
    // Index 0 is first byte.

    if( where < 0 )
      throw "Int56.getByteAt() negative.";

    // Index 6 for byte 7.
    if( where > 6 )
      throw "Int56.getByteAt() where > 6.";

    Int64 allBytes = N1;
    allBytes <<= 28;
    allBytes |= N0;

    return getByte( where, allBytes );
    }


  inline void setByteAt(
                    const Int32 where,
                    const Uint8 toSet )
    {
    if( where < 0 )
      throw "Int56.setByteAt() negative.";

    // 7 bytes in this 56 bit number.
    // Index 6 for byte 7.

    if( where > 6 )
      throw "Int56.setByteAt() where > 6.";

    Int64 allBytes = N1;
    allBytes <<= 28;
    allBytes |= N0;

    Int64 toSetTest = 0;
    toSetTest |= toSet;
    allBytes = setByte( where, allBytes, toSet );

    if( allBytes < toSetTest )
      throw "allBytes < toSetTest";

    if( allBytes >= Base56 )
      throw "Int56.setByteAt()  too big Base56.";

    N0 = allBytes & BitMask28;
    N1 = (allBytes >> 28) & BitMask28;
    }

  inline bool isNegative( void ) const
    {
    if( N3 < 0 )
      return true;

    return false;
    }


  inline void setZero( void )
    {
    N0 = 0;
    N1 = 0;
    N2 = 0;
    N3 = 0;
    }

  inline void setOne( void )
    {
    N0 = 1;
    N1 = 0;
    N2 = 0;
    N3 = 0;
    }

  inline void inc( void )
    {
    N0++;
    if( N0 >= Base28 )
      cleanCarry();

    }

  inline void dec( void )
    {
    N0--;
    if( N0 < 0 )
      cleanBorrow();

    }


  inline Int64 shiftRight( const Int32 howMany )
    {
    if( howMany > 48 )
      throw "Int56 shiftRight > 48 bits.";

    if( (N2 != 0) || (N3 != 0))
      throw "Int56 shiftRight top bits.";

    Int64 result = N1;
    result <<= 28;
    result |= N0;

    Int64 full56 = result;
    full56 >>= howMany;

    N0 = full56 & BitMask28;
    N1 = (full56 >> 28) & BitMask28;

    Int64 mask = BitMask56 << howMany;
    mask = ~mask;

    // Return what got shifted off.
    return result & mask;
    }



  inline Int64 getMod( const Int64 x ) const
    {
    if( x == 0 )
      throw "Int56 getMod() x == 0.";

    Int64 result = N1;
    result <<= 28;
    result |= N0;

    return result % x;
    }


  inline Int64 getAsLong( void ) const
    {
    if( (N2 != 0) ||
        (N3 != 0))
      throw "Int56.getAsLong() too big N2.";

    Int64 result = N1;
    result <<= 28;
    result |= N0;

    if( result >= Base56 )
      throw "Int56.getAsLong() too big Base56.";

    return result;
    }


  inline void setFromLong( const Int64 toSet )
    {
    if( toSet < 0 )
      throw "Int56 setFromLong toSet negative.";

    if( (toSet >> 56) != 0 )
      throw
         "Int56 setFromLong toSet >> 56) != 0.";

    N0 = toSet & BitMask28;
    N1 = (toSet >> 28) & BitMask28;
    N2 = 0;
    N3 = 0;
    }

  inline void addLong( const Int64 toAdd )
    {
    if( (toAdd >> 56) != 0 )
      throw
         "Int56 addLong() toAdd >> 56) != 0.";

    N0 += toAdd & BitMask28;
    N1 += (toAdd >> 28) & BitMask28;

    cleanCarry();
    }

  inline void addBase56( void )
    {
    N2 += 1;

    cleanCarry();
    }

  inline void cleanCarry( void )
    {
    Int64 carry = N0 >> 28;
    N0 = N0 & BitMask28;

    N1 += carry;
    carry = N1 >> 28;
    N1 = N1 & BitMask28;

    N2 += carry;
    carry = N2 >> 28;
    N2 = N2 & BitMask28;

    N3 += carry;
    carry = N3 >> 28;
    N3 = N3 & BitMask28;

    if( carry != 0 )
      throw "Int56 overflow.";

    }


  inline void add( const Int56& toAdd )
    {
    N0 += toAdd.N0;
    N1 += toAdd.N1;
    N2 += toAdd.N2;
    N3 += toAdd.N3;

    cleanCarry();
    }


  inline void subtract( const Int56& toSub )
    {
    N0 -= toSub.N0;
    N1 -= toSub.N1;
    N2 -= toSub.N2;
    N3 -= toSub.N3;

    cleanBorrow();
    }


  inline void cleanBorrow( void )
    {
    if( N0 < 0 )
      {
      N1--;
      N0 += Base28;
      }

    if( N1 < 0 )
      {
      N2--;
      N1 += Base28;
      }

    if( N2 < 0 )
      {
      N3--;
      N2 += Base28;
      }

    // if( N3 < 0 )
      // It is negative.

    }


  inline bool isZero( void ) const
    {
    if( N0 != 0)
      return false;

    if( N1 != 0)
      return false;

    if( N2 != 0)
      return false;

    if( N3 != 0)
      return false;

    return true;
    }


  inline bool isOne( void ) const
    {
    if( (N0 == 1) &&
        (N1 == 0) &&
        (N2 == 0) &&
        (N3 == 0))
      return true;

    return false;
    }


  inline bool isEven( void ) const
    {
    if( (N0 & 1) == 0 )
      return true;

    return false;
    }

  inline bool isEqual( const Int56& x ) const
    {
    if( (N0 == x.N0) &&
        (N1 == x.N1) &&
        (N2 == x.N2) &&
        (N3 == x.N3))
      {
      return true;
      }

    return false;
    }


  inline bool paramIsGreater(
                    const Int56& x ) const
    {
    if( x.N1 > N1 )
      return true;

    if( x.N1 < N1 )
      return false;

    // N1s are equal here.
    if( x.N0 > N0 )
      return true;

    // They might be equal.
    return false;
    }

  inline bool paramIsGreaterOrEq(
                    const Int56& x ) const
    {
    if( x.N1 > N1 )
      return true;

    if( x.N1 < N1 )
      return false;

    // N1s are equal here.
    if( x.N0 >= N0 )
      return true;

    return false;
    }



  inline void copy( const Int56& from )
    {
    N0 = from.N0;
    N1 = from.N1;
    N2 = from.N2;
    N3 = from.N3;
    }


  void multiply( const Int56& x,
                 const Int56& y );


  inline void getLowPart( Int56& toGet ) const
    {
    toGet.N0 = N0;
    toGet.N1 = N1;
    toGet.N2 = 0;
    toGet.N3 = 0;
    }

  inline bool highPartIsZero( void ) const
    {
    if( (N2 == 0) && (N3 == 0 ))
      return true;

    return false;
    }

  inline void getHighPart( Int56& toGet ) const
    {
    toGet.N0 = N2;
    toGet.N1 = N3;
    toGet.N2 = 0;
    toGet.N3 = 0;
    }

  inline void clearHighPart( void )
    {
    N2 = 0;
    N3 = 0;
    }


  bool divide( const Int56& toDivide,
               const Int56& divideBy,
               Int56& remainder );

  Int32 isDivisibleBySmallPrime(
               const SPrimes& sPrimes ) const;

  };
