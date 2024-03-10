// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "Integer.h"
#include "../CppBase/StIO.h"
#include "../CppBase/CharBuf.h"
#include "../CppBase/ByteSet64.h"
#include "../CryptoBase/Randomish.h"
#include "../CppBase/RangeC.h"
#include "../CppBase/Casting.h"



#include "../CppMem/MemoryWarnTop.h"



void Integer::setIndex( const Int32 setTo )
{
RangeC::test2( setTo, 0, last - 1,
               "Integer.setIndex() range." );

index = setTo;
}



void Integer::copy( const Integer& from )
{
index = from.index;
negative = from.negative;

const Int32 max = index;
for( Int32 count = 0; count <= max; count++ )
  dArray[count] = from.dArray[count];

}



void Integer::copyUpTo( const Integer& from,
                        const Int32 where )
{
RangeC::test2( where, 0,
               last - 1,
               "Integer.copyUpTo() range." );

negative = from.negative;
index = where;

const Int32 max = where;
for( Int32 count = 0; count <= max; count++ )
  dArray[count] = from.dArray[count];

}


Int64 Integer::getD( const Int32 where ) const
{
RangeC::test2( where, 0, last - 1,
               "Integer.getD() range." );

return dArray[where];
}



void Integer::setD( const Int32 where,
                    const Int64 toSet )
{
RangeC::test2( where, 0, last - 1,
               "Integer.setD() range." );

RangeC::test2( toSet, 0, Int24BitMask,
               "Integer.setD() toSet range." );

dArray[where] = toSet;
}



void Integer::setD48( const Int32 where,
                      const Int64 toSet )
{
RangeC::test2( where, 0, last - 1,
               "Integer.setD48() range." );

RangeC::test2( toSet, 0, Int48BitMask,
               "Integer.setD48() toSet range." );

dArray[where] = toSet;
}



void Integer::setFromInt24( const Int32 toSet )
{
RangeC::test2( toSet, 0, Int24BitMask,
              "Integer.setFromInt24() size." );

negative = false;
dArray[0] = toSet;
index = 0;
}



void Integer::setFromLong48( const Int64 toSet )
{
RangeC::test2( toSet, 0, Int48BitMask,
       "Integer.setFromLong48() toSet size." );

negative = false;

dArray[0] = toSet & Int24BitMask;
dArray[1] = toSet >> 24;

if( dArray[1] == 0 )
  index = 0;
else
  index = 1;

}


bool Integer::isEqual( const Integer& x ) const
{
if( negative != x.negative )
  return false;

// The quickest way to return false.
if( dArray[0] != x.dArray[0] )
  return false;

if( index != x.index )
  return false;

const Int32 max = index;
for( Int32 count = 1; count <= max; count++ )
  {
  if( dArray[count] != x.dArray[count] )
    return false;

  }

return true;
}



bool Integer::paramIsGreater( const Integer& x )
                                          const
{
// Get the absolute value, so ignore the
// negative value.
// if( negative )

if( index != x.index )
  {
  if( x.index > index )
    return true;
  else
    return false;

  }

// Indexes are the same:
for( Int32 count = index; count >= 0; count-- )
  {
  if( dArray[count] != x.dArray[count] )
    {
    if( x.dArray[count] > dArray[count] )
      return true;
    else
      return false;

    }
  }

return false; // It was equal, but not greater.
}



bool Integer::paramIsGreaterOrEq(
                     const Integer& x ) const
{
if( isEqual( x ))
  return true;

return paramIsGreater( x );
}



void Integer::carry( void )
{
Int64 carry = dArray[0] >> 24;
dArray[0] = dArray[0] & Int24BitMask;

const Int32 max = index;
for( Int32 count = 1; count <= max; count++ )
  {
  Int64 total = carry + dArray[count];
  dArray[count] = total & Int24BitMask;
  carry = total >> 24;
  }

if( carry != 0 )
  {
  index++;
  if( index >= last )
    throw  "Integer.carry() overflow.";

  dArray[index] = carry;
  }
}



void Integer::increment( void )
{
dArray[0] += 1;
if( (dArray[0] >> 24) == 0 )
  return;

carry();
}




void Integer::decrement( void )
{
dArray[0] -= 1;
if( dArray[0] >= 0 )
  return;

borrow();
}




