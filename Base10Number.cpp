// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#include "Base10Number.h"


// -Wno-unsafe-buffer-usage

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"


Base10Number::Base10Number( const CharBuf& toSet )
{
D = new Int32[digitArraySize];
setFromCharBuf( toSet );
}


Base10Number::Base10Number(
                        const Base10Number& obj )
{
if( obj.testForCopy )
  return;

throw "Base10Number copy constructor.";
}


Base10Number::~Base10Number( void )
{
delete[] D;
}



void Base10Number::setFromCharBuf(
                          const CharBuf& toSet )
{
index = 0;
D[0] = 0;

const Int32 last = toSet.getLast();
if( last < 1 )
  return;

Int32 where = 0;
for( Int32 count = last - 1; count >= 0; count-- )
  {
  if( where >= digitArraySize )
    throw "Base10Number: Too big for array.";

  Int32 c = toSet.getC( count );

  // Ignore white space, commas, non digits.
  if( (c < '0') || (c > '9'))
    continue;

  Int32 digit = convertDigit( c );

  // Test what convertDigit() returned.
  if( digit > 9 )
    throw "Base10Number: digit > 9.";

  D[where] = digit;
  where++;
  }

if( where == 0 )
  {
  // No valid digit was ever set.
  throw "Base10Number: no valid digits.";
  }

index = where - 1;
}

#pragma clang diagnostic pop

