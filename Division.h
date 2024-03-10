// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



#include "../CppBase/BasicTypes.h"
#include "Integer.h"
#include "IntegerMath.h"


class Division
  {
  private:

  public:
  static bool shortDivide(
            const Integer& toDivideOriginal,
            const Integer& divideByOriginal,
            Integer& quotient,
            Integer& remainder,
            IntegerMath& intMath );

  static Int64 shortDivideRem(
                const Integer& toDivideOriginal,
                const Int64 divideByU,
                Integer& quotient );

  static void divide(
                const Integer& toDivideOriginal,
                const Integer& divideByOriginal,
                Integer& quotient,
                Integer& remainder,
                IntegerMath& intMath );

  static bool longDivide1(
                    const Integer& toDivide,
                    const Integer& divideBy,
                    Integer& quotient,
                    Integer& remainder,
                    IntegerMath& intMath );

  static void testDivideBits(
                      const Int64 maxValue,
                      const bool isTop,
                      const Int32 testIndex,
                      const Integer& toDivide,
                      const Integer& divideBy,
                      Integer& quotient,
                      // Integer& remainder,
                      IntegerMath& intMath );

  static bool longDivide2(
                      const Integer& toDivide,
                      const Integer& divideBy,
                      Integer& quotient,
                      Integer& remainder,
                      IntegerMath& intMath );

  static Int32 findShiftBy( Int64 toTest );

  static void longDivide3(
                      const Integer& toDivide,
                      const Integer& divideBy,
                      Integer& quotient,
                      Integer& remainder,
                      IntegerMath& intMath );

  };