void Integer::addLong48( Int64 toAdd )
{
RangeC::test2( toAdd, 0, Int48BitMask,
        "Integer.addLong48() toAdd range." );

dArray[0] += toAdd & Int24BitMask;

if( index == 0 )
  {
  dArray[1] = toAdd >> 24;
  if( dArray[1] != 0 )
    index = 1;

  }
else
  {
  dArray[1] += toAdd >> 24;
  }

if( index == 0 )
  {
  if( (dArray[0] >> 24) == 0 )
    return; // Nothing to Carry.

  }
else
  {
  // index >= 1 )
  if( ((dArray[0] >> 24) == 0) &&
      ((dArray[1] >> 24) == 0))
    return; // Nothing to Carry.

  }

carry();
}



void Integer::add( const Integer& toAdd )
{
// There is a separate IntegerMath.add() that
// is a wrapper to handle negative numbers too.
if( negative )
  throw "Integer.add is negative.";

if( toAdd.negative )
  throw "Integer.add toAdd is negative.";

if( toAdd.isLong48() )
  {
  addLong48( toAdd.getAsLong48() );
  return;
  }

const Int32 thisIndex = index;
const Int32 toAddIndex = toAdd.index;
if( thisIndex < toAddIndex )
  {
  for( Int32 count = thisIndex + 1;
                  count <= toAddIndex; count++ )
    dArray[count] = toAdd.dArray[count];

  for( Int32 count = 0; count <= thisIndex;
                                      count++ )
    dArray[count] += toAdd.dArray[count];

  index = toAddIndex;
  }
else
  {
  for( Int32 count = 0; count <= toAddIndex;
                                         count++ )
    dArray[count] += toAdd.dArray[count];

  }

carry();
}






// If one index is shorter then it is doing
// xor on zeros.  So it stays the same.
/*
What am I doing with this?
void Integer::xorBits( const Integer& x,
                       const Integer& y )
{
Int32 biggestIndex = x.index;
if( y.index > biggestIndex )
  biggestIndex = y.index;

Int32 smallestIndex = x.index;
if( y.index < smallestIndex )
  smallestIndex = y.index;

for( Int32 count = 0; count <= biggestIndex;
                                   count++ )
  {
  if( x.index < count )
    {
    dArray[count] = y.dArray[count];
    continue;
    }

  if( y.index < count )
    {
    dArray[count] = x.dArray[count];
    continue;
    }

  dArray[count] = x.dArray[count] xor
                             y.dArray[count];
  }

index = biggestIndex;

for( Int32 count = index; count > 0; count-- )
  {
  if( dArray[count] != 0 )
    break;

  index--;
  }
}
*/




void Integer::andBits( const Integer& x,
                       const Integer& y )
{
// Anything higher than smallestIndex is the
// same as AND with zero.

Int32 smallestIndex = y.index;
if( x.index < smallestIndex )
  smallestIndex = x.index;

index = smallestIndex;

for( Int32 count = 0; count <= smallestIndex;
                                   count++ )
  {
  dArray[count] = x.dArray[count] &
                            y.dArray[count];
  }

for( Int32 count = index; count > 0; count-- )
  {
  if( dArray[count] != 0 )
    break;

  index--;
  }
}




