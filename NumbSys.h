// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once


// A Number System used for Modular Reduction.

// As far as I know I invented this idea for
// doing fast modular reduction.


#include "../CppBase/BasicTypes.h"
#include "Integer.h"
#include "IntegerMath.h"
#include "../CppBase/StIO.h"



class NumbSys
  {
  private:
  bool testForCopy = false;
  static const Int32 last =
                   IntConst::DigitArraySize;

  Integer currentBase;
  Integer* intAr;

  void setupBaseArray( const Integer& setBase,
                       IntegerMath& intMath );

  public:
  NumbSys( void );
  NumbSys( const NumbSys& in );
  ~NumbSys( void );

  void reduce( Integer& result,
               const Integer& toReduce,
               const Integer& modulus,
               IntegerMath& intMath );

  };
