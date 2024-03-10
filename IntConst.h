// Copyright Eric Chauvin 2023



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once


#include "../CppBase/BasicTypes.h"



class IntConst
  {
  private:

  public:
  // It has 24 bits per digit.
  static const Int32 DigitArraySize =
                    ((1024 * 16) / 24) + 16;


  };