/*
// This is an optimization for small squares.
void Integer::square0( void )
{
// If this got called then Index is 0.
Int64 square = D[0] * D[0];
D[0] = square & 0xFFFFFF;
D[1] = square >> 24;
if( D[1] != 0 )
  index = 1;

}



void Integer::square1( void )
{
// If this got called then Index is 1.
Int64 D0 = D[0];
Int64 D1 = D[1];

// If you were mu]ltiplying 23 * 23 on paper
// it would look like:
//                            2     3
//                            2     3
//                           3*2   3*3
//                     2*2   2*3

// And then you add up the columns.
//                           D1    D0
//                           D1    D0
//                         M1_0  M0_0
//                   M2_1  M1_1

// Top row:
Int64 M0_0 = D0 * D0;
Int64 M1_0 = D0 * D1;

// Second row:
// Int64 M1_1 = M1_0; // Avoiding D1 * D0 again.
Int64 M2_1 = D1 * D1;

// Add them up:
D[0] = M0_0 & 0xFFFFFF;
Int64 carry = M0_0 >> 24;

Int64 M0Right = M1_0 & 0xFFFFFF;
Int64 M0Left = M1_0 >> 24;

// Avoiding a redundancy:
// M1_1 is the same as M1_0.

Int64 total = M0Right + M0Right + carry;
D[1] = total & 0xFFFFFF;
carry = total >> 24;
carry += M0Left + M0Left;
total = (M2_1 & 0xFFFFFF) + carry;
D[2] = total & 0xFFFFFF;
carry = total >> 24;
carry += (M2_1 >> 24);
index = 2;
if( carry != 0 )
  {
  index++;
  D[3] = carry;
  }

// Bitwise multiplication with two bits is:
//       1  1
//       1  1
//     ------
//       1  1
//    1  1
// ----------
// 1  0  0  1
// Biggest bit is at position 3 (zero based index).
// Adding Indexes: (1 + 1) + 1.

//       1  0
//       1  0
//       0  0
//    1  0
//    1  0  0
// Biggest bit is at 2.
// Adding Indexes: (1 + 1).

// 7 * 7 = 49
//                 1  1  1
//                 1  1  1
//                --------
//                 1  1  1
//              1  1  1
//           1  1  1
//          --------------
//        1  1  0  0  0  1
//       32 16           1 = 49
// Biggest bit is at 5 (2 + 2) + 1.
// The highest bit is at either index +
// index or it's at index + index + 1.

// For this Integer class the Index might have to
// be incremented once for a Carry, but not more
// than once.
}



void Integer::square2( void )
{
// If this got called then Index is 2.
Int64 D0 = D[0];
Int64 D1 = D[1];
Int64 D2 = D[2];

//                   M2_0   M1_0  M0_0
//            M3_1   M2_1   M1_1
//     M4_2   M3_2   M2_2

// Top row:
Int64 M0_0 = D0 * D0;
Int64 M1_0 = D0 * D1;
Int64 M2_0 = D0 * D2;

// Second row:
// M1_1 = M1_0;
Int64 M2_1 = D1 * D1;
Int64 M3_1 = D1 * D2;

// Third row:
// M2_2 = M2_0;
// M3_2 = M3_1;
Int64 M4_2 = D2 * D2;

// Add them up:
D[0] = M0_0 & 0xFFFFFF;
Int64 carry = M0_0 >> 24;

// D[1]
Int64 M0Right = M1_0 & 0xFFFFFF;
Int64 M0Left = M1_0 >> 24;
// M1Right = M1_1 & 0xFFFFFF;
// M1Left = M1_1 >> 24;
Int64 total = M0Right + M0Right + carry;
D[1] = total & 0xFFFFFF;
carry = total >> 24;
carry += M0Left + M0Left;

// D[2]
M0Right = M2_0 & 0xFFFFFF;
M0Left = M2_0 >> 24;
Int64 M1Right = M2_1 & 0xFFFFFF;
Int64 M1Left = M2_1 >> 24;
// M2Right = M2_2 & 0xFFFFFF;
// M2Left = M2_2 >> 24;
total = M0Right + M1Right + M0Right + carry;
D[2] = total & 0xFFFFFF;
carry = total >> 24;
carry += M0Left + M1Left + M0Left;

// D[3]
M1Right = M3_1 & 0xFFFFFF;
M1Left = M3_1 >> 24;
// M2Right = M3_2 & 0xFFFFFF;
// M2Left = M3_2 >> 24;
total = M1Right + M1Right + carry;
D[3] = total & 0xFFFFFF;
carry = total >> 24;
carry += M1Left + M1Left;

// D[4]
Int64 M2Right = M4_2 & 0xFFFFFF;
Int64 M2Left = M4_2 >> 24;
total = M2Right + carry;
D[4] = total & 0xFFFFFF;
carry = total >> 24;
carry += M2Left;
index = 4;
if( carry != 0 )
  {
  index++;
  D[5] = carry;
  }
}
*/



void Integer::shiftLeft( const Int32 shiftBy )
{
if( shiftBy > 24 )
  throw "Integer.shiftLeft shiftBy > 24.";

Int64 carry = 0;
for( Int32 count = 0; count <= index; count++ )
  {
  Int64 digit = dArray[count];
  digit <<= shiftBy;
  dArray[count] = digit & Int24BitMask;
  dArray[count] |= carry;
  carry = digit >> 24;
  }

if( carry != 0 )
  {
  index++;
  if( index >= last )
    throw "Integer.shiftLeft overflowed.";

  dArray[index] = carry;
  }
}



