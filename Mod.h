// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once


// Modular Arithmetic.


#include "../CppBase/BasicTypes.h"
#include "../CppInt/Integer.h"
#include "../CppInt/NumbSys.h"


class Mod
  {
  private:
  bool testForCopy = false;
  NumbSys numbSys;

  public:
  inline Mod( void )
    {
    }

  inline Mod( const Mod& in )
    {
    if( in.testForCopy )
      return;

    throw "Mod copy constructor.";
    }

  inline ~Mod( void )
    {
    }


  void reduce( Integer& result,
               const Integer& toReduce,
               const Integer& modulus,
               IntegerMath& intMath );

  void makeExact( Integer& exact,
                  const Integer& modulus,
                  IntegerMath& intMath );

  void toPower( Integer& result,
                const Integer& exponent,
                const Integer& modulus,
                IntegerMath& intMath );

  void verifyInBaseRange(
                     const Integer& toCheck,
                     const Integer& modulus,
                     const char* errorS );

  void verifyMoreThanZero(
                      const Integer& toCheck );

  void add( Integer& result,
            const Integer& toAdd,
            const Integer& modulus,
            IntegerMath& intMath );

  void addL( Integer& result,
             const Int64 toAdd,
             const Integer& modulus,
             IntegerMath& intMath );

  void negate( Integer& result,
               const Integer& modulus,
               IntegerMath& intMath );

  void subtract( Integer& result,
                 const Integer& toSub,
                 const Integer& modulus,
                 IntegerMath& intMath );

  void multiply( Integer& result,
                 const Integer& toMul,
                 const Integer& modulus,
                 IntegerMath& intMath );

  void multiplyL( Integer& result,
                  const Int64 toMul,
                  const Integer& modulus,
                  IntegerMath& intMath );

  // void square( Integer& result,
  //             const Integer& modulus,
  //             IntegerMath& intMath );

  bool divide( Integer& result,
               const Integer& numerator,
               const Integer& divisor,
               const Integer& modulus,
               Integer& gcd,
               IntegerMath& intMath );

  };
