// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once


#include "../CppBase/BasicTypes.h"
#include "../CppBase/CharBuf.h"


// -Wno-unsafe-buffer-usage

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"


class Base10Number
  {
  // The number string 123456789 is put in the
  // array of digits like this:
  // D[0] = 9; // Least significant digit.
  // D[1] = 8;
  // D[2] = 7;
  // D[3] = 6;
  // D[4] = 5;
  // D[5] = 4;
  // D[6] = 3;
  // D[7] = 2;
  // D[8] = 1; // Most significant digit.

  private:
  bool testForCopy = false;

  // For 1024 bits it's about 309 decimal digits.
  static const Int32 digitArraySize =
                                   (310 * 4) + 2;
  Int32 index = 0;
  Int32* D;

  public:
  Base10Number( const CharBuf& toSet );
  Base10Number( const Base10Number& obj );
  ~Base10Number( void );

  inline Int32 getD( const Int32 where ) const
    {
    if( where >= digitArraySize )
      throw "getD() index out of bounds.";

    return D[where];
    }

  inline Int32 getIndex( void ) const
    {
    return index;
    }

  inline Int32 convertDigit( const Int32 digit )
    {
    if( (digit > '9') || (digit < '0') )
      throw "Base10Number: convertDigit().";

    return (digit - '0');
    }

  void setFromCharBuf( const CharBuf& toSet );

  };

#pragma clang diagnostic pop