void Integer::shiftDigitsLeft(
                       const Int32 howMany )
{
if( howMany == 0 )
  return;

if( howMany < 0 )
  throw "Integer.shiftDigitsLeft negative.";

const Int32 max = index;

if( (max + howMany) >= last )
  throw "Integer.shiftDigitsLeft too big.";

for( Int32 count = max; count >= 0; count-- )
  dArray[count + howMany] = dArray[count];

for( Int32 count = 0; count < howMany; count++ )
  dArray[count] = 0;

index = index + howMany;
}



void Integer::shiftRight( const Int32 shiftBy )
{
if( shiftBy > 24 )
  throw "Integer.shiftRight shiftBy > 24.";

Int64 carry = 0;
for( Int32 count = index; count >= 0; count-- )
  {
  Int64 digit = dArray[count] << 24;

  // What happens with a sign bit is
  // implementation defined.

  if( digit < 0 )
    throw "Integer.shiftRight negative.";

  digit >>= shiftBy;
  dArray[count] = digit >> 24;
  dArray[count] |= carry;
  carry = digit & Int24BitMask;
  }

if( dArray[index] == 0 )
  {
  if( index > 0 )
    index--;

  }

// Let it shift bits over the edge.
// if( Carry != 0 )
  // throw "ShiftRight() Carry not zero.";

}




// This is used in some algorithms to set one
// particular digit and have all other digits set
// to zero.
void Integer::setDigitAndClear( const Int32 where,
                                const Int64 toSet )
{
RangeC::test2( where, 0, last - 1,
   "Integer.setDigitAndClear() where range." );

// This would lead to an undefined number that's
// zero but not zero since the Index isn't zero.
if( (toSet == 0) && (where != 0) )
  throw "setDigitAndClear() with bad zero.";

if( (toSet >> 24) != 0 )
  throw "setDigitAndClear() toSet too big.";

index = where;
dArray[index] = toSet;
for( Int32 count = 0; count < index; count++ )
  dArray[count] = 0;

}




bool Integer::makeRandomOdd(
                        const Int32 setToIndex )
{
negative = false;
if( setToIndex > (last - 3))
  throw "MakeRandomOdd index is too high.";

Int32 howManyBytes = setToIndex * 3;
CharBuf cBuf;

// Make extra bytes with times 2.
Randomish::makeRandomBytes( cBuf,
                            howManyBytes * 2 );

index = setToIndex;
Int32 where = 0;
for( Int32 count = 0; count <= setToIndex;
                                        count++ )
  {
  Uint32 rand32 = cBuf.getU32( where );

  // It's 24 bits per digit.
  // Also mask a sign bit to zero.
  rand32 = rand32 & Int24BitMask;
  Int64 digit = Casting::u64ToI32( rand32 );
  dArray[count] = digit;
  where += 4;
  }

// Make sure there isn't a zero at the top.
if( dArray[index] == 0 )
  {
  // StIO::putS( "zero at top of random bytes." );
  return false;
  }

if( (dArray[index] >> 16) == 0 )
  {
  // StIO::putS(
  //    "zero at very top of random bytes." );
  return false;
  }

// Test:
for( Int32 count = 0; count <= index; count++ )
  {
  if( dArray[count] < 0 )
    throw
     "dArray[Count] < 0 MakeRandom().";

  if( (dArray[count] >> 24) != 0 )
    throw
     "(dArray[Count] >> 24) != 0 MakeRandom().";
  }

dArray[0] |= 1; // Make it odd.
return true;
}



// Because of the standards used with TLS,
// this will typically have one leading zero
// byte so that it doesn't get confused with
// a negative sign bit.  Sometimes it will,
// sometimes it won't.

