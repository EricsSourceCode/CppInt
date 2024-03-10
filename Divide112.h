/*


// Copyright Eric Chauvin 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppInt/Int112.h"



class Divide112
  {
  private:
  bool testForCopy = false;
  Int112 BaseN1;

  public:
  Divide112( void )
    {
    }

  Divide112( const Divide112& in )
    {
    if( in.testForCopy )
      return;

    throw "Divide112 copy constructor.";
    }

  inline ~Divide112( void )
    {
    }

  bool divide( const Int112& toDivide,
               const Int112& divideBy,
               Int112& quotient,
               Int112& remainder );

  };

*/

