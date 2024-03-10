/*

// Copyright Eric Chauvin 2021 - 2023.

// ====== Make it so the bases don't
// go as big as the digitArraySize.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once


// A Number System used for Modular Reduction.
// This is the Full version with quotients.


#include "../CppBase/BasicTypes.h"
#include "Integer.h"
#include "IntegerMath.h"
#include "../CppBase/StIO.h"



class NumbSysFull
  {
  private:
  bool testForCopy = false;
  static const Int32 last = 100;
                 //  IntConst::DigitArraySize;

  Integer currentBase;
  Integer* intAr;
  Integer* intArQ;

  public:
  NumbSysFull( void );
  NumbSysFull( const NumbSysFull& in );
  ~NumbSysFull( void );

  void setupBaseArray( const Integer& setBase,
                       IntegerMath& intMath );

  void reduce( Integer& result,
               const Integer& toReduce,
               const Integer& modulus,
               IntegerMath& intMath );

  };
*/