void Integer::setFromBigEndianCharBuf(
                        const CharBuf& cBuf )
{
const Int32 howMany = cBuf.getLast();

// 24 bits per digits.  3 Bytes.
if( (howMany / 3) > (last - 3))
  throw "Integer.setFromBigEndian() too big.";

negative = false;
index = 0;

for( Uint32 count = 0; count < last; count++ )
  dArray[count] = 0;

CharBuf allBytes;
allBytes.copy( cBuf );
allBytes.reverse(); // Now the least significant
                    // byte is at zero.

// StIO::putS( "allBytes hex: " );
// allBytes.showHex();

Int32 offset = 0;
for( Int32 count = 0; count < howMany; count++ )
  {
  Uint8 byteToSet = allBytes.getU8( count );
  Int64 digit = dArray[index];
  digit = ByteSet64::setByte( offset,
                              digit,
                              byteToSet );

  dArray[index] = digit;

  offset++;
  if( offset == 3 )
    {
    offset = 0;
    index++;
    if( index >= (last - 1) )
      throw
      "Integer.setFromBigEndian() too big 2.";

    }
  }

// Make sure it doesn't have leading zeros.
for( Int32 count = index; count >= 0; count-- )
  {
  if( dArray[count] != 0 )
    {
    index = count;
    break;
    }
  }
}



/*
void Integer::getBigEndianByteArray(
                             Uint8Array& toGet )
{
toGet.clear();

for( Uint32 count = 0; count <= index; count++ )
  {
  for( Uint32 offset = 0; offset < 4; offset++ )
    {
    Uint8 oneByte = getOneByteFromDValue( count,
                                       offset );
    toGet.appendVal( oneByte );
    }
  }

toGet.reverse();

// Now the most significant byte is at Result[0].
// toGet might have leading zero bytes here.
}
*/




/*
void Integer::setBitMask( const Int32 where,
                          const Int64 toSet )
{
if( (toSet >> 24) != 0 )
  throw "Integer.setBitMask() toSet range.";

index = where;
for( Int32 count = 0; count <= index; count++ )
  dArray[count] = toSet;

}
*/



void Integer::borrow( void )
{
const Int32 max = index;
for( Int32 count = 0; count < max; count++ )
  {
  if( dArray[count] < 0 )
    {
    // Borrowing from the next higher digit.
    dArray[count] += Int24BitMask + 1;
    dArray[count + 1]--;
    }
  }

if( dArray[max] < 0 )
  throw "Integer.borrow() negative.";

for( Int32 count = max; count >= 0; count-- )
  {
  if( dArray[count] != 0 )
    {
    index = count;
    return;
    }
  }

// If this was zero it wouldn't find a nonzero
// digit to set the index to and it would end
// up down here.
index = 0;
}



void Integer::subtract( const Integer& toSub )
{
if( negative )
  throw "Integer.subtract() negative.";

if( toSub.negative )
  throw "Integer.subtract() toSub negative.";

if( toSub.index > index )
  throw
     "Integer.subtract() toSub.index > index.";

const Int32 max = toSub.index;
for( Int32 count = 0; count <= max; count++ )
  dArray[count] -= toSub.dArray[count];

borrow();
}



void Integer::multiply24( const Int64 toMul )
{
RangeC::test2( toMul, 0, Int24BitMask,
   "Integer.multiply24 toMul range." );

if( toMul == 0 )
  {
  setToZero();
  return;
  }

if( toMul == 1 )
  return;

// If this was negative then it would just
// stay negative.

const Int32 max = index;
for( Int32 count = 0; count <= max; count++ )
  dArray[count] *= toMul;

carry();
}




// This destroys this number.
Int32 Integer::getModDestruct(
                   const Int64 divisor )
{
if( index == 0 )
  {
  Int32 result = Casting::i64ToI32(
                      dArray[0] % divisor );
  return result;
  }

const Int32 max = index;

// If you had a base-10 number and you wanted
// to find the remainder when divided by a
// single digit from 1 to 9 then you would
// be doing this same thing.
// 7123 divided by 6 is
// 7 / 6 leaves remainder 1.
// 1123 / 6 is
// 11 / 6 leaves remainder 5.
// 523 / 6 is
// 52 / 6 leaves remainder 4.
// 43 / 6 leave remainder 1.


Int64 remainder = 0;
Int64 oneDigit = dArray[max];

if( divisor <= oneDigit )
  {
  remainder = oneDigit % divisor;
  dArray[max] = remainder;
  }

for( Int32 count = max; count >= 1; count-- )
  {
  Int64 twoDigits = dArray[count];
  twoDigits <<= 24;
  twoDigits |= dArray[count - 1];
  remainder = twoDigits % divisor;

  // This is not necessary for just a remainder:
  // dArray[count] = 0;

  dArray[count - 1] = remainder;
  }

return Casting::i64ToI32( remainder );
}




#include "../CppMem/MemoryWarnBottom.h"
