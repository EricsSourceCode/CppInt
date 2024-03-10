// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



#include "../CppBase/BasicTypes.h"
#include "IntConst.h"
#include "../CppBase/CharBuf.h"



#include "../CppMem/MemoryWarnTop.h"



class Integer
  {
  private:
  bool testForCopy = false;
  bool negative = false;
  static const Int32 last =
                     IntConst::DigitArraySize;

  // I want this to be allocated on the stack.
  // See the /STACK option in BuildProj.bat
  Int64 dArray[last] = { 0,1,2 };
  // Int64* dArray;

  Int32 index = 0;

  public:
  //                               0xFF FF FF
  static const Int32 Int24BitMask = 0xFFFFFF;

    //                0x FF FF FF FF FF FF
  static const Int64 Int48BitMask =
                            0xFFFFFFFFFFFFLL;


  Integer( void )
    {
    setToZero();
    }

  Integer( const Integer& in )
    {
    if( in.testForCopy )
      return;

    throw "Integer copy constructor.";
    }

  ~Integer( void )
    {
    }

  inline void setToZero( void )
    {
    negative = false;
    index = 0;
    dArray[0] = 0;
    }

  inline void setToOne( void )
    {
    negative = false;
    index = 0;
    dArray[0] = 1;
    }


  inline bool isZero( void ) const
    {
    if( (index == 0) && (dArray[0] == 0) )
      return true;

    return false;
    }

  inline bool isOne( void ) const
    {
    if( negative )
      return false;

    if( (index == 0) && (dArray[0] == 1) )
      return true;

    return false;
    }

  inline bool isNegativeOne( void ) const
    {
    if( !negative )
      return false;

    if( (index == 0) && (dArray[0] == 1) )
      return true;

    return false;
    }

  inline Int32 getIndex( void ) const
    {
    return index;
    }


  void setIndex( const Int32 setTo );
  void copy( const Integer& from );
  void copyUpTo( const Integer& from,
                 const Int32 where );

  Int64 getD( const Int32 where ) const;
  void setD( const Int32 where,
             const Int64 toSet );
  void setD48( const Int32 where,
               const Int64 toSet );


  inline bool isMoreThanInt24( const Int32 check )
                                           const
    {
    if( negative )
      return false;

    if( index > 0 )
      return true;

    if( dArray[0] > check )
      return true;

    return false;
    }



  inline bool isEqualToInt24( const Int32 toTest )
                                          const
    {
    if( negative )
      return false;

    if( index > 0 )
      return false;

    if( dArray[0] != toTest )
      return false;

    return true;
    }


  bool isEqualToInt48( const Int64 toTest )
                                        const
    {
    if( negative )
      return false;

    if( index > 1 )
      return false;

    if( dArray[0] != (toTest & Int24BitMask))
      return false;

    Int64 shifted = toTest >> 24;
    if( index == 0 )
      {
      if( shifted == 0 )
        return true;
      else
        return false;

      }

    if( shifted != dArray[1] )
      return false;

    return true;
    }

  bool isEqual( const Integer& x ) const;

  inline bool getNegative( void ) const
    {
    return negative;
    }

  inline void setNegative( bool setTo )
    {
    if( setTo )
      {
      if( (index == 0) && (dArray[0] == 0))
        throw "Setting zero to negative.";

      }

    negative = setTo;
    }


  inline void incrementIndex( void )
    {
    index++;
    if( index >= last )
      throw "Integer incrementIndex() overflow.";

    }

  inline bool isLong48( void ) const
    {
    if( negative )
      return false;

    if( index > 1 )
      return false;

    return true;
    }


  inline Int64 getAsLong48( void ) const
    {
    if( negative )
      throw "Integer.getAsLong48() negative.";

    if( index > 1 )
      throw "Integer.getAsLong48() too big.";

    if( index == 0 )
      return dArray[0];

    Int64 result = dArray[1] << 24;
    result |= dArray[0];
    return result;
    }

  void setFromInt24( const Int32 toSet );
  void setFromLong48( const Int64 toSet );
  bool paramIsGreater( const Integer& x ) const;
  bool paramIsGreaterOrEq(
                     const Integer& x ) const;
  void carry( void );
  void increment( void );
  void decrement( void );
  void addLong48( Int64 toAdd );
  void add( const Integer& toAdd );
  void shiftLeft( const Int32 shiftBy );
  void shiftDigitsLeft( const Int32 howMany );
  void shiftRight( const Int32 shiftBy );
  bool makeRandomOdd( const Int32 setToIndex );
  void borrow( void );
  void subtract( const Integer& toSub );
  void multiply24( const Int64 toMul );
  Int32 getModDestruct( const Int64 divisor );
  void setDigitAndClear( const Int32 where,
                         const Int64 toSet );
  // void xorBits( const Integer& x,
  //              const Integer& y );

  void andBits( const Integer& x,
                const Integer& y );

  void setFromBigEndianCharBuf(
                        const CharBuf& cBuf );

  };


#include "../CppMem/MemoryWarnBottom.h"
